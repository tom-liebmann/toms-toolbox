#include <ttb/ui/Framework.hpp>


namespace ttb::ui
{
    Framework::~Framework() = default;

    Root* Framework::getRoot()
    {
        return m_root;
    }

    void Framework::setRoot( Root& value )
    {
        m_root = &value;
    }
}
