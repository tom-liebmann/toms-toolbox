#pragma once

#include <ttb/ui/Element.hpp>

#include <ttb/ui/utils/QuadRenderer.hpp>
#include <ttb/utils/Color.hpp>


namespace ttb::ui
{
    class SingleQuad : public Element
    {
    public:
        SingleQuad( Framework& framework, ColorRgb const& color, float radius );

        void color( ColorRgb const& value );

        virtual void offset( Offset const& value ) override;

        virtual void render( ttb::State& state ) const override;

    private:
        QuadRenderer m_quadRenderer;
    };
}
