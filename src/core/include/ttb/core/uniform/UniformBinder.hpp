#pragma once

#include <ttb/core/State.hpp>
#include <ttb/math/Matrix.hpp>
#include <ttb/math/matrix_operations.hpp>

#include <optional>
#include <string>
#include <utility>


namespace ttb
{
    template < typename TType >
    class UniformBinderHandle;

    template < typename TType >
    class UniformBinder
    {
    public:
        UniformBinder( std::string_view name, TType const& value, bool overwrite = false );

        UniformBinderHandle< TType > bind( State::Data& data ) const;

    private:
        std::string m_name;
        TType const& m_value;
        bool m_overwrite;
    };


    template < typename TType >
    class UniformBinderHandle
    {
    public:
        ~UniformBinderHandle();

        UniformBinderHandle( UniformBinderHandle&& rhs );
        UniformBinderHandle& operator=( UniformBinderHandle&& rhs );

        UniformBinderHandle( UniformBinderHandle const& rhs ) = delete;
        UniformBinderHandle& operator=( UniformBinderHandle const& rhs ) = delete;

    private:
        UniformBinderHandle( std::string const& name,
                             TType const& value,
                             bool overwrite,
                             State::Data& data );

        std::string m_name;
        State::Data& m_data;
        std::optional< TType > m_oldValue;

        friend UniformBinder< TType >;
    };
}


namespace ttb
{
    template < typename TType >
    UniformBinder< TType >::UniformBinder( std::string_view name,
                                           TType const& value,
                                           bool overwrite )
        : m_name{ name }, m_value{ value }, m_overwrite{ overwrite }
    {
    }

    template < typename TType >
    UniformBinderHandle< TType > UniformBinder< TType >::bind( State::Data& data ) const
    {
        return { m_name, m_value, m_overwrite, data };
    }


    template < typename TType >
    UniformBinderHandle< TType >::~UniformBinderHandle()
    {
        if( m_name.empty() )
        {
            return;
        }

        if( m_oldValue.has_value() )
        {
            auto& holder =
                static_cast< TypedUniformHolder< TType >& >( *m_data.uniforms[ m_name ] );

            holder.value( m_oldValue.value() );
        }
        else
        {
            m_data.uniforms.erase( m_name );
        }
    }

    template < typename TType >
    UniformBinderHandle< TType >::UniformBinderHandle( UniformBinderHandle&& rhs )
        : m_name{ std::exchange( rhs.m_name, {} ) }
        , m_data{ rhs.m_data }
        , m_oldValue{ rhs.m_oldValue }
    {
    }

    template < typename TType >
    auto UniformBinderHandle< TType >::operator=( UniformBinderHandle&& rhs )
        -> UniformBinderHandle&
    {
        m_name = std::exchange( rhs.m_name, {} );
        m_data = rhs.m_data;
        m_oldValue = rhs.m_oldValue;
        return *this;
    }

    template < typename TType >
    UniformBinderHandle< TType >::UniformBinderHandle( std::string const& name,
                                                       TType const& value,
                                                       bool overwrite,
                                                       State::Data& data )
        : m_name{ name }, m_data{ data }
    {
        auto const iter = m_data.uniforms.find( m_name );

        if( iter != std::end( m_data.uniforms ) )
        {
            auto& holder = static_cast< TypedUniformHolder< TType >& >( *iter->second );
            m_oldValue = holder.value();

            if constexpr( ttb::is_matrix_v< TType > )
            {
                if( overwrite )
                {
                    holder.value( value );
                }
                else
                {
                    holder.value( holder.value() * value );
                }
            }
            else
            {
                holder.value( value );
            }
        }
        else
        {
            m_data.uniforms.insert(
                { m_name, std::make_unique< TypedUniformHolder< TType > >( value ) } );
        }
    }
}
