#pragma once

#include <ttb/utils/co/Coroutine.hpp>

#include <string>


namespace ttb
{
    class SignInManager
    {
    public:
        static SignInManager& getInstance();

        ttb::co::Coroutine< bool > performSilentSignIn();

        ttb::co::Coroutine< bool > performExplicitSignIn();

        ttb::co::Coroutine< std::optional< std::string > >
            generateServerToken( std::string const& serverId );

    private:
        SignInManager() = default;
    };
}
