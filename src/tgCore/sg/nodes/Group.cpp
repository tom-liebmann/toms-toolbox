#include "Group.hpp"

using namespace tg::sg::nodes;

void Group::traverseChildren(
    std::function< void ( const Node* ) > callback ) const
{
    for( const auto& child : m_children )
        callback( child.get() );
}
