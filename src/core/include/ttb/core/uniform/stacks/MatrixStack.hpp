#pragma once

#include "UniformStack.hpp"

#include <ttb/math/Matrix.hpp>


namespace ttb
{
    template < typename TType, size_t TRows, size_t TCols >
    class UniformStack< Matrix< TType, TRows, TCols > > : public UniformStackBase
    {
    public:
        using ValueType = Matrix< TType, TRows, TCols >;

        class Handle;

        Handle push( ValueType const& value );

        Handle set( ValueType const& value );

        ValueType const& value() const;

    private:
        virtual void apply( GLuint location ) const override;

        std::stack< ValueType > m_valueStack;
    };
}


namespace ttb
{
    template < typename TType, size_t TRows, size_t TCols >
    inline typename UniformStack< Matrix< TType, TRows, TCols > >::Handle
        UniformStack< Matrix< TType, TRows, TCols > >::push( ValueType const& value )
    {
        m_valueStack.push( this->value() * value );
        return { *this };
    }

    template < typename TType, size_t TRows, size_t TCols >
    inline Matrix< TType, TRows, TCols > const&
        UniformStack< Matrix< TType, TRows, TCols > >::value() const
    {
        return m_valueStack.top();
    }

    template < typename TType, size_t TRows, size_t TCols >
    void UniformStack< Matrix< TType, TRows, TCols > >::apply( GLuint location ) const
    {
        helper::set_uniform< ValueType >::apply( location, value() );
    }
}
