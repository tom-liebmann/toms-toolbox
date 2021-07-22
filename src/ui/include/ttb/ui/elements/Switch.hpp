#pragma once

#include <ttb/ui/Element.hpp>

#include <map>


namespace ttb::ui
{
    class Switch : public Element
    {
    public:
        using Index = uint32_t;

        Switch( Framework& framework );

        void set( Index index, Element* child );

        void activate( Index index );

        virtual void destroy() override;
        virtual Size fit( Size const& size ) override;
        virtual void update( float timeDiff ) override;
        virtual void render( ttb::State& state ) const override;
        virtual bool onEvent( Event const& event ) override;
        virtual void onChildChanged( Element& child ) override;

    private:
        Index m_activeIndex{ 0 };
        Element* m_activeChild{ nullptr };

        std::map< Index, Element* > m_children;
    };
}
