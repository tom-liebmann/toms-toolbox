#pragma once

#include <ttb/ui/Element.hpp>
#include <ttb/utils/Color.hpp>


namespace ttb
{
    class Geometry;
    class Program;
}


namespace ttb::ui
{
    class Rectangle : public Element
    {
    public:
        Rectangle( Framework& framework, ColorRgb const& color );

        ~Rectangle();

        virtual void render( ttb::State& state ) const override;

    private:
        ColorRgb m_color;

        std::shared_ptr< ttb::Program const > m_program;
        std::unique_ptr< ttb::Geometry const > m_geometry;
    };
}
