#pragma once

#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/XmlLoader.hpp>


namespace ttb::ui
{
    class Margin : public WrappedElement
    {
    public:
        Margin( Root& root, float right, float top, float left, float bottom );

        Margin( Root& root, float hMargin, float vMargin );

        Margin( Root& root, float margin );

        Margin( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader );

        void child( Element* element );

        void right( float value, bool isLastChange = true );

        void top( float value, bool isLastChange = true );

        void left( float value, bool isLastChange = true );

        void bottom( float value, bool isLastChange = true );

        virtual Size fit( Size const& size ) override;

        virtual void offset( Offset const& value ) override;

        using Element::offset;

        virtual void size( Size const& value ) override;

        using Element::size;

    private:
        float m_right{ 0.0f };
        float m_top{ 0.0f };
        float m_left{ 0.0f };
        float m_bottom{ 0.0f };
    };
}
