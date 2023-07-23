#pragma once

#include <string>


namespace android
{
    class SignInManager;
}


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

    android::SignInManager& getSignInManager();

private:
    AndroidManager();
};
