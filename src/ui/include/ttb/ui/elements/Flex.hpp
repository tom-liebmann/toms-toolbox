#pragma once

#include <ttb/ui/Element.hpp>


namespace ttb::ui
{
    class Flex : public Element
    {
    public:
        enum class SlotType
        {
            FIXED,
            FLEX,
            FIT,
        };

        enum class Direction
        {
            HORIZONTAL,
            VERTICAL,
        };

        Flex( Framework& framework, Direction direction );

        //! @copydoc Element::fit(Size const&)
        virtual Size fit( Size const& space ) override;

        //! @copydoc Element::update(float)
        virtual void update( float timeDiff ) override;

        //! @copydoc Element::render(ttb::State&)
        virtual void render( ttb::State& state ) const override;

        //! @copydoc Element::onEvent(Event const&)
        virtual bool onEvent( Event const& event ) override;

        //! @copydoc Element::onChildChanged(Element&)
        virtual void onChildChanged( Element& child ) override;

        virtual std::string info() const override;

        size_t addSlot( SlotType type, float value, bool isLastChange = true );

        void child( size_t slot, Element* element, bool isLastChange = true );

        void slotValue( size_t slot, float value, bool isLastChange = true );

    private:
        Position transform( size_t slot, Position const& pos ) const;

        Position transformInv( size_t slot, Position const& pos ) const;

        size_t dirDim() const;

        struct Slot
        {
            SlotType type;
            float value;
            float offset{ 0.0f };
            Element* child;
        };

        Direction m_direction;
        std::vector< Slot > m_slots;

        ttb::Matrix< float, 3, 3 > m_transform;
    };
}
