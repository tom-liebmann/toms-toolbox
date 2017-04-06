#pragma once

#include <ttb/core/VertexAttributeList.hpp>

#include <iostream>
#include <memory>
#include <vector>

// forward declarations
//=============================================================================

namespace ttb
{
    class State;
}



// declarations
//=============================================================================

namespace ttb
{
    class VertexBuffer
    {
    public:
        class Access;

        VertexBuffer( GLenum mode, std::shared_ptr< VertexAttributeList > attributes );

        ~VertexBuffer();

        std::shared_ptr< Access > access();

        void render( State& state ) const;

    private:
        Access* m_access;

        std::vector< uint8_t > m_vBuffer;
        GLuint m_vBufferObject;

        std::vector< GLuint > m_iBuffer;
        GLuint m_iBufferObject;

        GLenum m_mode;
        std::shared_ptr< VertexAttributeList > m_attributes;
    };



    class VertexBuffer::Access
    {
    public:
        static void end( Access* access );

        template < class... T >
        void pushI( GLuint index, T... rest );

        void pushI( GLuint index );

        template < class... T >
        GLuint pushV( T... data );

        void clearI();
        void clearV();
        void clear();

    private:
        Access( VertexBuffer* vertexBuffer );

        template < class T >
        void pushVRest( T value );

        template < class T, class... U >
        void pushVRest( T value, U... rest );

        VertexBuffer* m_vertexBuffer;
        bool m_iChanged;
        bool m_vChanged;

        friend class VertexBuffer;
    };



    //============================================== Template definitions =====

    template < class... T >
    void VertexBuffer::Access::pushI( GLuint index, T... rest )
    {
        pushI( index );
        pushI( rest... );
    }

    template < class... T >
    GLuint VertexBuffer::Access::pushV( T... data )
    {
        GLuint index =
            m_vertexBuffer->m_vBuffer.size() / m_vertexBuffer->m_attributes->getBlockSize();
        pushVRest( data... );
        return index;
    }

    template < class T >
    void VertexBuffer::Access::pushVRest( T value )
    {
        m_vChanged = true;
        m_vertexBuffer->m_vBuffer.insert(
            m_vertexBuffer->m_vBuffer.end(), reinterpret_cast< uint8_t* >( &value ),
            reinterpret_cast< uint8_t* >( &value ) + sizeof( value ) );
    }

    template < class T, class... U >
    void VertexBuffer::Access::pushVRest( T value, U... rest )
    {
        pushVRest( value );
        pushVRest( rest... );
    }



    //================================================ Inline definitions =====

    inline void VertexBuffer::Access::pushI( GLuint index )
    {
        m_iChanged = true;
        m_vertexBuffer->m_iBuffer.push_back( index );
    }

    inline void VertexBuffer::Access::clearI()
    {
        m_iChanged = true;
        m_vertexBuffer->m_iBuffer.clear();
    }

    inline void VertexBuffer::Access::clearV()
    {
        m_vChanged = true;
        m_vertexBuffer->m_vBuffer.clear();
    }

    inline void VertexBuffer::Access::clear()
    {
        clearI();
        clearV();
    }
}
