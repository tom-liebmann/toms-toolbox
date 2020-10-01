#pragma once

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
        class Attribute;
        class Creator;

    public:
        static Creator create( GLenum mode );

        ~Geometry();

        void draw( State& state ) const;

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



    class Geometry::Attribute
    {
    public:
        Attribute( std::string const& name,
                   std::shared_ptr< VertexBuffer > const& vertexBuffer,
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



    class Geometry::Creator
    {
    public:
        Creator& attribute( std::string const& name,
                            std::shared_ptr< VertexBuffer > const& buffer,
                            size_t index = 0 );

        Creator& indices( std::shared_ptr< IndexBuffer > const& indexBuffer );

        std::unique_ptr< Geometry > finish();

    private:
        Creator( GLenum mode );

        GLenum m_mode;
        std::vector< Attribute > m_attributes;
        std::shared_ptr< IndexBuffer > m_indexBuffer;

        friend class Geometry;
    };
}
