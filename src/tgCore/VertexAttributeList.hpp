#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <list>

namespace tgCore
{
    class VertexAttribute;

    class VertexAttributeList
    {
        public:
            VertexAttributeList( std::initializer_list< VertexAttribute > attributes );

            std::list< VertexAttribute >::iterator begin();
            std::list< VertexAttribute >::iterator end();

            GLsizei getBlockSize() const;

        private:
            std::list< VertexAttribute > m_attributes;
            GLsizei m_blockSize;
    };



    inline std::list< VertexAttribute >::iterator VertexAttributeList::begin()
    {
        return m_attributes.begin();
    }

    inline std::list< VertexAttribute >::iterator VertexAttributeList::end()
    {
        return m_attributes.end();
    }

    inline GLsizei VertexAttributeList::getBlockSize() const
    {
        return m_blockSize;
    }
}
