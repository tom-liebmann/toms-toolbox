#include <ttb/project/AdManager.hpp>


namespace ttb
{
    AdManager& AdManager::getInstance()
    {
        static AdManager instance;
        return instance;
    }

    ttb::co::Coroutine< void > AdManager::init()
    {
        co_return;
    }

    ttb::co::Coroutine< bool > AdManager::playAd( std::string const& /* id */ )
    {
        co_return true;
    }
}
