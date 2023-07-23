#pragma once

#include <ttb/utils/co/Coroutine.hpp>


namespace ttb
{
    class SignInManager
    {
    public:
        static SignInManager& getInstance();

        ttb::co::Coroutine< std::optional< std::string > > performSilentSignIn();

        ttb::co::Coroutine< std::optional< std::string > > performExplicitSignIn();

    private:
        SignInManager() = default;
    };
}
