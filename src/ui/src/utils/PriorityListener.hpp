#pragma once

#include <ttb/utils/EventListener.hpp>

#include <cstdint>
#include <functional>
#include <vector>


namespace ttb::ui
{
    class Root;
}


namespace ttb::ui
{
    class PriorityListener : public EventListener
    {
    public:
        PriorityListener( Root& root, EventListener& listener );

        ~PriorityListener();

        virtual bool onEvent( Event const& event ) override;

    private:
        std::reference_wrapper< Root > m_root;
        std::reference_wrapper< EventListener > m_listener;

        EventListener* m_oldListener;
    };
}
