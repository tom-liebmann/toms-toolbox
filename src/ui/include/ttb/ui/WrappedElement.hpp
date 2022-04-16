#pragma once

#include <ttb/ui/Element.hpp>


namespace ttb::ui
{
    class WrappedElement : public Element
    {
    public:
        WrappedElement( Framework& framework );

        //! @copydoc Element::destroy()
        virtual void destroy() override;

        //! @copydoc Element::fit( Size const& )
        virtual Size fit( Size const& size ) override;

        //! @copydoc Element::offset( Offset const& )
        virtual void offset( Offset const& value ) override;

        using Element::offset;

        //! @copydoc Element::size( Size const& )
        virtual void size( Size const& value ) override;

        using Element::size;

        //! @copydoc Element::update( float )
        virtual void update( float timeDiff ) override;

        //! @copydoc Element::render( ttb::State& )
        virtual void render( ttb::State& state ) const override;

        //! @copydoc Element::onEvent( Event const& )
        virtual bool onEvent( Event const& event ) override;

    protected:
        void wrappedChild( Element* child );

        Element* wrappedChild();

        Element const* wrappedChild() const;

    private:
        Element* m_child{ nullptr };
    };
}
