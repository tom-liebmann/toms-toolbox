#pragma once

#include <tgCore/sg/nodes/Group.hpp>

#include <map>
#include <vector>

namespace tgCore
{
    namespace sg
    {
        class Instance;

        class SceneGraph
        {
            public:
                class State;

                void addChild( std::shared_ptr< Node > child );
                void removeChild( const std::shared_ptr< Node >& child );
                void clearChildren();

                std::unique_ptr< State > traverse();

            private:
                nodes::Group m_root; 
        };

        class SceneGraph::State
        {
            public:
                ~State();

                void pushInstance(
                    Instance* instance );

                void render( uint32_t pass );

            private:    
                std::map<
                    uint32_t,
                    std::tuple<
                        bool,
                        std::vector< Instance* > > > m_passes;
        };



        inline void SceneGraph::addChild( std::shared_ptr< Node > child )
        {
            m_root.addChild( std::move( child ) );
        }

        inline void SceneGraph::removeChild( const std::shared_ptr< Node >& child )
        {
            m_root.removeChild( child );
        }

        inline void SceneGraph::clearChildren()
        {
            m_root.clearChildren();
        }
    }
}
