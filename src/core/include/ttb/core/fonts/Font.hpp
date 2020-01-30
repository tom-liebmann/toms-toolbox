#pragma once

#include <ttb/core/texture.hpp>
#include <ttb/math/Range.hpp>

#include <iostream>
#include <map>
#include <memory>


namespace ttb
{
    class Font
    {
    public:
        class Character
        {
        public:
            Character( std::istream& stream );

            char id() const;

            float x() const;

            float y() const;

            float width() const;

            float height() const;

            float xOffset() const;

            float yOffset() const;

            float xAdvance() const;

        private:
            char m_id;
            float m_x;
            float m_y;
            float m_width;
            float m_height;
            float m_xOffset;
            float m_yOffset;
            float m_xAdvance;
        };

        Font( std::string const& filename );

        Character const& character( char index ) const;

        float lineHeight() const;

        /**
         * Computes the dimensions of the geometry for the given text.
         *
         * @param size Size of the text to compute width for
         * @param text Text to compute width for
         * @return Dimensions of the given text
         */
        ttb::Range< float, 2 > textDimensions( float size, std::string const& text ) const;

        std::shared_ptr< ttb::Texture2D > const& texture() const;

    private:
        float m_lineHeight;

        std::shared_ptr< ttb::Texture2D > m_texture;
        std::map< char, Character > m_characters;
    };
}
