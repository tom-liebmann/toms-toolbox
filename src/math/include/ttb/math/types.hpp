#pragma once

#include <ttb/math/Matrix.hpp>
#include <ttb/math/Range.hpp>
#include <ttb/math/Vector.hpp>


namespace ttb
{
    using Range2F = ttb::Range< float, 2 >;
    using Range2I = ttb::Range< int, 2 >;

    using Matrix3F = ttb::Matrix< float, 3, 3 >;
    using Mat3F = ttb::Matrix< float, 3, 3 >;
    using Mat4F = ttb::Matrix< float, 4, 4 >;

    using Vec2F = ttb::Vector< float, 2 >;
    using Vec3F = ttb::Vector< float, 3 >;
    using Vec2I = ttb::Vector< int, 2 >;
}
