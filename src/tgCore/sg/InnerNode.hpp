#pragma once

#include <tgCore/sg/Node.hpp>

namespace tgCore
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



        inline InnerNode::~InnerNode()
        { }

        inline bool InnerNode::isLeaf() const
        {
            return false;
        }
    }
}
