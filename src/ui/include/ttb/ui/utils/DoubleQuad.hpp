#pragma once

#include <ttb/ui/Element.hpp>

#include <ttb/ui/utils/QuadRenderer.hpp>
#include <ttb/utils/Color.hpp>

#include <ttb/core/geometry.hpp>


namespace ttb::ui
{
    class DoubleQuad : public Element
    {
    public:
        DoubleQuad( Root& root,
                    ColorRgb const& outerColor,
                    float outerRadius,
                    ColorRgb const& innerColor,
                    float innerRadius,
                    float thickness );

        void outerColor( ColorRgb const& color );

        void innerColor( ColorRgb const& color );

        virtual Size fit( Size const& size ) override;

        virtual void render( ttb::State& state ) const override;

    private:
        float m_thickness;

        QuadRenderer m_quadRenderer;
    };
}
