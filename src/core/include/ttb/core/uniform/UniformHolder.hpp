#pragma once

#include "uniformHelper.hpp"


namespace ttb
{
    class UniformHolder
    {
    public:
        virtual ~UniformHolder();

        virtual void apply( GLuint location ) const = 0;
    };


    template < typename TType >
    class TypedUniformHolder : public UniformHolder
    {
    public:
        TypedUniformHolder( TType const& value );

        void value( TType const& value );

        TType const& value() const;

        virtual void apply( GLuint location ) const override;

    private:
        TType m_value;
    };
}


namespace ttb
{
    template < typename TType >
    TypedUniformHolder< TType >::TypedUniformHolder( TType const& value ) : m_value{ value }
    {
    }

    template < typename TType >
    void TypedUniformHolder< TType >::value( TType const& value )
    {
        m_value = value;
    }

    template < typename TType >
    TType const& TypedUniformHolder< TType >::value() const
    {
        return m_value;
    }

    template < typename TType >
    void TypedUniformHolder< TType >::apply( GLuint location ) const
    {
        helper::set_uniform< TType >::apply( location, value() );
    }
}
