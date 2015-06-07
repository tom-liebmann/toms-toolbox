#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <core/VertexAttribute.hpp>

#include <vector>

// declarations
//=============================================================================

namespace tg
{
    class VertexAttributeList
    {
        public:
            VertexAttributeList( std::initializer_list< VertexAttribute > attributes );

            std::vector< VertexAttribute >::const_iterator begin() const;
            std::vector< VertexAttribute >::const_iterator end() const;

            GLsizei getBlockSize() const;

            const VertexAttribute& operator[]( size_t i ) const;

            size_t size() const;

        private:
            std::vector< VertexAttribute > m_attributes;
            GLsizei m_blockSize;
    };
}



// defintions
//=============================================================================

namespace tg
{
    inline std::vector< VertexAttribute >::const_iterator VertexAttributeList::begin() const
    {
        return m_attributes.begin();
    }

    inline std::vector< VertexAttribute >::const_iterator VertexAttributeList::end() const
    {
        return m_attributes.end();
    }

    inline GLsizei VertexAttributeList::getBlockSize() const
    {
        return m_blockSize;
    }

    inline const VertexAttribute& VertexAttributeList::operator[]( size_t i ) const
    {
        return m_attributes[ i ];
    }

    inline size_t VertexAttributeList::size() const
    {
        return m_attributes.size();
    }
}
