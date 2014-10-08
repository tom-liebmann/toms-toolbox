#pragma once

#include <GL/gl.h>

#include <memory>

namespace tgCore
{
    class VertexAttributeList;

    class VertexBuffer
        : public std::enable_shared_from_this< VertexBuffer >
    {
        public:
            class IndexAccess
            {
                public:
                    friend class VertexBuffer;

                    static void end( IndexAccess* access );

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

                private:
                    VertexAccess( std::shared_ptr< VertexBuffer > vertexBuffer );

                    void push( void* data, size_t size );

                    std::shared_ptr< VertexBuffer > m_vertexBuffer;
            };

            VertexBuffer( std::shared_ptr< VertexAttributeList > attributes );
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

            std::shared_ptr< VertexAttributeList > m_attributes;
    };
}
