#pragma once

#include <functional>

namespace tgCore
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



        inline Node::~Node()
        { }

        inline void Node::traverseModifier(
            std::function< void ( const Modifier* ) > callback ) const
        { }
    }
}
