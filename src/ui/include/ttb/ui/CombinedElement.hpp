#pragma once

#include <ttb/ui/Element.hpp>

#include <vector>


namespace ttb::ui
{
    class CombinedElement : public Element
    {
    public:
        CombinedElement( Framework& framework );

        //! @copydoc Element::destroy()
        virtual void destroy() override;

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

    protected:
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
