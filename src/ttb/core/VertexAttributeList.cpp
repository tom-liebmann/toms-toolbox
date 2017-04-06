#include "VertexAttributeList.hpp"

#include <ttb/core/VertexAttribute.hpp>

using ttb::VertexAttributeList;

VertexAttributeList::VertexAttributeList( std::initializer_list< VertexAttribute > attributes )
    : m_attributes( attributes.begin(), attributes.end() ), m_blockSize( 0 )
{
    for( auto& attribute : m_attributes )
    {
        switch( attribute.getType() )
        {
            case GL_FLOAT:
                m_blockSize += attribute.getSize() * sizeof( GLfloat );
                break;

            case GL_BYTE:
                m_blockSize += attribute.getSize() * sizeof( GLbyte );
                break;

            case GL_UNSIGNED_INT:
                m_blockSize += attribute.getSize() * sizeof( GLuint );
                break;
        }
    }
}
