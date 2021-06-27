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

        using Element::size;

        //! @copydoc Element::fit( Size const& )
        virtual Size fit( Size const& size ) override;

        //! @copydoc Element::update( float )
        virtual void update( float timeDiff ) override;

        //! @copydoc Element::render( ttb::State& )
        virtual void render( ttb::State& state ) const override;

        //! @copydoc Element::onEvent( Event const& )
        virtual bool onEvent( Event const& event ) override;

    protected:
        void wrappedChild( Element* child, Transform transform = {}, Transform transformInv = {} );

        Element* wrappedChild();

    private:
        Element* m_child{ nullptr };
    };
}
