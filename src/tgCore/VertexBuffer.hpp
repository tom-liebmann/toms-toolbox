#pragma once

#include <tgCore/VertexAttributeList.hpp>

#include <memory>
#include <iostream>

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

                    void clear();

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


                    template< class T, class... V >
                    GLuint push( T v, V... rest );

                    void clear();

                private:
                    VertexAccess( std::shared_ptr< VertexBuffer > vertexBuffer );

                    template< class T >
                    void pushRest( T v );

                    template< class T, class... V>
                    void pushRest( T v, V... rest );

                    template< class T >
                    void pushValue( T value );

                    void pushData( void* data, size_t size );

                    std::shared_ptr< VertexBuffer > m_vertexBuffer;
            };

            // due to std::enable_shared_from_this the VertexBuffer has to be created as shader ptr
            static std::shared_ptr< VertexBuffer > create( GLenum mode, std::shared_ptr< VertexAttributeList > attributes );

            ~VertexBuffer();

            std::shared_ptr< IndexAccess > accessIndices();
            std::shared_ptr< VertexAccess > accessVertices();

            void render() const;
            
        private:
            VertexBuffer( GLenum mode, std::shared_ptr< VertexAttributeList > attributes );

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



    inline std::shared_ptr< VertexBuffer > VertexBuffer::create(
        GLenum mode,
        std::shared_ptr< VertexAttributeList > attributes )
    {
        return std::shared_ptr< VertexBuffer >( new VertexBuffer(
            mode, std::move( attributes ) ) );
    }



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

    template< class T, class... V >
    inline GLuint VertexBuffer::VertexAccess::push( T v, V... rest )
    {
        GLuint index = m_vertexBuffer->m_vSize / m_vertexBuffer->m_attributes->getBlockSize();
        pushValue< T >( v );
        pushRest( rest... );
        return index;
    }

    template< class T >
    inline void VertexBuffer::VertexAccess::pushRest( T v )
    {
        pushValue< T >( v );
    }

    template< class T, class... V >
    inline void VertexBuffer::VertexAccess::pushRest( T v, V... rest )
    {
        pushValue< T >( v );
        pushRest( rest... );
    }

    template< class T >
    inline void VertexBuffer::VertexAccess::pushValue( T value )
    {
        pushData( reinterpret_cast< void* >( &value ), sizeof( T ) );
    }
}
