#include <ttb/core/texture/ImageBinder.hpp>

#include <ttb/core/uniform.hpp>
#include <ttb/math/matrix_operations.hpp>


namespace ttb
{
    namespace
    {
        class LocalTextureBinder : public DynamicBindable::Binder
        {
        public:
            LocalTextureBinder( std::shared_ptr< Texture2D > texture, std::uint8_t slot );

            virtual ~LocalTextureBinder();

        private:
            std::shared_ptr< Texture2D > m_texture;
            std::uint8_t m_slot;
        };


        class LocalTextureBindable : public DynamicBindable
        {
        public:
            LocalTextureBindable( std::shared_ptr< Texture2D > texture, std::uint8_t slot );

            virtual std::unique_ptr< Binder > bind( State::Data& data ) const override;

        private:
            std::shared_ptr< Texture2D > m_texture;
            std::uint8_t m_slot;
        };


        class LocalTextureSliceBinder : public DynamicBindable::Binder
        {
        public:
            LocalTextureSliceBinder( std::shared_ptr< Texture2D > texture,
                                     std::string transformName,
                                     ttb::Range< float, 2 > const& range,
                                     std::uint8_t slot,
                                     State::Data& data );

            virtual ~LocalTextureSliceBinder();

        private:
            std::shared_ptr< Texture2D > m_texture;
            std::uint8_t m_slot;
            ttb::UniformBinderHandle< ttb::Matrix< float, 3, 3 > > m_transformBinderHandle;
        };


        class LocalTextureSliceBindable : public DynamicBindable
        {
        public:
            LocalTextureSliceBindable( std::shared_ptr< Texture2D > texture,
                                       std::string transformName,
                                       ttb::Range< float, 2 > const& range,
                                       std::uint8_t slot );

            virtual std::unique_ptr< Binder > bind( State::Data& data ) const override;

        private:
            std::shared_ptr< Texture2D > m_texture;
            std::string m_transformName;
            ttb::Range< float, 2 > m_range;
            std::uint8_t m_slot;
        };
    }
}


namespace ttb
{
    ImageBinder::~ImageBinder() = default;

    TextureBinder::TextureBinder( std::shared_ptr< Texture2D > texture )
        : m_texture{ std::move( texture ) }
    {
    }

    std::shared_ptr< DynamicBindable > TextureBinder::getBindable( std::uint8_t slot ) const
    {
        return std::make_unique< LocalTextureBindable >( m_texture, slot );
    }


    TextureSliceBinder::TextureSliceBinder( std::shared_ptr< Texture2D > texture,
                                            std::string transformName,
                                            ttb::Range< float, 2 > const& range )
        : m_texture{ std::move( texture ) }
        , m_transformName{ std::move( transformName ) }
        , m_range{ range }
    {
    }

    std::shared_ptr< DynamicBindable > TextureSliceBinder::getBindable( std::uint8_t slot ) const
    {
        return std::make_unique< LocalTextureSliceBindable >(
            m_texture, m_transformName, m_range, slot );
    }
}


namespace ttb
{
    namespace
    {
        LocalTextureBindable::LocalTextureBindable( std::shared_ptr< Texture2D > texture,
                                                    std::uint8_t slot )
            : m_texture{ std::move( texture ) }, m_slot{ slot }
        {
        }

        auto LocalTextureBindable::bind( State::Data& /* data */ ) const
            -> std::unique_ptr< Binder >
        {
            return std::make_unique< LocalTextureBinder >( m_texture, m_slot );
        }


        LocalTextureBinder::LocalTextureBinder( std::shared_ptr< Texture2D > texture,
                                                std::uint8_t slot )
            : m_texture{ std::move( texture ) }, m_slot{ slot }
        {
            m_texture->bind( m_slot );
        }

        LocalTextureBinder::~LocalTextureBinder()
        {
            m_texture->unbind( m_slot );
        }


        LocalTextureSliceBindable::LocalTextureSliceBindable( std::shared_ptr< Texture2D > texture,
                                                              std::string transformName,
                                                              ttb::Range< float, 2 > const& range,
                                                              std::uint8_t slot )
            : m_texture{ std::move( texture ) }
            , m_transformName{ std::move( transformName ) }
            , m_range{ range }
            , m_slot{ slot }
        {
        }

        auto LocalTextureSliceBindable::bind( State::Data& data ) const -> std::unique_ptr< Binder >
        {
            return std::make_unique< LocalTextureSliceBinder >(
                m_texture, m_transformName, m_range, m_slot, data );
        }


        LocalTextureSliceBinder::LocalTextureSliceBinder( std::shared_ptr< Texture2D > texture,
                                                          std::string transformName,
                                                          ttb::Range< float, 2 > const& range,
                                                          std::uint8_t slot,
                                                          State::Data& data )
            : m_texture{ std::move( texture ) }
            , m_slot{ slot }
            , m_transformBinderHandle{
                UniformBinder(
                    transformName,
                    ttb::mat::transform( ttb::Range< float, 2 >{ { 0.0f, 0.0f }, { 1.0f, 1.0f } },
                                         range ) )
                    .bind( data )
            }
        {
            m_texture->bind( m_slot );
        }

        LocalTextureSliceBinder::~LocalTextureSliceBinder()
        {
            m_texture->unbind( m_slot );
        }
    }
}
