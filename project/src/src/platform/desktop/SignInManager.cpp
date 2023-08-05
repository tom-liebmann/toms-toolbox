#include <ttb/project/SignInManager.hpp>


namespace ttb
{
    SignInManager& SignInManager::getInstance()
    {
        static SignInManager instance;
        return instance;
    }

    ttb::co::Coroutine< bool > SignInManager::performSilentSignIn()
    {
        co_return true;
    }

    ttb::co::Coroutine< bool > SignInManager::performExplicitSignIn()
    {
        co_return true;
    }

    ttb::co::Coroutine< std::optional< std::string > >
        SignInManager::generateServerToken( std::string const& serverId )
    {
        // TODO: Return fixed debugging string
        co_return "12345";
    }
}
