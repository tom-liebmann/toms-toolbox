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
        using Size = ttb::Vector< float, 2 >;
        using Position = ttb::Vector< float, 2 >;
        using Transform = std::function< Position( Position const& ) >;

        Element( Framework& framework );

        virtual ~Element();

        virtual void destroy();

        Size const& size() const;

        virtual Size fit( Size const& space );

        virtual void render( ttb::State& state ) const = 0;

        virtual void update( float timeDiff );

        virtual bool onEvent( Event const& event ) override;

        virtual std::string info() const;

        Element* parent() const;

        void parent( Element* parent, Transform transform = {}, Transform transformInv = {} );

        virtual void onChildChanged( Element& child );

        ttb::Vector< float, 2 > localToScreen( ttb::Vector< float, 2 > const& vec ) const;

        ttb::Vector< float, 2 > screenToLocal( ttb::Vector< float, 2 > const& vec ) const;

    protected:
        void changed();

        Framework& framework() const;

    private:
        Element* m_parent{ nullptr };

        Size m_size{ 0.0f, 0.0f };

        /// Transforms local coordinates to parent coordinates
        Transform m_transform;

        /// Transforms parent coordinates to local coordinates
        Transform m_transformInv;

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
