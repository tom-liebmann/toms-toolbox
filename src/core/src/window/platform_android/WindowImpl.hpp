#pragma once

#include <ttb/core/window/Window.hpp>


namespace ttb
{
    namespace priv
    {
        class WindowImpl : public Window
        {
        public:
            static void init( std::string_view title, WindowRequest const& request );

            static std::unique_ptr< WindowImpl >& instance();

            static void onResize( uint16_t width, uint16_t height );
            static void onPointerDown( int pointerId, float x, float y );
            static void onPointerUp( int pointerId, float x, float y );
            static void onPointerMove( int poitnerId, float x, float y );

            virtual void pollEvents() override;
            virtual void begin( State& state ) const override;
            virtual void end( State& state ) const override;
            virtual bool use() override;
            virtual bool unuse() override;

        private:
            WindowImpl( std::string_view title, Size const& size, WindowFlag flags );
        };
    }
}
