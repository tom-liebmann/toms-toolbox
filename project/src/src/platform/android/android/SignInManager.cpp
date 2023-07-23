

namespace android
{
    SignInManager::SignInManager( JavaVM* vm, jobject object )
        : ObjectBase{ vm }, m_object{ object }
    {
        auto& env = getEnv();

        auto const cls = findClass( env, "toms_toolbox/SignInManager" );
        m_isSignedInMethod = findMethod( env, cls, "isSignedInMethod", "()Z" );
        m_performSilentSignInMethod = findMethod( env, cls, "performSilentSignIn", "()V" );
        m_performExplicitSignInMethod = findMethod( env, cls, "performExplicitSignIn", "()V" );
    }

    SignInManager::~SignInManager()
    {
        auto& env = getEnv();

        env->DeleteGlobalRef( m_object );
    }

    bool isSignedIn()
    {
        auto& env = getEnv();
        return env->CallBooleanMethod( m_object, m_isSignedInMethod );
    }

    void performSilentSignIn()
    {
        auto& env = getEnv();
        env->CallVoidMethod( m_object, m_performSilentSignInMethod );
    }

    void performExplicitSignIn()
    {
        auto& env = getEnv();
        env->CallVoidMethod( m_object, m_performExplicitSignInMethod );
    }
}
