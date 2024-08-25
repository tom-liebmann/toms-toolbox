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

        Linear( Root& root );

        Linear( Root& root, Direction direction );

        virtual void parseXml( XmlNode const& node, XmlLoader& loader ) override;

        //! @copydoc Element::fitWidth( Size const& )
        virtual FitExtent fitWidth( Size const& space ) const override;

        //! @copydoc Element::fitHeight( Size const& )
        virtual FitExtent fitHeight( Size const& space ) const override;

        //! @copydoc Element::setSize( Size const& )
        virtual void setSize( Size const& value ) override;

        //! @copydoc Element::setPosition( Position const& )
        virtual void setPosition( Position const& value ) override;

        //! @copydoc Element::update( float )
        virtual void update( float timeDiff ) override;

        //! @copydoc Element::render( ttb::State& )
        virtual void render( ttb::State& state ) const override;

        //! @copydoc Element::onEvent( Event const& )
        virtual bool onEvent( Event const& event ) override;

        //! @copydoc ElementParent::onChildChanged( Element& )
        virtual void onChildChanged( Element& child ) override;

        Element& getChild( std::size_t position );

        void remove( Element& child );

        void insert( std::size_t position, Element* child, float weight = 1.0f );

        void add( Element* child, float weight = 1.0f );

    private:
        Direction m_direction;

        struct Slot
        {
            bool isFixed;
            float weight;
            float size;
            Element* child;
        };

        std::vector< Slot > m_slots;
    };
}
