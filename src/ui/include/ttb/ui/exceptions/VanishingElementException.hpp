#pragma once

#include <exception>


namespace ttb::ui
{
    class VanishingElementException : public std::exception
    {
    public:
        char const* what() const noexcept;
    };
}


namespace ttb::ui
{
    inline char const* VanishingElementException::what() const noexcept
    {
        return "Element has size zero because of a MATCH_PARENT inside MATCH_CHILD size hint";
    }
}
