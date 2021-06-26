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
        Root( Framework& framework, Range const& range );

        ~Root();

        void child( Element* child );

        virtual void destroy() override;

        using Element::range;

        virtual void range( Range const& range ) override;

        virtual void update( float timeDiff ) override;

        virtual void render( ttb::State& state ) const override;

        virtual void onChildChanged( Element& child ) override;

        virtual bool onEvent( Event const& event ) override;

    private:
        Element* m_child{ nullptr };
    };
}
