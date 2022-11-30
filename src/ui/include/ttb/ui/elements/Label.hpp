#pragma once

#include <ttb/ui/Element.hpp>

#include <ttb/core/fonts/Font.hpp>
#include <ttb/core/geometry.hpp>
#include <ttb/core/shader.hpp>
#include <ttb/ui/XmlLoader.hpp>
#include <ttb/utils/Color.hpp>


namespace ttb::ui
{
    class Label : public Element
    {
    public:
        Label( Framework& framework, std::string text, float size, ColorRgb const& color );

        Label( Framework& framework, rapidxml::xml_node<> const& node, XmlLoader& loader );

        void text( std::string value );

        void setSize( float value );

        void setColor( ColorRgb const& color );

        virtual Size fit( Size const& size ) override;

        virtual void render( ttb::State& state ) const override;

    private:
        void updateGeometry();

        std::string m_text;
        float m_size;
        ColorRgb m_color;

        std::shared_ptr< ttb::Program > m_shader;
        std::shared_ptr< ttb::Font > m_font;
        std::unique_ptr< ttb::Geometry > m_textGeometry;
        std::shared_ptr< ttb::Texture2D > m_texture;
    };
}
