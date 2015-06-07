#include "SceneGraph.hpp"

#include <tg/core/sg/Instance.hpp>
#include <tg/core/sg/LeafNode.hpp>
#include <tg/core/sg/Modifier.hpp>

#include <list>
#include <algorithm>

using namespace tg::sg;

std::unique_ptr< SceneGraph::State > SceneGraph::traverse()
{
    std::unique_ptr< State > state( new State() );

    std::list< const Node* > nodeStack;
    std::list< const Modifier* > modifierList;
    std::list< std::list< const Modifier* >::iterator > modifierStack;

    nodeStack.push_front( &m_root );

    while( !nodeStack.empty() )
    {
        const Node* node = nodeStack.front();
        nodeStack.pop_front();

        if( !node )
        {
            modifierList.erase( modifierStack.back() );
            modifierStack.pop_back();
        }
        else
        {
            node->traverseModifier(
                [ &modifierList,
                  &modifierStack,
                  &nodeStack ] ( const Modifier* modifier )
                {
                    auto iter = modifierList.begin();
                    while( iter != modifierList.end()
                        && (*iter)->getPriority() >= modifier->getPriority() )
                        ++iter;

                    modifierStack.push_back( modifierList.insert( iter, modifier ) );
                    nodeStack.push_front( nullptr );
                } );

            if( node->isLeaf() )
            {
                state->pushInstance( new Instance(
                    modifierList, static_cast< const LeafNode* >( node ) ) );
            }
            else
            {
                static_cast< const InnerNode* >( node )->traverseChildren(
                    [ &nodeStack ] ( const Node* child )
                    {
                        nodeStack.push_front( child );
                    } );
            }
        }
    }

    return state;
}

SceneGraph::State::~State()
{
    for( auto& pass : m_passes )
        for( auto instance : std::get< 1 >( pass.second ) )
            delete instance;
}

void SceneGraph::State::pushInstance(
    Instance* instance )
{
    // since std::map does not support std::unique_ptr correctly yet, we
    // have to do a workaround

    auto iter = m_passes.find( instance->getLeafNode()->getRenderPass() );
    if( iter != m_passes.end() )
        std::get< 1 >( iter->second ).push_back( instance );
    else
        m_passes.emplace(
            instance->getLeafNode()->getRenderPass(),
            std::make_tuple(
                false,
                std::vector< Instance* >( { instance } ) ) );
}

namespace
{
    bool sortInstances(
        Instance* instance1,
        Instance* instance2 )
    {
        return std::lexicographical_compare(
            instance1->getModifiers().begin(), instance1->getModifiers().end(),
            instance2->getModifiers().begin(), instance2->getModifiers().end() );
    }
}

void SceneGraph::State::render( tg::State& state, uint32_t pass )
{
    auto iter = m_passes.find( pass );
    if( iter != m_passes.end() )
    {
        auto& instances = std::get< 1 >( iter->second );
        if( !std::get< 0 >( iter->second ) )
        {
            std::sort( instances.begin(), instances.end(), sortInstances );
            std::get< 0 >( iter->second ) = true;
        }

        Instance* last = nullptr;
        std::list< const Modifier* >::iterator lIter; 
        for( auto& current : instances )
        {
            auto cIter = current->getModifiers().begin();

            if( last )
            {
                // check which modifiers are the same
                auto lIter = last->getModifiers().begin();
                while( cIter != current->getModifiers().end()
                    && lIter != last->getModifiers().end()
                    && (*cIter) == (*lIter) )
                {
                    ++cIter;
                    ++lIter;
                }

                // remove old modifiers
                auto iter = last->getModifiers().end();
                while( iter != lIter )
                {
                    --iter;
                    ( *iter )->end( state );
                }
            }

            // add new modifiers
            while( cIter != current->getModifiers().end() )
            {
                ( *cIter )->begin( state );
                ++cIter;
            }

            current->getLeafNode()->render( state );

            last = current;
        }

        if( last )
        {
            for( auto iter = last->getModifiers().rbegin(); iter != last->getModifiers().rend(); ++iter )
                ( *iter )->end( state );
        }
    }
}
