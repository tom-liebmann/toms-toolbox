#pragma once

#include <ttb/core/window/WindowRequest.hpp>


namespace ttb
{
    class ApplicationProperties
    {
    public:
        ApplicationProperties( bool limitFps,
                               ttb::WindowRequest::Size windowSize,
                               ttb::WindowFlag windowFlags );

        bool getLimitFps() const;

        ttb::WindowRequest const& getWindowRequest() const;

    private:
        bool m_limitFps;
        ttb::WindowRequest m_windowRequest;
    };
}


namespace ttb
{
    inline ApplicationProperties::ApplicationProperties( bool limitFps,
                                                         ttb::WindowRequest::Size windowSize,
                                                         ttb::WindowFlag windowFlags )
        : m_limitFps{ limitFps }, m_windowRequest{ windowSize, windowFlags }
    {
    }

    inline bool ApplicationProperties::getLimitFps() const
    {
        return m_limitFps;
    }

    inline ttb::WindowRequest const& ApplicationProperties::getWindowRequest() const
    {
        return m_windowRequest;
    }
}
