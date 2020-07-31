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
        static std::unique_ptr< Window > create( std::string const& title, WindowMode const& mode );

        static std::unique_ptr< Window >
            create( std::string const& title, WindowMode const& mode, Window const& sharedWindow );

    private:
        WindowCreator() = delete;
    };
}
