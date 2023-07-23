#pragma once

#include "ObjectBase.hpp"


namespace android
{
    class SignInManager : public ObjectBase
    {
    public:
        SignInManager( JavaVM* vm, jobject object );

        SignInManager( SignInManager const& ) = delete;

        SignInManager& operator=( SignInManager const& ) = delete;

        bool isSignedIn();

        void performSilentSignIn();

        void performExplicitSignIn();

    private:
        jobject m_object;

        jmethodID m_isSignedInMethod;
        jmethodID m_performSilentSignInMethod;
        jmethodID m_performExplicitSignInMethod;
    };
}
