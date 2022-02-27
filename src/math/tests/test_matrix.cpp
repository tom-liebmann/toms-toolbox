#include <catch2/catch.hpp>

#include <ttb/math/Matrix.hpp>
#include <ttb/math/matrix_operations.hpp>


namespace
{
    template < typename TType, std::size_t TRows, std::size_t TCols >
    class MatchesMatrix
    {
    public:
        template < typename TType2 >
        using Matrix = ttb::Matrix< TType2, TRows, TCols >;

        MatchesMatrix( Matrix< TType > const& matrix, TType tolerance = TType{ 0 } )
            : m_matrix{ matrix }
        {
        }

        bool match( Matrix< TType > const& matrix ) const
        {
            using std::abs;

            for( std::size_t r = 0; r < TRows; ++r )
            {
                for( std::size_t c = 0; c < TCols; ++c )
                {
                    if( abs( m_matrix( r, c ) - matrix( r, c ) ) > m_tolerance )
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        std::string toString() const
        {
            return "Matches Matrix";
        }

    private:
        Matrix< TType > const& m_matrix;
        TType m_tolerance;
    };
}


TEST_CASE( "Matrix", "[math][matrix]" )
{
    SECTION( "Inversion" )
    {
        SECTION( "Matrix 1x1" )
        {
            auto const m = ttb::Matrix< float, 1, 1 >{ 5.0f };

            REQUIRE_THAT( ttb::invert( m ),
                          MatchesMatrix( ttb::Matrix< float, 1, 1 >{ 1.0f / 5.0f } ) );

            REQUIRE_THAT( ttb::invert( m ) * m, MatchesMatrix( ttb::mat::identity< float, 1 >() ) );
        }

        SECTION( "Matrix 2x2" )
        {
            auto const m = ttb::Matrix< float, 2, 2 >{ 1.0f, 2.0f, 3.0f, 4.0f };

            REQUIRE_THAT( ttb::invert( m ) * m, MatchesMatrix( ttb::mat::identity< float, 2 >() ) );
        }

        SECTION( "Matrix 3x3" )
        {
            auto const m =
                ttb::Matrix< float, 3, 3 >{ 1.0f, 2.0f, 3.0f, 0.0f, 1.0f, 4.0f, 5.0f, 6.0f, 0.0f };

            REQUIRE_THAT( ttb::invert( m ),
                          MatchesMatrix( ttb::Matrix< float, 3, 3 >{
                              -24.0f, 18.0f, 5.0f, 20.0f, -15.0f, -4.0f, -5.0f, 4.0f, 1.0f } ) );
            REQUIRE_THAT( ttb::invert( m ) * m, MatchesMatrix( ttb::mat::identity< float, 3 >() ) );
        }
    }
}
