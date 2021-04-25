#include <ttb/core/Viewport.hpp>


namespace ttb
{
    std::ostream& operator<<( std::ostream& stream, Viewport const& viewport )
    {
        return stream << "V[ " << viewport.getX() << ", " << viewport.getY() << ", "
                      << viewport.getWidth() << ", " << viewport.getHeight() << " ]";
    }
}
