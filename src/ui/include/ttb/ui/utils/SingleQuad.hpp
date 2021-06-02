#pragma once

#include <ttb/ui/Element.hpp>

#include <ttb/ui/utils/QuadRenderer.hpp>
#include <ttb/utils/Color.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb::ui
{
    class SingleQuad : public Element, public UniqueCreate< SingleQuad >
    {
    public:
        void color( ColorRgb const& value );

        virtual void range( Range const& range ) override;
        virtual void render( ttb::State& state ) const override;

    private:
        SingleQuad( Framework& framework, ColorRgb const& color, float radius );

        QuadRenderer m_quadRenderer;

        friend UniqueCreate;
    };
}
