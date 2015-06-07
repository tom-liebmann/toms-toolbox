#pragma once

#include <core/sg/InnerNode.hpp>

#include <memory>
#include <set>

// declarations
//=============================================================================

namespace tg
{
    namespace sg
    {
        namespace nodes
        {
            class Group
                : public InnerNode
            {
                public:
                    void addChild( std::shared_ptr< Node > child );
                    void removeChild( const std::shared_ptr< Node >& child );
                    void clearChildren();

                    // InnerNode
                    virtual void traverseChildren(
                        std::function< void ( const Node* ) > callback ) const override;

                private:
                    std::set< std::shared_ptr< Node > > m_children;
            };
        }
    }
}



// definitions
//=============================================================================

namespace tg
{
    namespace sg
    {
        namespace nodes
        {
            inline void Group::addChild( std::shared_ptr< Node > child )
            {
                m_children.insert( std::move( child ) );
            }

            inline void Group::removeChild( const std::shared_ptr< Node >& child )
            {
                m_children.erase( child );
            }

            inline void Group::clearChildren()
            {
                m_children.clear();
            }
        }
    }
}
