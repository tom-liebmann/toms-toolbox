#pragma once

#include <ttb/ui/WrappedElement.hpp>

#include <ttb/ui/XmlLoader.hpp>


namespace ttb::ui
{
    class ScaleToFit : public WrappedElement
    {
    public:
        ScaleToFit( Root& root );

        ScaleToFit( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader );

        void child( Element* element );

        /// @copydoc WrappedElement::fit( Size const& )
        virtual Size fit( Size const& size ) override;

        /// @copydoc WrappedElement::
        virtual void render( ttb::State& state ) const override;

    private:
        float m_factor{ 1.0f };
    };
}
