#pragma once

#include <ttb/utils/EventListener.hpp>

#include <cstdint>
#include <functional>
#include <vector>


namespace ttb
{
    class EventManager;
}


namespace ttb::ui
{
    class PriorityListener : public EventListener
    {
    public:
        PriorityListener( EventManager& manager, EventListener& listener );

        ~PriorityListener();

        void addType( uint32_t type );

        virtual bool onEvent( Event& event ) override;

    private:
        std::reference_wrapper< EventManager > m_manager;
        std::reference_wrapper< EventListener > m_listener;

        std::vector< uint32_t > m_types;
    };
}
