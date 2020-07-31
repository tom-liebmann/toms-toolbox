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


    template < typename TType, size_t TRows, size_t TCols >
    class UniformStack< Matrix< TType, TRows, TCols > >::Handle
    {
    public:
        Handle( Handle const& copy ) = delete;
        Handle& operator=( Handle const& copy ) = delete;

        ~Handle();

    private:
        explicit Handle( UniformStack& stack );

        UniformStack& m_stack;

        friend class UniformStack;
    };

}


namespace ttb
{
    template < typename TType, size_t TRows, size_t TCols >
    inline typename UniformStack< Matrix< TType, TRows, TCols > >::Handle
        UniformStack< Matrix< TType, TRows, TCols > >::push( ValueType const& value )
    {
        if( m_valueStack.empty() )
        {
            return set( value );
        }
        else
        {
            return set( this->value() * value );
        }
    }

    template < typename TType, size_t TRows, size_t TCols >
    inline typename UniformStack< Matrix< TType, TRows, TCols > >::Handle
        UniformStack< Matrix< TType, TRows, TCols > >::set( ValueType const& value )
    {
        m_valueStack.push( value );

        return Handle{ *this };
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


    template < typename TType, size_t TRows, size_t TCols >
    UniformStack< Matrix< TType, TRows, TCols > >::Handle::~Handle()
    {
        m_stack.m_valueStack.pop();
    }

    template < typename TType, size_t TRows, size_t TCols >
    UniformStack< Matrix< TType, TRows, TCols > >::Handle::Handle( UniformStack& stack )
        : m_stack{ stack }
    {
    }
}
