#pragma once

#include <ttb/ui/Element.hpp>

#include <vector>


namespace ttb::ui
{
    class Group : public Element, public ElementParent
    {
    public:
        Group( Root& root );

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

        void remove( Element& child );

        void remove( std::size_t position );

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
