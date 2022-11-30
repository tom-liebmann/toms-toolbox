#pragma once

#include <ttb/core/Bindable.hpp>
#include <ttb/core/uniform.hpp>


namespace ttb
{
    class TextureSlice2D;
}


namespace ttb
{
    class TextureSlice2DBinder : public Bindable< TextureSlice2DBinder >
    {
    public:
        using Slot = std::uint8_t;

        TextureSlice2DBinder( TextureSlice2D const& slice,
                              std::string_view transformName,
                              Slot slot );

        TextureSlice2DBinder( TextureSlice2D const& slice, Slot slot );

    private:
        TextureSlice2D const& m_slice;
        std::string_view m_transformName;
        Slot m_slot;

        friend Bindable::Binder;
    };


    template <>
    class Bindable< TextureSlice2DBinder >::Binder
    {
    public:
        ~Binder();

    private:
        using Transform = ttb::Matrix< float, 3, 3 >;

        Binder( TextureSlice2DBinder const& parent, State::Data& data );

        TextureSlice2DBinder const& m_parent;
        State::Data& m_data;

        std::optional< UniformBinder< Transform > > m_transform;

        friend Bindable;
    };
}
