#include "VertexAttributeList.hpp"

#include <tgCore/VertexAttribute.hpp>

using tgCore::VertexAttributeList;

VertexAttributeList::VertexAttributeList( std::initializer_list< VertexAttribute > attributes )
    : m_attributes( attributes.begin(), attributes.end() )
    , m_blockSize( 0 )
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
        }
    }
}
