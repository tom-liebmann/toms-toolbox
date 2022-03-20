#pragma once

#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/Element.hpp>
#include <ttb/ui/XmlLoader.hpp>

#include <vector>


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
            FIT_INFINITY,
        };

        enum class Direction
        {
            HORIZONTAL,
            VERTICAL,
        };

        Flex( Framework& framework, Direction direction );

        Flex( Framework& framework, rapidxml::xml_node<> const& node, XmlLoader& loader );

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

        size_t addSlot( SlotType type, float value, Element* child, bool isLastChange = true );

        void child( size_t slot, Element* element, bool isLastChange = true );

        void slotValue( size_t slot, float value, bool isLastChange = true );

    private:
        size_t dirDim() const;

        struct Slot
        {
            SlotType type;
            float value;
            float offset{ 0.0f };
            float width{ 0.0f };
            Element* child;
        };

        Direction m_direction;
        std::vector< Slot > m_slots;

        ttb::Matrix< float, 3, 3 > m_transform{ ttb::mat::identity< float, 3 >() };
    };
}
