#pragma once

#include <ttb/ui/Element.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb::ui
{
    class Flex : public Element, public UniqueCreate< Flex >
    {
    public:
        class Modifier;

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

        Modifier modify();

        //! @copydoc Element::range(Range const&)
        virtual void range( Range const& range ) override;

        //! @copydoc Element::fit(Range const&)
        virtual Range fit( Range const& space ) override;

        //! @copydoc Element::update(float)
        virtual void update( float timeDiff ) override;

        //! @copydoc Element::render(ttb::State&)
        virtual void render( ttb::State& state ) const override;

        //! @copydoc Element::onEvent(Event&)
        virtual bool onEvent( Event& event ) override;

        //! @copydoc Element::onChildChanged(Element&)
        virtual void onChildChanged( Element& child ) override;

        virtual Element* child( std::string const& id ) override;

        virtual Element const* child( std::string const& id ) const override;

    private:
        Flex( Framework& framework, Direction direction );

        size_t dirDim() const;

        struct Slot
        {
            SlotType type;
            float value;
            std::unique_ptr< Element > child;
        };

        Direction m_direction;
        std::vector< Slot > m_slots;

        ttb::Matrix< float, 3, 3 > m_transform;

        friend UniqueCreate;
    };


    class Flex::Modifier
    {
    public:
        ~Modifier();

        Modifier( Modifier&& rhs ) = default;
        Modifier& operator=( Modifier&& rhs ) = default;

        Modifier( Modifier const& rhs ) = delete;
        Modifier& operator=( Modifier const& rhs ) = delete;

        Modifier const& slot( SlotType type, float value ) const;

        Modifier const& child( size_t slot, std::unique_ptr< Element > element ) const;

        Modifier const& slotValue( size_t slot, float value ) const;

    private:
        Modifier( Flex& element );

        std::reference_wrapper< Flex > m_element;

        friend Flex;
    };
}
