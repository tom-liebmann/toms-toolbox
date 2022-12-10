#pragma once

#include <ttb/core/resources/Manager.hpp>
#include <ttb/ui/Element.hpp>

#include <list>


namespace ttb::ui
{
    class Root : public ElementParent
    {
    public:
        using Size = Element::Size;

        Root( std::shared_ptr< ttb::resources::Manager > resourceManager, Size const& size );

        void setChild( Element* child );

        void pushPriorityListener( EventListener& listener );

        void popPriorityListener();

        ttb::resources::Manager& getResourceManager();

        void update( float timeDiff );

        void render( ttb::State& state ) const;

        bool onEvent( Event const& event );

        virtual void onChildChanged( Element& child ) override;

    private:
        std::shared_ptr< ttb::resources::Manager > m_resourceManager;

        Size m_size;

        Element* m_child{ nullptr };

        std::list< EventListener* > m_priorityListeners;
    };
}
