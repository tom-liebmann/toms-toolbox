#pragma once

#include <memory>


namespace ttb
{
    class Window;
    class WindowMode;
}


namespace ttb
{
    class WindowCreator
    {
    public:
        static void init( std::string title, WindowMode const& mode );

        static std::shared_ptr< Window > const& instance();

    private:
        WindowCreator() = delete;
    };
}
