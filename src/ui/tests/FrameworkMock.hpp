#pragma once

#include <ttb/core/resources/Manager.hpp>
#include <ttb/ui/Framework.hpp>
#include <ttb/utils/EventManager.hpp>


namespace ttb::ui::test
{
    class FrameworkMock : public ttb::ui::Framework
    {
    public:
        virtual ttb::resources::Manager& resourceManager() override
        {
            return m_resourceManager;
        }

    private:
        ttb::resources::Manager m_resourceManager;
    };
}
