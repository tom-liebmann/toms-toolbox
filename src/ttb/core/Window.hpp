#pragma once

#define GLFW_INCLUDE_GLEXT
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ttb/core/EventManager.hpp>
#include <ttb/core/RenderTarget.hpp>
#include <ttb/core/WindowMode.hpp>

#include <memory>



// declarations
//=============================================================================

namespace ttb
{
    class Window : public RenderTarget
    {
    public:
        Window( std::string title, WindowMode mode );

        ~Window();

        void eventManager( std::shared_ptr< EventManager > const& eventManager );
        std::shared_ptr< EventManager > eventManager() const;

        void mode( WindowMode const& mode );
        WindowMode const& mode() const;

        GLFWwindow* handle() const;

        void update();

        // Override: RenderTarget
        virtual size_t width() const override;
        virtual size_t height() const override;
        virtual void begin( State& state ) const override;
        virtual void end( State& state ) const override;

    private:
        static uint8_t s_windowCount;

        std::string m_title;

        std::shared_ptr< EventManager > m_eventManager;
        WindowMode m_mode;
        GLFWwindow* m_handle;
    };
}
