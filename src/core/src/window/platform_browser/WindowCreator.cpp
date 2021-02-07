#include <ttb/core/window/WindowCreator.hpp>

#include "WindowImpl.hpp"

#include <string>


namespace ttb
{
    namespace
    {
        std::shared_ptr< ttb::Window >& instance();
    }
}


namespace ttb
{
    void WindowCreator::init( std::string title, WindowMode const& mode )
    {
        ttb::instance().reset( new Window{
            std::unique_ptr< Window::Impl >{ new Window::Impl{ std::move( title ), mode } } } );
    }

    std::shared_ptr< Window > const& WindowCreator::instance()
    {
        return ttb::instance();
    }
}


namespace ttb
{
    namespace
    {
        std::shared_ptr< ttb::Window >& instance()
        {
            static std::shared_ptr< ttb::Window > windowPtr;
            return windowPtr;
        }
    }
}
