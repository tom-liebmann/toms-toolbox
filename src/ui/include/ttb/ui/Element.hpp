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

        Element* parent() const;

        void parent( Element& parent );

        Range const& range() const;

        /**
         * Sets the range of the element relative to the parent's position.
         *
         * @param range Range of the element within the parent space
         */
        virtual void range( Range const& range );

        virtual void destroy();

        virtual void render( ttb::State& state ) const = 0;

        /**
         * Returns the space the element takes within the provided area.
         *
         * Default behavior is that the element takes all the available space.
         *
         * @param space Space available to the element
         * @returns Space taken by the element
         */
        virtual Range fit( Range const& space );

        virtual void update( float timeDiff );

        virtual bool onEvent( Event const& event ) override;

        virtual void onChildChanged( Element& child );

        virtual Element* child( std::string const& id );

        virtual Element const* child( std::string const& id ) const;

        ttb::Vector< float, 2 > screenToParent( ttb::Vector< float, 2 > const& vec ) const;

        ttb::Vector< float, 2 > screenToLocal( ttb::Vector< float, 2 > const& vec ) const;

        ttb::Vector< float, 2 > localToScreen( ttb::Vector< float, 2 > const& vec ) const;

        template < typename... TId >
        Element* child( std::string const& id1, std::string const& id2, TId&&... ids );

        template < typename... TId >
        Element const* child( std::string const& id1, std::string const& id2, TId&&... ids ) const;

        template < typename TType, typename... TId >
        TType* castChild( TId&&... ids );

        template < typename TType, typename... TId >
        TType const* castChild( TId&&... ids ) const;

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
    template < typename... TId >
    Element* Element::child( std::string const& id1, std::string const& id2, TId&&... ids )
    {
        return child( id1 )->child( id2, std::forward< TId >( ids )... );
    }

    template < typename... TId >
    Element const*
        Element::child( std::string const& id1, std::string const& id2, TId&&... ids ) const
    {
        return child( id1 )->child( id2, std::forward< TId >( ids )... );
    }

    template < typename TType, typename... TId >
    TType* Element::castChild( TId&&... ids )
    {
        return dynamic_cast< TType* >( child( std::forward< TId >( ids )... ) );
    }

    template < typename TType, typename... TId >
    TType const* Element::castChild( TId&&... ids ) const
    {
        return dynamic_cast< TType const* >( child( std::forward< TId >( ids )... ) );
    }

    inline Framework& Element::framework() const
    {
        return m_framework;
    }
}
