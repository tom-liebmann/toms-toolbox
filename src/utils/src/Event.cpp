#include <ttb/utils/Event.hpp>

#include <ttb/math/matrix_operations.hpp>


namespace ttb
{
    void Event::transform( Transform const& /* transform */, EventCallback const& callback ) const
    {
        callback( *this );
    }

    void Event::transform( ttb::Matrix< float, 3, 3 > const& transform,
                           EventCallback const& callback ) const
    {
        this->transform(
            [ &transform ]( auto const& v )
            {
                return transform * v;
            },
            callback );
    }
}
