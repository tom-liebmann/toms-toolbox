#pragma once

#include <tg/core/sg/Node.hpp>

// declarations
//=============================================================================

namespace tg
{
    namespace sg
    {
        class InnerNode
            : public Node
        {
            public:
                virtual ~InnerNode();

                // Node
                virtual bool isLeaf() const override;

                virtual void traverseChildren(
                    std::function< void ( const Node* ) > callback ) const = 0;
        };
    }
}



// definitions
//=============================================================================

namespace tg
{
    namespace sg
    {
        inline InnerNode::~InnerNode()
        { }

        inline bool InnerNode::isLeaf() const
        {
            return false;
        }
    }
}
