#pragma once

#include <ttb/ui/WrappedElement.hpp>


namespace ttb::ui
{
    class ScaleToFit : public WrappedElement
    {
    public:
        ScaleToFit( Framework& framework );

        void child( Element* element );

        /// @copydoc WrappedElement::fit( Range const& )
        virtual Range fit( Range const& space ) override;

        /// @copydoc WrappedElement::
        virtual void render( ttb::State& state ) const override;

    private:
        float m_factor{ 1.0f };
    };
}
