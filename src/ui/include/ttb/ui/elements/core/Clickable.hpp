#pragma once

#include <ttb/ui/ElementWrapper.hpp>
#include <ttb/utils/EventListener.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb::ui
{
    class PriorityListener;
}


namespace ttb::ui
{
    class Clickable : public ElementWrapper, public UniqueCreate< Clickable >
    {
    public:
        enum class Action
        {
            START,
            ABORT,
            END
        };

        using ClickCallback = std::function< void( Action, Element& ) >;

        ~Clickable();

        void callback( ClickCallback value );

        void child( std::unique_ptr< Element > element );

        using Element::range;

    private:
        Clickable( Framework& framework );

        /// @copydoc EventListenere::onEvent( Event& event )
        virtual bool onEvent( Event& event ) override;

        ClickCallback m_callback;

        std::unique_ptr< PriorityListener > m_prioListener;

        friend UniqueCreate;
    };
}
