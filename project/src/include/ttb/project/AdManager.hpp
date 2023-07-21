#pragma once

#include <ttb/utils/co/Coroutine.hpp>


namespace ttb
{
    class AdManager
    {
    public:
        static AdManager& getInstance();

        ttb::co::Coroutine< void > init();

        ttb::co::Coroutine< bool > playAd( std::string const& id );

    private:
        AdManager() = default;
    };
}
