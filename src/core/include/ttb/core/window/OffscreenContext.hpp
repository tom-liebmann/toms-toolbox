#pragma once

#include <ttb/core/Context.hpp>

#include <memory>


namespace ttb
{
    class OffscreenContext : public Context
    {
    public:
        OffscreenContext( Context& sharedContext );

        OffscreenContext();

        ~OffscreenContext();

        // Override: Context
        virtual bool use() override;
        virtual bool unuse() override;

    private:
        class Impl;

        std::unique_ptr< Impl > m_impl;
    };
}
