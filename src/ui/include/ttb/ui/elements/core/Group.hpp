#pragma once

#include <ttb/ui/Element.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb::ui
{
    class Group : public Element, public UniqueCreate< Group >
    {
    public:
        void addChild( std::unique_ptr< Element > child, bool considerSize = true );

        //! @copydoc Element::range(Range const&)
        virtual void range( Range const& range ) override;

        //! @copydoc Element::destroy()
        virtual void destroy() override;

        //! @copydoc Element::fit(Range const&)
        virtual Range fit( Range const& space ) override;

        //! @copydoc Element::update(float)
        virtual void update( float timeDiff ) override;

        //! @copydoc Element::render(ttb::State&)
        virtual void render( ttb::State& state ) const override;

        //! @copydoc Element::onEvent(Event&)
        virtual bool onEvent( Event& event ) override;

        virtual Element* child( std::string const& id ) override;

        virtual Element const* child( std::string const& id ) const override;

    private:
        Group( Framework& framework );

        struct Slot
        {
            bool considerSize;
            std::unique_ptr< Element > element;
        };

        std::vector< Slot > m_children;

        ttb::Matrix< float, 3, 3 > m_transform;
    };
}
