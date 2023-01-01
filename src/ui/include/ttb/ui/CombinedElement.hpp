#pragma once

#include <ttb/ui/Element.hpp>

#include <vector>


namespace ttb::ui
{
    class CombinedElement : public Element, public ElementParent
    {
    public:
        CombinedElement( Root& root );

        //! @copydoc Element::fit( Size const& )
        virtual Size fit( Size const& size ) override;

        //! @copydoc Element::size( Size const& )
        virtual void size( Size const& value ) override;

        //! @copydoc Element::offset( Offset const& )
        virtual void offset( Offset const& value );

        //! @copydoc Element::update( float )
        virtual void update( float timeDiff ) override;

        //! @copydoc Element::render( ttb::State& )
        virtual void render( ttb::State& state ) const override;

        //! @copydoc Element::onEvent( Event const& )
        virtual bool onEvent( Event const& event ) override;

        virtual void onChildChanged( Element& child ) override;

    protected:
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
