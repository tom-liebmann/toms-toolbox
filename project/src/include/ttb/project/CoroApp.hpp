#pragma once

#include "ApplicationProperties.hpp"
#include <ttb/project/Application.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/co/Coroutine.hpp>

namespace ttb
{
    class CoroApp : public Application
    {
    protected:
        auto nextFrame() const -> co::Coroutine< float >;

    private:
        virtual auto getProperties() const -> ApplicationProperties override = 0;
        virtual auto run( int argc, char** argv ) -> co::Coroutine< void > = 0;
        virtual void draw() const override = 0;

        virtual void init( int argc, char** argv ) override;
        virtual void destroy() override;
        virtual void update( float frameTime ) override;

        void onEvent( ttb::Event const& event );

        float m_frameTime;
        co::Coroutine< void > m_runCoro;
    };


    class CoroAppClosed
    {
    };
}
