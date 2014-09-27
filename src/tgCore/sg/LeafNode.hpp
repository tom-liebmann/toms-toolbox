#pragma once

#include <tgCore/sg/Node.hpp>

namespace tgCore
{
    namespace sg
    {
        class LeafNode
            : public Node
        {
            public:
                virtual ~LeafNode();

                // Node
                virtual bool isLeaf() const override;

                virtual uint32_t getRenderPass() const = 0;
                virtual void render() const = 0;
        };



        inline LeafNode::~LeafNode()
        { }

        inline bool LeafNode::isLeaf() const
        {
            return true;
        }
    }
}
