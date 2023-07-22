#pragma once

#include <string>


class AndroidManager
{
public:
    static AndroidManager& getInstance();

    void enableNetworkCheck();

    void disableNetworkCheck();

    bool isNetworkCheckEnabled();

    bool isNetworkAvailable();

    bool isAdManagerInitialized();

    void initializeAdManager();

    void runRewardedAd( std::string const& id );

private:
    AndroidManager();
};
