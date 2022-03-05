#pragma once

#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/XmlLoader.hpp>


namespace ttb::ui
{
    class Margin : public WrappedElement
    {
    public:
        Margin( Framework& framework, float right, float top, float left, float bottom );

        Margin( Framework& framework, float hMargin, float vMargin );

        Margin( Framework& framework, float margin );

        Margin( Framework& framework, rapidxml::xml_node<> const& node, XmlLoader& loader );

        void child( Element* element );

        void right( float value, bool isLastChange = true );

        void top( float value, bool isLastChange = true );

        void left( float value, bool isLastChange = true );

        void bottom( float value, bool isLastChange = true );

        virtual Size fit( Size const& size ) override;

        virtual void render( ttb::State& state ) const override;

        virtual bool onEvent( ttb::Event const& event ) override;

    private:
        Position transform( Position const& pos ) const;

        Position transformInv( Position const& pos ) const;

        float m_right{ 0.0f };
        float m_top{ 0.0f };
        float m_left{ 0.0f };
        float m_bottom{ 0.0f };

        ttb::Matrix< float, 3, 3 > m_transform;
    };
}
