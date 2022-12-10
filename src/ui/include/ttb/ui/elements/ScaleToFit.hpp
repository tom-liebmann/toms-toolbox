#pragma once

#include <ttb/ui/WrappedElement.hpp>


namespace ttb::ui
{
    class ScaleToFit : public WrappedElement
    {
    public:
        ScaleToFit( Root& root );

        void child( Element* element );

        /// @copydoc WrappedElement::fit( Size const& )
        virtual Size fit( Size const& size ) override;

        /// @copydoc WrappedElement::
        virtual void render( ttb::State& state ) const override;

    private:
        float m_factor{ 1.0f };
    };
}
