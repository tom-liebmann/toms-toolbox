#pragma once

#include <ttb/ui/Element.hpp>


namespace ttb::ui
{
    class CombinedElement : public Element
    {
    public:
        CombinedElement( Framework& framework );

        //! @copydoc Element::destroy()
        virtual void destroy() override;

        //! @copydoc Element::range(Range const&)
        virtual void range( Range const& range ) override;

        //! @copydoc Element::fit(Range const&)
        virtual Range fit( Range const& space ) override;

        //! @copydoc Element::update(float)
        virtual void update( float timeDiff ) override;

        //! @copydoc Element::render(ttb::State&)
        virtual void render( ttb::State& state ) const override;

        //! @copydoc Element::onEvent(Event const&)
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

        ttb::Matrix< float, 3, 3 > m_transform;
    };
}
