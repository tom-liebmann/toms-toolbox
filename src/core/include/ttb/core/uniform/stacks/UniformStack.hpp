#pragma once

#include <ttb/core/shader/Program.hpp>

#include <stack>


namespace ttb
{
    class Program;
    class State;
}


namespace ttb
{
    class UniformStackBase
    {
    public:
        virtual ~UniformStackBase();

    private:
        virtual void apply( GLuint location ) = 0;

        friend class Program;
    };


    template < typename TType >
    class UniformStack : public UniformStackBase
    {
    public:
        using ValueType = TType;

        class Handle;

        Handle push( ValueType const& value );

        ValueType const& value() const;

    private:
        virtual void apply( GLuint location ) override;

        std::stack< ValueType > m_valueStack;
    };


    template < typename TType >
    class UniformStack< TType >::Handle
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
    template < typename TType >
    inline typename UniformStack< TType >::Handle
        UniformStack< TType >::push( ValueType const& value )
    {
        m_valueStack.push( value );
        return { *this };
    }

    template < typename TType >
    inline TType const& UniformStack< TType >::value() const
    {
        return m_valueStack.top();
    }

    template < typename TType >
    void UniformStack< TType >::apply( GLuint location )
    {
        helper::set_uniform< TType >::apply( location, value() );
    }


    template < typename TType >
    UniformStack< TType >::Handle::~Handle()
    {
        m_stack.m_valueStack.pop();
    }

    template < typename TType >
    UniformStack< TType >::Handle::Handle( UniformStack& stack ) : m_stack{ stack }
    {
    }
}
