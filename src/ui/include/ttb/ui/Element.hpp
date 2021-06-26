#pragma once

#include <ttb/core/State.hpp>
#include <ttb/math/Range.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventListener.hpp>


namespace ttb::ui
{
    class Framework;
}


namespace ttb::ui
{
    class Element : public EventListener
    {
    public:
        using Range = ttb::Range< float, 2 >;
        using Size = ttb::Vector< float, 2 >;
        using Offset = ttb::Vector< float, 2 >;

        Element( Framework& framework );

        virtual ~Element();

        virtual void destroy();

        Range const& range() const;

        /**
         * Sets the range of the element relative to the parent's position.
         *
         * @param range Range of the element within the parent space
         */
        virtual void range( Range const& range );

        /**
         * Returns the space the element takes within the provided area.
         *
         * Default behavior is that the element takes all the available space.
         *
         * @param space Space available to the element
         * @returns Space taken by the element
         */
        virtual Range fit( Range const& space );

        virtual void render( ttb::State& state ) const = 0;

        virtual void update( float timeDiff );

        virtual bool onEvent( Event const& event ) override;

        Element* parent() const;

        void parent( Element* parent );

        virtual void onChildChanged( Element& child );

        ttb::Vector< float, 2 > screenToParent( ttb::Vector< float, 2 > const& vec ) const;

        ttb::Vector< float, 2 > screenToLocal( ttb::Vector< float, 2 > const& vec ) const;

        ttb::Vector< float, 2 > localToScreen( ttb::Vector< float, 2 > const& vec ) const;

    protected:
        void changed();

        Framework& framework() const;

    private:
        Element* m_parent{ nullptr };

        //! The range of the element relative to the parents root range.
        Range m_range{ { 0.0f, 0.0f }, { 1.0f, 1.0f } };

        Framework& m_framework;
    };
}


namespace ttb::ui
{
    inline Framework& Element::framework() const
    {
        return m_framework;
    }
}
