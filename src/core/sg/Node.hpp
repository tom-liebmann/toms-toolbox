#pragma once

#include <functional>

// declarations
//=============================================================================

namespace tg
{
    namespace sg
    {
        class Modifier;

        class Node
        {
        public:
            virtual ~Node();

            virtual bool isLeaf() const = 0;

            virtual void traverseModifier(
                std::function< void ( const Modifier* ) > callback ) const;
        };
    }
}



// definitions
//=============================================================================

namespace tg
{
    namespace sg
    {
        inline Node::~Node()
        { }

        inline void Node::traverseModifier(
            std::function< void ( const Modifier* ) > callback ) const
        { }
    }
}
