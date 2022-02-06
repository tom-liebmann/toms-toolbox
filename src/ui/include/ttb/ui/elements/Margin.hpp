#pragma once

#include <ttb/ui/WrappedElement.hpp>


namespace ttb::ui
{
    class Margin : public WrappedElement
    {
    public:
        Margin( Framework& framework, float right, float top, float left, float bottom );

        Margin( Framework& framework, float hMargin, float vMargin );

        Margin( Framework& framework, float margin );

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

        float m_right;
        float m_top;
        float m_left;
        float m_bottom;

        ttb::Matrix< float, 3, 3 > m_transform;
    };
}
