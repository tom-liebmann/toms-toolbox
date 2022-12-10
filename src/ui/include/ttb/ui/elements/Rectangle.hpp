#pragma once

#include <ttb/ui/Element.hpp>
#include <ttb/ui/XmlLoader.hpp>
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
        Rectangle( Root& root, ColorRgb const& color );

        Rectangle( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader );

        ~Rectangle();

        void color( ColorRgb const& value );

        ColorRgb const& color() const;

        virtual void render( ttb::State& state ) const override;

    private:
        void initGeometry();

        ColorRgb m_color;

        std::shared_ptr< ttb::Program const > m_program;
        std::unique_ptr< ttb::Geometry const > m_geometry;
    };
}
