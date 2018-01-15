#pragma once

#include <ttb/core/fonts/Font.hpp>
#include <ttb/core/geometry.hpp>

#include <memory>


namespace ttb
{
    class TextFactory
    {
    public:
        TextFactory() = delete;
        TextFactory( TextFactory const& copy ) = delete;
        TextFactory( TextFactory&& copy ) = delete;

        static std::unique_ptr< ttb::Geometry > createText( Font const& font,
                                                            std::string const& text );
    };
}
