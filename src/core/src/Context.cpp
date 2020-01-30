#include <ttb/core/Context.hpp>

#include <map>
#include <thread>


namespace
{
    std::map< std::thread::id, ttb::Context* > s_currentContexts;
}


namespace ttb
{
    Context::ContextHandle Context::makeCurrent()
    {
        auto threadId = std::this_thread::get_id();

        // release previous context
        if( auto iter = s_currentContexts.find( threadId ); iter != std::end( s_currentContexts ) )
        {
            return { false };
        }

        use();

        s_currentContexts[ threadId ] = this;

        return { true };
    }

    Context::~Context() = default;


    Context::ContextHandle::ContextHandle( bool valid ) : m_valid{ valid }
    {
    }

    Context::ContextHandle::~ContextHandle()
    {
        auto threadId = std::this_thread::get_id();

        if( auto iter = s_currentContexts.find( threadId ); iter != std::end( s_currentContexts ) )
        {
            iter->second->unuse();
            s_currentContexts.erase( iter );
        }
    }

    Context::ContextHandle::operator bool() const
    {
        return m_valid;
    }
}
