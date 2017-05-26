#pragma once

#include <GL/glew.h>

#include <GL/gl.h>

#include <memory>
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

    private:
        Geometry( GLenum mode, std::vector< Attribute > attributes,
                  std::shared_ptr< IndexBuffer > const& indexBuffer );

        GLenum m_mode;
        GLuint m_arrayObject;
        std::vector< Attribute > m_attributes;
        size_t m_minAttributeSize;
        std::shared_ptr< IndexBuffer > m_indexBuffer;
    };



    class Geometry::Attribute
    {
    public:
        Attribute( std::string const& name, std::shared_ptr< VertexBuffer > const& vertexBuffer,
                   size_t index );

        std::string const& name() const;

        VertexBuffer const& buffer() const;

        size_t index() const;

    private:
        std::string m_name;
        std::shared_ptr< VertexBuffer > m_buffer;
        size_t m_index;
    };



    class Geometry::Creator
    {
    public:
        Creator& attribute( std::string const& name, std::shared_ptr< VertexBuffer > const& buffer,
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
