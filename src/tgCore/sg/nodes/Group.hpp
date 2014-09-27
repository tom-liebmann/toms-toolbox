#pragma once

#include <tgCore/sg/InnerNode.hpp>

#include <memory>
#include <set>

namespace tgCore
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

                    // InnerNode
                    virtual void traverseChildren(
                        std::function< void ( const Node* ) > callback ) const override;

                private:
                    std::set< std::shared_ptr< Node > > m_children;
            };



            inline void Group::addChild( std::shared_ptr< Node > child )
            {
                m_children.insert( std::move( child ) );
            }

            inline void Group::removeChild( const std::shared_ptr< Node >& child )
            {
                m_children.erase( child );
            }
        }
    }
}
