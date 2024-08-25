#pragma once

#include <ttb/core/State.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/ElementParent.hpp>
#include <ttb/ui/Extent.hpp>
#include <ttb/ui/FitExtent.hpp>
#include <ttb/ui/Margin.hpp>
#include <ttb/ui/xml_loading.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventListener.hpp>


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

        virtual ~Element();

        virtual FitExtent fitWidth( Size const& space ) const;

        virtual FitExtent fitHeight( Size const& space ) const;

        Size finalFit( Size const& space ) const;

        virtual void setPosition( Position const& value );

        Position const& getPosition() const;

        virtual void setSize( Size const& value );

        Size const& getSize() const;

        void setMargin( Margin value );

        Margin getMargin() const;

        void setWidth( Extent value );
        void setHeight( Extent value );

        Extent getWidth() const;
        Extent getHeight() const;

        virtual void render( ttb::State& state ) const = 0;

        virtual void update( float timeDiff );

        virtual bool onEvent( Event const& event ) override;

        ElementParent* getParent() const;

        void setParent( ElementParent* parent );

        Transform getTransform() const;

        virtual void parseXml( XmlNode const& node, XmlLoader& loader );

    protected:
        void changed();

        Root& getRoot() const;

    private:
        Root& m_root;

        ElementParent* m_parent{ nullptr };

        Size m_size{};
        Position m_position{};

        Margin m_margin;

        Extent m_width{ Extent::Type::MATCH_PARENT };
        Extent m_height{ Extent::Type::MATCH_PARENT };
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

    inline void Element::setMargin( Margin value )
    {
        m_margin = value;
        changed();
    }

    inline Margin Element::getMargin() const
    {
        return m_margin;
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
