#pragma once

#include <ttb/ui/Element.hpp>
#include <ttb/utils/Color.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb
{
    class Geometry;
    class Program;
}


namespace ttb::ui
{
    class Rectangle : public Element, public UniqueCreate< Rectangle >
    {
    public:
        ~Rectangle();

        virtual void render( ttb::State& state ) const override;

    private:
        Rectangle( Framework& framework, ColorRgb const& color );

        ColorRgb m_color;

        std::shared_ptr< ttb::Program const > m_program;
        std::unique_ptr< ttb::Geometry const > m_geometry;

        friend UniqueCreate;
    };
}
