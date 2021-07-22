#pragma once

#include <ttb/ui/Element.hpp>


namespace ttb::ui
{
    class RowLayout : public Element
    {
    public:
        RowLayout( Framework& framework, uint16_t numCols );

        void child( uint16_t index, Element* element );

        void erase( uint16_t index );

        uint16_t findChild( Element const& element );

        uint16_t size() const;

        virtual Size fit( Size const& size ) override;
        virtual void update( float timeDiff ) override;
        virtual void render( ttb::State& state ) const override;
        virtual bool onEvent( Event const& event ) override;

    private:
        struct Child
        {
            uint16_t index;
            Element* element;

            bool operator<( Child const& rhs ) const;
        };

        uint16_t m_numCols;
        std::vector< Child > m_children;
    };
}
