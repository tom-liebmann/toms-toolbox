#pragma once


class AndroidManager
{
public:
    static AndroidManager& getInstance();

    void enableNetworkCheck();

    void disableNetworkCheck();

    bool isNetworkCheckEnabled();

    bool isNetworkAvailable();

private:
    AndroidManager();
};
