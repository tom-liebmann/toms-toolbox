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

        using Element::range;

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
        void wrappedChild( Element* child );

        Element* wrappedChild();

    private:
        Element* m_child{ nullptr };

        ttb::Matrix< float, 3, 3 > m_transform;
    };
}
