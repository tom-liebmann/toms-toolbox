#pragma once

#include <ttb/ui/Element.hpp>


namespace ttb::ui
{
    class Linear : public Element, public ElementParent
    {
    public:
        enum class Direction
        {
            HORIZONTAL,
            VERTICAL,
        };

        Linear( Root& root, Direction direction );

        Linear( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader );

        //! @copydoc Element::fitWidth( float )
        virtual float fitWidth( float space ) const override;

        //! @copydoc Element::fitHeight( float )
        virtual float fitHeight( float space ) const override;

        //! @copydoc Element::setSize( Size const& )
        virtual void setSize( Size const& value ) override;

        //! @copydoc Element::setPosition( Position const& )
        virtual void setPosition( Position const& value );

        //! @copydoc Element::update( float )
        virtual void update( float timeDiff ) override;

        //! @copydoc Element::render( ttb::State& )
        virtual void render( ttb::State& state ) const override;

        //! @copydoc Element::onEvent( Event const& )
        virtual bool onEvent( Event const& event ) override;

        //! @copydoc ElementParent::onChildChanged( Element& )
        virtual void onChildChanged( Element& child ) override;

        void remove( Element& child );

        void insert( std::size_t position, Element* child );

        void add( Element* child );

    private:
        Direction m_direction;

        struct Slot
        {
            float weight;
            float size;
            Element* child;
        };

        std::vector< Slot > m_slots;
    };
}
