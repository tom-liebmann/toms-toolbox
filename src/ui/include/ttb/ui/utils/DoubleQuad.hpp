#pragma once

#include <ttb/ui/Element.hpp>

#include <ttb/ui/utils/QuadRenderer.hpp>
#include <ttb/utils/Color.hpp>
#include <ttb/utils/UniqueCreate.hpp>

#include <ttb/core/geometry.hpp>


namespace ttb::ui
{
    class DoubleQuad : public Element, public UniqueCreate< DoubleQuad >
    {
    public:
        void outerColor( ColorRgb const& color );

        void innerColor( ColorRgb const& color );

        virtual void range( Range const& range ) override;
        virtual void render( ttb::State& state ) const override;

    private:
        DoubleQuad( Framework& framework,
                    ColorRgb const& outerColor,
                    float outerRadius,
                    ColorRgb const& innerColor,
                    float innerRadius,
                    float thickness );

        float m_thickness;

        QuadRenderer m_quadRenderer;
    };
}
