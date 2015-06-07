#include "Group.hpp"

namespace tg
{
    namespace sg
    {
        namespace nodes
        {
            void Group::traverseChildren(
                std::function< void ( const Node* ) > callback ) const
            {
                for( const auto& child : m_children )
                    callback( child.get() );
            }
        }
    }
}
