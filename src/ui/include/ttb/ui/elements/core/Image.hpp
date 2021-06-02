#pragma once

#include <ttb/ui/Element.hpp>

#include <ttb/core/texture.hpp>
#include <ttb/math/Matrix.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb
{
    class Geometry;
    class Program;
}


namespace ttb::ui
{
    class Image : public Element, public UniqueCreate< Image >
    {
    public:
        ~Image();

        void image( std::shared_ptr< Texture2D const > image );

        virtual void range( Range const& range ) override;
        virtual void render( ttb::State& state ) const override;

    private:
        Image( Framework& framework, std::shared_ptr< Texture2D const > image );

        void init();

        void updateGeometry();

        std::shared_ptr< Texture2D const > m_image;

        ttb::Matrix< float, 3, 3 > m_transform;
        ttb::Matrix< float, 3, 3 > m_texTransform;

        std::unique_ptr< ttb::Geometry > m_geometry;
        std::shared_ptr< ttb::Program > m_program;
    };
}
