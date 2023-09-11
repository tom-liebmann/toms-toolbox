#pragma once

#include <ttb/core/fonts/Font.hpp>
#include <ttb/core/geometry.hpp>

#include <memory>


namespace ttb
{
    class TextLayout;
}


namespace ttb
{
    class TextFactory
    {
    public:
        TextFactory() = delete;

        /**
         * Computes the dimensions of the geometry for the given text.
         *
         * @param  font   Font to use.
         * @param  size   Size of the text to compute width for.
         * @param  text   Text to compute width for.
         * @param  layout Layout to be used for positioning the glyphs.
         * @return        Dimensions of the given text
         */
        static ttb::Range< float, 2 > getDimensions( Font const& font,
                                                     float size,
                                                     std::string_view text,
                                                     TextLayout const& layout );

        static std::unique_ptr< ttb::Geometry > createText( Font const& font,
                                                            float size,
                                                            std::string_view text,
                                                            TextLayout const& layout );
    };
}
