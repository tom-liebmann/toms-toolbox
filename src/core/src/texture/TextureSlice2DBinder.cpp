#include <ttb/core/texture/TextureSlice2DBinder.hpp>

#include <ttb/core/texture/Texture2D.hpp>
#include <ttb/core/texture/TextureSlice2D.hpp>
#include <ttb/math/matrix_operations.hpp>


namespace ttb
{
    TextureSlice2DBinder::TextureSlice2DBinder( TextureSlice2D const& slice,
                                                std::string_view transformName,
                                                Slot slot )
        : m_slice{ slice }, m_transformName{ std::move( transformName ) }, m_slot{ slot }
    {
    }

    TextureSlice2DBinder::TextureSlice2DBinder( TextureSlice2D const& slice, Slot slot )
        : m_slice{ slice }, m_slot{ slot }
    {
    }


    TextureSlice2DBinder::Binder::~Binder()
    {
        m_parent.m_slice.getTexture()->unbind( m_parent.m_slot );
    }

    TextureSlice2DBinder::Binder::Binder( TextureSlice2DBinder const& parent, State::Data& data )
        : m_parent{ parent }, m_data{ data }
    {
        m_parent.m_slice.getTexture()->bind( m_parent.m_slot );

        if( !m_parent.m_transformName.empty() )
        {
            m_transform.emplace( ttb::UniformBinder< Transform >{
                m_parent.m_transformName,
                ttb::mat::transform( { { 0.0f, 0.0f }, { 1.0f, 1.0f } },
                                     m_parent.m_slice.getRange() ) }
                                     .bind( data ) );
        }
    }
}
