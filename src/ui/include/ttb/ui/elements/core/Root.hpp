#pragma once

#include <ttb/ui/Element.hpp>

#include <ttb/math/Range.hpp>
#include <ttb/utils/EventListener.hpp>
#include <ttb/utils/UniqueCreate.hpp>

#include <memory>


namespace ttb::ui
{
    class Framework;
}


namespace ttb::ui
{
    class Root : public Element, public UniqueCreate< Root >
    {
    public:
        ~Root();

        void child( std::unique_ptr< Element > child );

        using Element::range;

        virtual void range( Range const& range ) override;

        virtual void update( float timeDiff ) override;

        virtual void render( ttb::State& state ) const override;

        virtual void onChildChanged( Element& child ) override;

        virtual Element* child( std::string const& id ) override;

        virtual Element const* child( std::string const& id ) const override;

        virtual bool onEvent( Event const& event ) override;

    private:
        Root( Framework& framework, Range const& range );

        std::unique_ptr< Element > m_child;

        friend UniqueCreate;
    };
}
