#pragma once

#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/XmlLoader.hpp>
#include <ttb/utils/gesture/events.hpp>


namespace ttb::ui
{
    class PriorityListener;
}


namespace ttb::ui
{
    class ScrollArea : public WrappedElement
    {
    public:
        enum class Direction
        {
            HORIZONTAL,
            VERTICAL,
        };

        ScrollArea( Root& root, Direction direction );

        ScrollArea( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader );

        ~ScrollArea();

        void setChild( Element* element );

        void setScrollOffset( float value );

        virtual Size fit( Size const& space ) override;
        virtual void offset( Offset const& value ) override;
        virtual void size( Size const& value ) override;
        virtual bool onEvent( Event const& event ) override;
        virtual void update( float timeDiff ) override;
        virtual void render( ttb::State& state ) const override;

        using Element::offset;
        using Element::size;

    private:
        bool onPointerPressStart( ttb::events::PointerPressStart const& event );
        bool onDragStart( ttb::events::DragStart const& event );
        bool onDragEnd( ttb::events::DragEnd const& event );
        bool onDragMove( ttb::events::DragMove const& event );

        std::unique_ptr< PriorityListener > m_prioListener;

        Direction m_direction;
        float m_offset{ 0.0f };
        ttb::Vector< float, 2 > m_dragPos;

        float m_overScroll{ 0.1f };
        float m_overScrollFactor{ 0.1f };
    };
}
