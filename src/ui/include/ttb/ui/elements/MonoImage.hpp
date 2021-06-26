#pragma once

#include <ttb/ui/Element.hpp>

#include <ttb/math/Matrix.hpp>
#include <ttb/utils/Color.hpp>


namespace ttb
{
    class Texture2D;
    class Geometry;
    class Program;
}


namespace ttb::ui
{
    class MonoImage : public Element
    {
    public:
        MonoImage( Framework& framework,
                   std::shared_ptr< Texture2D const > image,
                   ColorRgb const& color );

        ~MonoImage();

        void image( std::shared_ptr< Texture2D const > image );

        void color( ColorRgb const& value );

        virtual void range( Range const& range ) override;
        virtual void render( ttb::State& state ) const override;

    private:
        std::shared_ptr< Texture2D const > m_image;
        ColorRgb m_color;

        ttb::Matrix< float, 3, 3 > m_transform;
        ttb::Matrix< float, 3, 3 > m_texTransform;

        std::unique_ptr< ttb::Geometry > m_geometry;
        std::shared_ptr< ttb::Program > m_program;
    };
}
