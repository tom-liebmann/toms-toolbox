#pragma once

#include <ttb/utils/co/Coroutine.hpp>


namespace ttb
{
    class SignInManager
    {
    public:
        static SignInManager& getInstance();

        co::Coroutine< std::optional< std::string > > performSilentSignIn();

        co::Coroutine< std::optional< std::string > > performExplicitSignIn();

    private:
        SignInManager() = default;
    };
}
