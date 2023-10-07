#pragma once

#include <ttb/ui/Element.hpp>


namespace ttb::ui
{
    class WrappedElement : public Element, public ElementParent
    {
    public:
        WrappedElement( Root& root );

        //! @copydoc Element::fitWidth( Size )
        virtual FitExtent fitWidth( Size const& space ) const override;

        //! @copydoc Element::fitHeight( Size )
        virtual FitExtent fitHeight( Size const& space ) const override;

        //! @copydoc Element::setPosition( Position const& )
        virtual void setPosition( Position const& value ) override;

        //! @copydoc Element::setSize( Size const& )
        virtual void setSize( Size const& value ) override;

        //! @copydoc Element::update( float )
        virtual void update( float timeDiff ) override;

        //! @copydoc Element::render( ttb::State& )
        virtual void render( ttb::State& state ) const override;

        //! @copydoc Element::onEvent( Event const& )
        virtual bool onEvent( Event const& event ) override;

        virtual void onChildChanged( Element& child ) override;

    protected:
        void wrappedChild( Element* child );

        Element* wrappedChild();

        Element const* wrappedChild() const;

    private:
        Element* m_child{ nullptr };
    };
}
