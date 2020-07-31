#pragma once


namespace ttb
{
    class Context
    {
    public:
        class ContextHandle;

        virtual ~Context();

        ContextHandle makeCurrent();

        virtual bool use() = 0;

        virtual bool unuse() = 0;
    };


    class Context::ContextHandle
    {
    public:
        ContextHandle( bool valid );

        ContextHandle( ContextHandle const& copy ) = delete;
        ContextHandle( ContextHandle&& copy ) = delete;

        ContextHandle& operator=( ContextHandle const& copy ) = delete;
        ContextHandle& operator=( ContextHandle&& copy ) = delete;

        ~ContextHandle();

        operator bool() const;

    private:
        bool m_valid;
    };
}
