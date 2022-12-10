#pragma once

#include <ttb/core/State.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/ElementParent.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventListener.hpp>


namespace ttb::ui
{
    class Root;
}


namespace ttb::ui
{
    class Element : public EventListener
    {
    public:
        using Size = ttb::Vector< float, 2 >;
        using Offset = ttb::Vector< float, 2 >;
        using Transform = ttb::Matrix< float, 3, 3 >;

        Element( Root& root );

        virtual ~Element();

        virtual Size fit( Size const& space );

        virtual void offset( Offset const& value );

        Offset const& offset() const;

        virtual void size( Size const& value );

        Size const& size() const;

        virtual void render( ttb::State& state ) const = 0;

        virtual void update( float timeDiff );

        virtual bool onEvent( Event const& event ) override;

        virtual std::string info() const;

        ElementParent* getParent() const;

        void setParent( ElementParent* parent );

        Transform transform() const;

    protected:
        void changed();

        Root& getRoot() const;

    private:
        ElementParent* m_parent{ nullptr };

        Size m_size{};
        Offset m_offset{};

        Root& m_root;
    };
}


namespace ttb::ui
{
    inline Root& Element::getRoot() const
    {
        return m_root;
    }

    inline auto Element::offset() const -> Offset const&
    {
        return m_offset;
    }

    inline auto Element::size() const -> Size const&
    {
        return m_size;
    }

    inline ElementParent* Element::getParent() const
    {
        return m_parent;
    }

    inline void Element::setParent( ElementParent* parent )
    {
        m_parent = parent;
    }
}
