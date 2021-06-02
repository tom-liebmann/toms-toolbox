#pragma once

#include <ttb/ui/Element.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb::ui
{
    class RowLayout : public Element, public UniqueCreate< RowLayout >
    {
    public:
        void child( uint16_t index, std::unique_ptr< Element > element );

        void erase( uint16_t index );

        uint16_t findChild( Element const& element );

        uint16_t size() const;

        virtual void range( Range const& range ) override;
        virtual Range fit( Range const& space ) override;
        virtual void update( float timeDiff ) override;
        virtual void render( ttb::State& state ) const override;
        virtual bool onEvent( Event const& event ) override;

        // TODO
        // virtual Element* child( std::string const& id ) override;
        // virtual Element const* child( std::string const& id ) const override;

    private:
        RowLayout( Framework& framework, uint16_t numCols );

        struct Child
        {
            uint16_t index;
            std::unique_ptr< Element > element;

            bool operator<( Child const& rhs ) const;
        };

        uint16_t m_numCols;
        std::vector< Child > m_children;
    };
}
