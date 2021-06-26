#pragma once

#include <ttb/ui/Element.hpp>


namespace ttb::ui
{
    class PriorityListener;
}


namespace ttb::ui
{
    class ScrollArea : public Element
    {
    public:
        enum class Direction
        {
            HORIZONTAL,
            VERTICAL,
        };

        ScrollArea( Framework& framework, Direction direction );

        ~ScrollArea();

        using Element::range;

        void child( Element* element );

        virtual void range( Range const& range ) override;
        virtual void destroy() override;
        virtual void render( ttb::State& state ) const override;
        virtual Range fit( Range const& space ) override;
        virtual void update( float timeDiff ) override;
        virtual bool onEvent( Event const& event ) override;
        virtual void onChildChanged( Element& child ) override;

    private:
        class ScrollableArea;

        std::unique_ptr< PriorityListener > m_prioListener;

        void offset( float value );

        Direction m_direction;
        float m_offset{ -0.25f };
        ttb::Vector< float, 2 > m_dragPos;

        std::unique_ptr< ScrollableArea > m_child;
        ttb::Matrix< float, 3, 3 > m_transform;
    };
}
