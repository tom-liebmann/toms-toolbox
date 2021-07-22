#pragma once

#include <ttb/ui/Element.hpp>

#include <ttb/core/texture.hpp>
#include <ttb/math/Matrix.hpp>


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

        Image( Framework& framework, std::shared_ptr< Texture2D const > image );

        ~Image();

        void image( std::shared_ptr< Texture2D const > image );

        /// @copydoc Element::fit( Size const& )
        virtual Size fit( Size const& size ) override;

        /// @copydoc Element::render( ttb::State& ) const
        virtual void render( ttb::State& state ) const override;

    private:
        std::shared_ptr< Texture2D const > m_image;

        ttb::Matrix< float, 3, 3 > m_transform;
        ttb::Matrix< float, 3, 3 > m_texTransform;

        std::unique_ptr< ttb::Geometry > m_geometry;
        std::shared_ptr< ttb::Program > m_program;
    };
}
