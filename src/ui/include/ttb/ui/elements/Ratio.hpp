#pragma once

#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/XmlLoader.hpp>


namespace ttb::ui
{
    class Ratio : public WrappedElement
    {
    public:
        Ratio( Framework& framework, float ratio = 1.0f );

        Ratio( Framework& framework, rapidxml::xml_node<> const& node, XmlLoader& loader );

        void child( Element* element );

        void ratio( float ratio );

        virtual Size fit( Size const& size ) override;

        virtual void size( Size const& value ) override;

        using Element::size;

    private:
        float m_ratio{ 1.0f };
    };
}
