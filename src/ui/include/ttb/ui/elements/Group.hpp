#pragma once

#include <ttb/ui/Element.hpp>

#include <vector>


namespace ttb::ui
{
    class Group : public Element, public ElementParent
    {
    public:
        Group( Root& root );

        Group( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader );

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

        virtual void onChildChanged( Element& child ) override;

        void remove( Element& child );

        void insert( std::size_t position, Element* child, bool considerSize = true );

        void add( Element* child, bool considerSize = true );

    private:
        struct Slot
        {
            bool considerSize;
            Element* element;
        };

        std::vector< Slot > m_children;
    };
}
