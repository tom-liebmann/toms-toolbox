#include "Group.hpp"

using tgCore::sg::nodes::Group;

void Group::traverseChildren(
    std::function< void ( const Node* ) > callback ) const
{
    for( const auto& child : m_children )
        callback( child.get() );
}
