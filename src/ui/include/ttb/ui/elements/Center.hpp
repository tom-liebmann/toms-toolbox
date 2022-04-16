#pragma once

#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/XmlLoader.hpp>


namespace ttb::ui
{
    class Center : public WrappedElement
    {
    public:
        enum class HAlignment
        {
            LEFT,
            CENTER,
            RIGHT,
        };

        enum class VAlignment
        {
            TOP,
            MIDDLE,
            BOTTOM,
        };

        Center( Framework& framework, HAlignment hAlign, VAlignment vAlign );

        Center( Framework& framework, rapidxml::xml_node<> const& node, XmlLoader& loader );

        void child( Element* element );

        //! @copydoc WrappedElement::fit( Size const& )
        virtual Size fit( Size const& size ) override;

        virtual void offset( Offset const& value ) override;

        using Element::offset;

        virtual void size( Size const& value ) override;

        using Element::size;

        virtual void render( ttb::State& state ) const override;

        virtual std::string info() const override;

    private:
        HAlignment m_hAlign;
        VAlignment m_vAlign;

        Offset m_childOffset{};
    };
}
