#pragma once

#include <ttb/ui/Element.hpp>

#include <ttb/math/Range.hpp>
#include <ttb/utils/EventListener.hpp>

#include <memory>


namespace ttb::ui
{
    class Framework;
}


namespace ttb::ui
{
    class Root : public Element
    {
    public:
        static std::unique_ptr< Root > create( Range const& range, Framework& framework );

        ~Root();

        void child( std::unique_ptr< Element > child );

        using Element::range;

        virtual void range( Range const& range ) override;

        virtual void update( float timeDiff ) override;

        virtual void render( ttb::State& state ) const override;

        virtual void onChildChanged( Element& child ) override;

        virtual Element* child( std::string const& id ) override;

        virtual Element const* child( std::string const& id ) const override;

    private:
        Root( Framework& framework, Range const& range );

        // Override: EventListener
        virtual bool onEvent( Event& event ) override;

        std::unique_ptr< Element > m_child;
    };
}
