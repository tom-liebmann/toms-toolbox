#pragma once

#include <ttb/utils/EventListener.hpp>

#include <cstdint>
#include <functional>
#include <vector>


namespace ttb::ui
{
    class Framework;
}


namespace ttb::ui
{
    class PriorityListener : public EventListener
    {
    public:
        PriorityListener( Framework& framework, EventListener& listener );

        ~PriorityListener();

        virtual bool onEvent( Event const& event ) override;

    private:
        std::reference_wrapper< Framework > m_framework;
        std::reference_wrapper< EventListener > m_listener;

        EventListener* m_oldListener;
    };
}
