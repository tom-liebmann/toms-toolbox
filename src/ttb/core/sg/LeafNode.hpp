#pragma once

#include <ttb/core/sg/Node.hpp>

// forward declarations
//=============================================================================

namespace ttb
{
    class State;
}



// declarations
//=============================================================================

namespace ttb
{
    namespace sg
    {
        class LeafNode : public Node
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

namespace ttb
{
    namespace sg
    {
        inline LeafNode::~LeafNode()
        {
        }

        inline bool LeafNode::isLeaf() const
        {
            return true;
        }
    }
}
