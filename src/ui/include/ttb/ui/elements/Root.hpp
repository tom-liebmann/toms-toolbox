#pragma once

#include <ttb/ui/Element.hpp>

#include <ttb/math/Range.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/utils/EventListener.hpp>

#include <memory>


namespace ttb::ui
{
    class Framework;
}


namespace ttb::ui
{
    class Root : public Element
    {
    public:
        Root( Framework& framework, Size const& size );

        void child( Element* child );

        void setPriorityListener( EventListener* listener );

        EventListener* getPriorityListener() const;

        virtual void destroy() override;

        virtual void update( float timeDiff ) override;

        virtual void render( ttb::State& state ) const override;

        virtual void onChildChanged( Element& child ) override;

        virtual bool onEvent( Event const& event ) override;

    private:
        Element* m_child{ nullptr };
        EventListener* m_priorityListener{ nullptr };
    };
}
