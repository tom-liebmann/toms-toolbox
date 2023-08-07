#pragma once

#include <ttb/core/State.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/ElementParent.hpp>
#include <ttb/ui/Extent.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventListener.hpp>

#include <rapidxml/rapidxml.hpp>


namespace ttb::ui
{
    class Root;
    class XmlLoader;
}


namespace ttb::ui
{
    class Element : public EventListener
    {
    public:
        using Size = ttb::Vector< float, 2 >;
        using Position = ttb::Vector< float, 2 >;
        using Transform = ttb::Matrix< float, 3, 3 >;

        Element( Root& root );

        Element( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader );

        virtual ~Element();

        virtual float fitWidth( float space ) const;

        virtual float fitHeight( float space ) const;

        Size fit( Size const& space ) const;

        virtual void setPosition( Position const& value );

        Position const& getPosition() const;

        virtual void setSize( Size const& value );

        Size const& getSize() const;

        void setRight( float value );
        void setTop( float value );
        void setLeft( float value );
        void setBottom( float value );

        float getRight() const;
        float getTop() const;
        float getLeft() const;
        float getBottom() const;

        void setWidth( Extent value );
        void setHeight( Extent value );

        Extent getWidth() const;
        Extent getHeight() const;

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
        Position m_position{};

        float m_right{ 0.0f };
        float m_top{ 0.0f };
        float m_left{ 0.0f };
        float m_bottom{ 0.0f };

        Extent m_width;
        Extent m_height;

        Root& m_root;
    };
}


namespace ttb::ui
{
    inline Root& Element::getRoot() const
    {
        return m_root;
    }

    inline auto Element::getPosition() const -> Position const&
    {
        return m_position;
    }

    inline auto Element::getSize() const -> Size const&
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

    inline void Element::setRight( float value )
    {
        m_right = value;
        changed();
    }

    inline void Element::setTop( float value )
    {
        m_top = value;
        changed();
    }

    inline void Element::setLeft( float value )
    {
        m_left = value;
        changed();
    }

    inline void Element::setBottom( float value )
    {
        m_bottom = value;
        changed();
    }

    inline float Element::getRight() const
    {
        return m_right;
    }

    inline float Element::getTop() const
    {
        return m_top;
    }

    inline float Element::getLeft() const
    {
        return m_left;
    }

    inline float Element::getBottom() const
    {
        return m_bottom;
    }

    inline void Element::setWidth( Extent value )
    {
        m_width = value;
        changed();
    }

    inline void Element::setHeight( Extent value )
    {
        m_height = value;
        changed();
    }

    inline Extent Element::getWidth() const
    {
        return m_width;
    }

    inline Extent Element::getHeight() const
    {
        return m_height;
    }
}
