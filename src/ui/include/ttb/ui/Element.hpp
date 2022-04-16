#pragma once

#include <ttb/core/State.hpp>
#include <ttb/math/matrix_operations.hpp>
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
        using Offset = ttb::Vector< float, 2 >;
        using Transform = ttb::Matrix< float, 3, 3 >;

        Element( Framework& framework );

        virtual ~Element();

        virtual void destroy();

        virtual Size fit( Size const& space );

        virtual void offset( Offset const& value );

        Offset const& offset() const;

        virtual void size( Size const& value );

        Size const& size() const;

        virtual void render( ttb::State& state ) const = 0;

        virtual void update( float timeDiff );

        virtual bool onEvent( Event const& event ) override;

        virtual std::string info() const;

        Element* parent() const;

        void parent( Element* parent );

        virtual void onChildChanged( Element& child );

        Transform transform() const;

    protected:
        void changed();

        Framework& framework() const;

    private:
        Element* m_parent{ nullptr };

        Size m_size{};
        Offset m_offset{};

        Framework& m_framework;
    };
}


namespace ttb::ui
{
    inline Framework& Element::framework() const
    {
        return m_framework;
    }

    inline auto Element::offset() const -> Offset const&
    {
        return m_offset;
    }

    inline auto Element::size() const -> Size const&
    {
        return m_size;
    }
}
