#pragma once

#include "ObjectBase.hpp"

#include <string>


namespace android
{
    class SignInManager : public ObjectBase
    {
    public:
        SignInManager( JavaVM* vm, jobject object );

        SignInManager( SignInManager const& ) = delete;

        ~SignInManager();

        SignInManager& operator=( SignInManager const& ) = delete;

        bool isSignedIn();

        void performSilentSignIn();

        void performExplicitSignIn();

        void generateServerToken( std::string const& token );

    private:
        jobject m_object;

        jmethodID m_isSignedInMethod;
        jmethodID m_performSilentSignInMethod;
        jmethodID m_performExplicitSignInMethod;
        jmethodID m_generateServerTokenMethod;
    };
}
