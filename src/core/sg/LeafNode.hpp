#pragma once

#include <core/sg/Node.hpp>

// forward declarations
//=============================================================================

namespace tg
{
    class State;
}



// declarations
//=============================================================================

namespace tg
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
            virtual void render( State& state ) const = 0;
        };
    }
}



// definitions
//=============================================================================

namespace tg
{
    namespace sg
    {
        inline LeafNode::~LeafNode()
        { }

        inline bool LeafNode::isLeaf() const
        {
            return true;
        }
    }
}
