#pragma once

#include "uniformHelper.hpp"

#include <GL/gl.h>

#include <string>


namespace ttb
{
    class UniformBase
    {
    public:
        virtual ~UniformBase();

        virtual void apply( GLint location ) const = 0;
    };


    template < typename TType >
    class Uniform : public UniformBase
    {
    public:
        Uniform( TType const& value );

        TType const& value() const;

        template < typename TType2 >
        void value( TType2&& value );

        // Override: UniformBase
        virtual void apply( GLint location ) const override;

    private:
        TType m_value;
    };
}


// definitions
//=================================================================================================

namespace ttb
{
    template < typename TType >
    Uniform< TType >::Uniform( TType const& value ) : m_value( value )
    {
    }

    template < typename TType >
    TType const& Uniform< TType >::value() const
    {
        return m_value;
    }

    template < typename TType >
    template < typename TType2 >
    void Uniform< TType >::value( TType2&& value )
    {
        m_value = std::forward< TType2 >( value );
    }

    template < typename TType >
    void Uniform< TType >::apply( GLint location ) const
    {
        helper::set_uniform< TType >::apply( location, m_value );
    }
}
