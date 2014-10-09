#pragma once

#include <tgCore/VertexAttributeList.hpp>

#include <GL/gl.h>

#include <memory>

namespace tgCore
{
    class VertexBuffer
        : public std::enable_shared_from_this< VertexBuffer >
    {
        public:
            class IndexAccess
            {
                public:
                    friend class VertexBuffer;

                    static void end( IndexAccess* access );

                    void pushIndex( GLuint i );
                    void pushIndex( GLuint i1, GLuint i2, GLuint i3 );

                private:
                    IndexAccess( std::shared_ptr< VertexBuffer > vertexBuffer );

                    void push( void* data, size_t size );

                    std::shared_ptr< VertexBuffer > m_vertexBuffer;
            };

            class VertexAccess
            {
                public:
                    friend class VertexBuffer;

                    static void end( VertexAccess* access );

                    GLuint pushFloat( GLfloat v );
                    GLuint pushFloat( GLfloat v1, GLfloat v2 );
                    GLuint pushFloat( GLfloat v1, GLfloat v2, GLfloat v3 );
                    GLuint pushFloat( GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4 );

                private:
                    VertexAccess( std::shared_ptr< VertexBuffer > vertexBuffer );

                    void push( void* data, size_t size );

                    std::shared_ptr< VertexBuffer > m_vertexBuffer;
            };

            VertexBuffer( GLenum mode, std::shared_ptr< VertexAttributeList > attributes );
            ~VertexBuffer();

            std::shared_ptr< IndexAccess > accessIndices();
            std::shared_ptr< VertexAccess > accessVertices();

            void render() const;
            
        private:
            bool m_editVertices;
            size_t m_vSize;
            size_t m_vCapacity;
            void* m_vBuffer;
            GLuint m_vBufferObject;

            bool m_editIndices;
            size_t m_iSize;
            size_t m_iCapacity;
            void* m_iBuffer;
            GLuint m_iBufferObject;

            GLenum m_mode;
            std::shared_ptr< VertexAttributeList > m_attributes;
    };



    inline void VertexBuffer::IndexAccess::pushIndex( GLuint i )
    {
        push( reinterpret_cast< void* >( &i ), sizeof( GLuint ) );
    }

    inline void VertexBuffer::IndexAccess::pushIndex( GLuint i1, GLuint i2, GLuint i3 )
    {
        pushIndex( i1 );
        pushIndex( i2 );
        pushIndex( i3 );
    }

    inline GLuint VertexBuffer::VertexAccess::pushFloat( GLfloat v )
    {
        GLuint index = m_vertexBuffer->m_vSize / m_vertexBuffer->m_attributes->getBlockSize();
        push( reinterpret_cast< void* >( &v ), sizeof( GLfloat ) );
        return index;
    }

    inline GLuint VertexBuffer::VertexAccess::pushFloat( GLfloat v1, GLfloat v2 )
    {
        GLuint index = m_vertexBuffer->m_vSize / m_vertexBuffer->m_attributes->getBlockSize();
        push( reinterpret_cast< void* >( &v1 ), sizeof( GLfloat ) );
        push( reinterpret_cast< void* >( &v2 ), sizeof( GLfloat ) );
        return index;
    }

    inline GLuint VertexBuffer::VertexAccess::pushFloat( GLfloat v1, GLfloat v2, GLfloat v3 )
    {
        GLuint index = m_vertexBuffer->m_vSize / m_vertexBuffer->m_attributes->getBlockSize();
        push( reinterpret_cast< void* >( &v1 ), sizeof( GLfloat ) );
        push( reinterpret_cast< void* >( &v2 ), sizeof( GLfloat ) );
        push( reinterpret_cast< void* >( &v3 ), sizeof( GLfloat ) );
        return index;
    }

    inline GLuint VertexBuffer::VertexAccess::pushFloat( GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4 )
    {
        GLuint index = m_vertexBuffer->m_vSize / m_vertexBuffer->m_attributes->getBlockSize();
        push( reinterpret_cast< void* >( &v1 ), sizeof( GLfloat ) );
        push( reinterpret_cast< void* >( &v2 ), sizeof( GLfloat ) );
        push( reinterpret_cast< void* >( &v3 ), sizeof( GLfloat ) );
        push( reinterpret_cast< void* >( &v4 ), sizeof( GLfloat ) );
        return index;
    }
}
