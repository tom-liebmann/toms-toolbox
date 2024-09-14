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
    class Text : public Element
    {
    public:
        Text( Root& root );

        Text( Root& root, std::string text, float size, ColorRgb const& color );

        virtual void parseXml( XmlNode const& node, XmlLoader& loader ) override;

        void setText( std::string value );

        void setSize( float value );

        void setColor( ColorRgb const& color );

        virtual FitExtent fitWidth( Size const& space ) const override;

        virtual FitExtent fitHeight( Size const& space ) const override;

        virtual void setSize( Size const& value ) override;

        virtual void render( ttb::State& state ) const override;

    private:
        void updateGeometry();

        void updateMaxWidth( float value ) const;

        std::string m_text;
        float m_size;
        ColorRgb m_color;

        std::shared_ptr< ttb::Program > m_shader;
        std::shared_ptr< ttb::Font > m_font;
        std::unique_ptr< ttb::Geometry > m_textGeometry;
        std::shared_ptr< ttb::Texture2D > m_texture;

        std::unique_ptr< ttb::TextLayout > m_textLayout;

        mutable ttb::Range< float, 2 > m_fontRange;
    };
}
