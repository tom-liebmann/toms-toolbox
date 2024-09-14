#pragma once

#include <ttb/ui/elements/Slot.hpp>
#include <ttb/ui/XmlLoader.hpp>
#include <ttb/utils/gesture/events.hpp>


namespace ttb::ui
{
    class PriorityListener;
}


namespace ttb::ui
{
    class ScrollArea : public Slot
    {
    public:
        enum class Direction
        {
            HORIZONTAL,
            VERTICAL,
        };

        ScrollArea( Root& root );

        ScrollArea( Root& root, Direction direction );

        ~ScrollArea();

        void setScrollOffset( float value );

        virtual FitExtent fitWidth( Size const& space ) const override;
        virtual FitExtent fitHeight( Size const& space ) const override;
        virtual void setPosition( Position const& value ) override;
        virtual void setSize( Size const& value ) override;

        virtual bool onEvent( Event const& event ) override;
        virtual void update( float timeDiff ) override;
        virtual void render( ttb::State& state ) const override;

    private:
        bool onPointerPressStart( ttb::events::PointerPressStart const& event );
        bool onDragStart( ttb::events::DragStart const& event );
        bool onDragEnd( ttb::events::DragEnd const& event );
        bool onDragMove( ttb::events::DragMove const& event );

        std::unique_ptr< PriorityListener > m_prioListener;

        Direction m_direction;
        float m_offset{ 0.0f };
        ttb::Vector< float, 2 > m_dragPos;

        float m_velocity{ 0.0f };

        float m_overScroll{ 0.15f };
        float m_overScrollFactor{ 0.02f };
    };
}
