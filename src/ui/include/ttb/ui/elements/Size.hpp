#pragma once

#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/XmlLoader.hpp>
#include <ttb/ui/elements/Height.hpp>
#include <ttb/ui/elements/Width.hpp>


namespace ttb::ui
{
    class Size : public WrappedElement
    {
    public:
        Size( Root& root, float width, float height );

        Size( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader );

        void child( Element* element );

        virtual Element::Size fit( Element::Size const& size ) override;

    private:
        float m_width;
        float m_height;
    };
}
