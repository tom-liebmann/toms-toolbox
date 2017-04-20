#pragma once

#include <ttb/math/Matrix.hpp>
#include "Program.hpp"
#include "Uniform.hpp"

// declarations
//=================================================================================================

namespace ttb
{
    namespace helper
    {
        template < typename TType >
        struct set_uniform;

        template <>
        struct set_uniform< float >
        {
            static void apply( GLint location, float value );
        };

        template <>
        struct set_uniform< int >
        {
            static void apply( GLint location, int value );
        };

        template <>
        struct set_uniform< Matrix< float, 4, 4 > >
        {
            static void apply( GLint location, Matrix< float, 4, 4 > const& value );
        };
    }
}



namespace ttb
{
    template < typename TType >
    class UniformPrimitive : public UniformBase
    {
    public:
        UniformPrimitive( std::string const& name, TType const& value );

        TType const& value() const;
        void value( TType const& value );

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
    UniformPrimitive< TType >::UniformPrimitive( std::string const& name, TType const& value )
        : UniformBase( name ), m_value( value )
    {
    }

    template < typename TType >
    TType const& UniformPrimitive< TType >::value() const
    {
        return m_value;
    }

    template < typename TType >
    void UniformPrimitive< TType >::value( TType const& value )
    {
        m_value = value;
    }

    template < typename TType >
    void UniformPrimitive< TType >::apply( GLint location ) const
    {
        helper::set_uniform< TType >::apply( location, m_value );
    }
}
