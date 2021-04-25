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
            Character( std::string const& line );

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

        Font( float lineHeight,
              uint16_t width,
              uint16_t height,
              std::string const& charFileContent );

        Character const& character( char index ) const;

        float lineHeight() const;

        uint16_t width() const;

        uint16_t height() const;

        /**
         * Computes the dimensions of the geometry for the given text.
         *
         * @param  size Size of the text to compute width for
         * @param  text Text to compute width for
         * @return      Dimensions of the given text
         */
        ttb::Range< float, 2 > textDimensions( float size, std::string const& text ) const;

    private:
        float m_lineHeight;
        uint16_t m_width;
        uint16_t m_height;
        std::map< char, Character > m_characters;
    };
}
