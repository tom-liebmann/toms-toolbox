#pragma once

#include <ttb/core/State.hpp>
#include <ttb/math/Matrix.hpp>

#include <optional>
#include <string>


namespace ttb
{
    template < typename TType >
    class UniformBinderHandle;

    template < typename TType >
    class UniformBinder
    {
    public:
        UniformBinder( std::string const& name, TType const& value );

        UniformBinderHandle< TType > bind( State::Data& data ) const;

    private:
        std::string const& m_name;
        TType const& m_value;
    };


    template < typename TType >
    class UniformBinderHandle
    {
    public:
        ~UniformBinderHandle();

    private:
        UniformBinderHandle( std::string const& name, TType const& value, State::Data& data );

        std::string const& m_name;
        State::Data& m_data;
        std::optional< TType > m_oldValue;

        friend UniformBinder< TType >;
    };


    template < typename TType, size_t TRows, size_t TCols >
    class UniformBinderHandle< Matrix< TType, TRows, TCols > >
    {
    public:
        ~UniformBinderHandle();

    private:
        UniformBinderHandle( std::string const& name,
                             Matrix< TType, TRows, TCols > const& value,
                             State::Data& data );

        std::string const& m_name;
        State::Data& m_data;
        std::optional< Matrix< TType, TRows, TCols > > m_oldValue;

        friend UniformBinder< Matrix< TType, TRows, TCols > >;
    };
}


namespace ttb
{
    template < typename TType >
    UniformBinder< TType >::UniformBinder( std::string const& name, TType const& value )
        : m_name{ name }, m_value{ value }
    {
    }

    template < typename TType >
    UniformBinderHandle< TType > UniformBinder< TType >::bind( State::Data& data ) const
    {
        return { m_name, m_value, data };
    }


    template < typename TType >
    UniformBinderHandle< TType >::~UniformBinderHandle()
    {
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
    UniformBinderHandle< TType >::UniformBinderHandle( std::string const& name,
                                                       TType const& value,
                                                       State::Data& data )
        : m_name{ name }, m_data{ data }
    {
        auto const iter = m_data.uniforms.find( m_name );

        if( iter != std::end( m_data.uniforms ) )
        {
            auto& holder = static_cast< TypedUniformHolder< TType >& >( *iter->second );
            m_oldValue = holder.value();
            holder.value( value );
        }
        else
        {
            m_data.uniforms.insert(
                { m_name, std::make_unique< TypedUniformHolder< TType > >( value ) } );
        }
    }


    template < typename TType, size_t TRows, size_t TCols >
    UniformBinderHandle< Matrix< TType, TRows, TCols > >::~UniformBinderHandle()
    {
        if( m_oldValue.has_value() )
        {
            auto& holder = static_cast< TypedUniformHolder< Matrix< TType, TRows, TCols > >& >(
                *m_data.uniforms[ m_name ] );

            holder.value( m_oldValue.value() );
        }
        else
        {
            m_data.uniforms.erase( m_name );
        }
    }

    template < typename TType, size_t TRows, size_t TCols >
    UniformBinderHandle< Matrix< TType, TRows, TCols > >::UniformBinderHandle(
        std::string const& name, Matrix< TType, TRows, TCols > const& value, State::Data& data )
        : m_name{ name }, m_data{ data }
    {
        auto const iter = m_data.uniforms.find( m_name );

        if( iter != std::end( m_data.uniforms ) )
        {
            auto& holder = static_cast< TypedUniformHolder< Matrix< TType, TRows, TCols > >& >(
                *iter->second );
            m_oldValue = holder.value();
            holder.value( holder.value() * value );
        }
        else
        {
            m_data.uniforms.insert(
                { m_name,
                  std::make_unique< TypedUniformHolder< Matrix< TType, TRows, TCols > > >(
                      value ) } );
        }
    }
}
