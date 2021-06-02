#pragma once


namespace ttb::resources
{
    class Manager;
}

namespace ttb
{
    class EventManager;
}


namespace ttb::ui
{
    class Framework
    {
    public:
        virtual ~Framework();

        virtual ttb::resources::Manager& resourceManager() = 0;

        virtual ttb::EventManager& eventManager() = 0;
    };
}
