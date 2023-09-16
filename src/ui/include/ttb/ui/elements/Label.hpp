#pragma once

#include <ttb/ui/Element.hpp>

#include <ttb/core/fonts/Font.hpp>
#include <ttb/core/fonts/TextLayout.hpp>
#include <ttb/core/geometry.hpp>
#include <ttb/core/shader.hpp>
#include <ttb/core/texture/Texture2D.hpp>
#include <ttb/ui/XmlLoader.hpp>
#include <ttb/utils/Color.hpp>


namespace ttb::ui
{
    class Label : public Element
    {
    public:
        Label( Root& root, std::string text, float size, ColorRgb const& color );

        Label( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader );

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

        std::unique_ptr< ttb::TextLayout > m_textLayout;

        ttb::Range< float, 2 > m_fontRange;
    };
}
