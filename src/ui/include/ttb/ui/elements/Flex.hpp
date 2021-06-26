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

        //! @copydoc Element::range(Range const&)
        virtual void range( Range const& range ) override;

        //! @copydoc Element::fit(Range const&)
        virtual Range fit( Range const& space ) override;

        //! @copydoc Element::update(float)
        virtual void update( float timeDiff ) override;

        //! @copydoc Element::render(ttb::State&)
        virtual void render( ttb::State& state ) const override;

        //! @copydoc Element::onEvent(Event const&)
        virtual bool onEvent( Event const& event ) override;

        //! @copydoc Element::onChildChanged(Element&)
        virtual void onChildChanged( Element& child ) override;

        size_t addSlot( SlotType type, float value, bool isLastChange = true );

        void child( size_t slot, Element* element, bool isLastChange = true );

        void slotValue( size_t slot, float value, bool isLastChange = true );

    private:
        size_t dirDim() const;

        struct Slot
        {
            SlotType type;
            float value;
            Element* child;
        };

        Direction m_direction;
        std::vector< Slot > m_slots;

        ttb::Matrix< float, 3, 3 > m_transform;
    };
}
