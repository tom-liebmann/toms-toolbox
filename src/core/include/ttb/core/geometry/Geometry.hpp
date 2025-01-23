#pragma once

#include <ttb/core/State.hpp>
#include <ttb/core/gl.hpp>

#include <memory>
#include <string>
#include <vector>


// forward declarations
//=================================================================================================

namespace ttb
{
    class State;
    class IndexBuffer;
    class VertexBuffer;
}



// declarations
//=================================================================================================

namespace ttb
{
    class Geometry
    {
    public:
        class Attribute;
        class Builder;

        ~Geometry();

        void draw( State::Data& data ) const;

        Attribute const& attribute( size_t index ) const;

        Attribute& attribute( size_t index );

        GLuint arrayObject() const;

    private:
        Geometry( GLenum mode,
                  std::vector< Attribute > attributes,
                  std::shared_ptr< IndexBuffer > const& indexBuffer );

        GLenum m_mode;
        GLuint m_arrayObject;
        std::vector< Attribute > m_attributes;
        std::shared_ptr< IndexBuffer > m_indexBuffer;
    };


    class Geometry::Builder
    {
    public:
        Builder( GLenum mode );

        auto addAttribute( std::string const& name,
                           std::shared_ptr< VertexBuffer > buffer,
                           std::size_t index = 0 ) -> Builder&;

        auto addIndices( std::shared_ptr< IndexBuffer > indexBuffer ) -> Builder&;

        auto build() -> std::unique_ptr< Geometry >;

    private:
        GLenum m_mode;
        std::vector< Attribute > m_attributes;
        std::shared_ptr< IndexBuffer > m_indexBuffer;
    };



    class Geometry::Attribute
    {
    public:
        Attribute( std::string const& name,
                   std::shared_ptr< VertexBuffer > vertexBuffer,
                   size_t index );

        std::string const& name() const;

        std::shared_ptr< VertexBuffer > const& buffer() const;

        size_t index() const;

        void buffer( std::shared_ptr< VertexBuffer > buffer, size_t index );

    private:
        std::string m_name;
        std::shared_ptr< VertexBuffer > m_buffer;
        size_t m_index;
    };
}
