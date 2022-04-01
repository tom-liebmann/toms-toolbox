#pragma once


namespace ttb::resources
{
    class Manager;
}

namespace ttb::ui
{
    class Root;
}


namespace ttb::ui
{
    class Framework
    {
    public:
        virtual ~Framework();

        virtual ttb::resources::Manager& resourceManager() = 0;

        Root* getRoot();

        void setRoot( Root& value );

    private:
        Root* m_root{ nullptr };
    };
}
