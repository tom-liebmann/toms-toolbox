#pragma once

#include <ttb/ui/WrappedElement.hpp>


namespace ttb::ui
{
    class ScaleToFit : public WrappedElement
    {
    public:
        ScaleToFit( Framework& framework );

        void child( Element* element );

        /// @copydoc WrappedElement::fit( Size const& )
        virtual Size fit( Size const& size ) override;

        /// @copydoc WrappedElement::
        virtual void render( ttb::State& state ) const override;

    private:
        Position transform( Position const& pos ) const;

        Position transformInv( Position const& pos ) const;

        float m_factor{ 1.0f };
    };
}
