#pragma once

#include <ttb/ui/WrappedElement.hpp>


namespace ttb::ui
{
    class Margin : public WrappedElement
    {
    public:
        Margin( Framework& framework,
                float right = 0.0f,
                float top = 0.0f,
                float left = 0.0f,
                float bottom = 0.0f );

        void child( Element* element );

        void right( float value, bool isLastChange = true );

        void top( float value, bool isLastChange = true );

        void left( float value, bool isLastChange = true );

        void bottom( float value, bool isLastChange = true );

        virtual Size fit( Size const& size ) override;

    private:
        Position transform( Position const& pos ) const;

        Position transformInv( Position const& pos ) const;

        float m_right;
        float m_top;
        float m_left;
        float m_bottom;
    };
}
