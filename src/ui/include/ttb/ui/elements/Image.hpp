#pragma once

#include <ttb/core/texture.hpp>
#include <ttb/core/texture/ImageBinder.hpp>
#include <ttb/math/Matrix.hpp>
#include <ttb/ui/Element.hpp>


namespace ttb
{
    class Geometry;
    class Program;
}


namespace ttb::ui
{
    class Image : public Element
    {
    public:
        Image( Framework& framework );

        Image( Framework& framework, ImageBinder const& image );

        ~Image();

        void image( ImageBinder const& image );

        /// @copydoc Element::render( ttb::State& ) const
        virtual void render( ttb::State& state ) const override;

    private:
        std::shared_ptr< DynamicBindable > m_image;

        std::unique_ptr< ttb::Geometry > m_geometry;
        std::shared_ptr< ttb::Program > m_program;
    };
}
