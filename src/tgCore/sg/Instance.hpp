#pragma once

#include <list>

// declarations
//=============================================================================

namespace tg
{
    namespace sg
    {
        class Modifier;
        class LeafNode;

        class Instance
        {
            public:
                Instance(
                    std::list< const Modifier* > modifiers,
                    const LeafNode* leafNode );

                const std::list< const Modifier* >& getModifiers() const;
                const LeafNode* getLeafNode() const;

            private:
                std::list< const Modifier* > m_modifiers;
                const LeafNode* m_leafNode;
        };
    }
}



// definitions
//=============================================================================

namespace tg
{
    namespace sg
    {
        inline Instance::Instance(
            std::list< const Modifier* > modifiers,
            const LeafNode* leafNode )
            : m_modifiers( std::move( modifiers ) )
            , m_leafNode( leafNode )
        { }

        const std::list< const Modifier* >& Instance::getModifiers() const
        {
            return m_modifiers;
        }

        const LeafNode* Instance::getLeafNode() const
        {
            return m_leafNode;
        }
    }
}
