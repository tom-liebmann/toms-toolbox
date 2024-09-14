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
        Rectangle( Root& root );

        Rectangle( Root& root, ColorRgb const& color, float opacity );

        virtual void parseXml( XmlNode const& node, XmlLoader& loader ) override;

        ~Rectangle();

        void setColor( ColorRgb const& value );

        void setOpacity( float value );

        ColorRgb const& getColor() const;

        float getOpacity() const;

        virtual void render( ttb::State& state ) const override;

    private:
        void initGeometry();

        ColorRgb m_color;
        float m_opacity;

        std::shared_ptr< ttb::Program const > m_program;
        std::unique_ptr< ttb::Geometry const > m_geometry;
    };
}
