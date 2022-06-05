#pragma once

#include "ApplicationProperties.hpp"


namespace ttb
{
    class ApplicationRunner;
}


namespace ttb
{
    class Application
    {
    public:
        virtual ~Application();

        void stop();

        bool stopped() const;

    private:
        virtual ApplicationProperties getProperties() const = 0;

        virtual void init() = 0;

        virtual void destroy() = 0;

        virtual void update( float frameTime ) = 0;

        virtual void draw() const = 0;

        bool m_stopped{ false };

        friend ApplicationRunner;
    };
}


/**
 * Main entry point into the application derivate that
 * implements application-specific behavior.
 * This function is not implemented within ttb but has
 * to be provided through an external source.
 *
 * @return Reference to application object
 */
ttb::Application& application();
