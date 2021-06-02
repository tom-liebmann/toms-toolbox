#pragma once

#include <ttb/ui/Element.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb::ui
{
    /**
     * Wrapper element that takes two children (first, second) and acts
     * like the first one if it's set and uses the second one as a
     * fallback.
     */
    class Priority : public Element, public UniqueCreate< Priority >
    {
    public:
        using Element::range;

        std::unique_ptr< Element > releaseFirstChild();

        void firstChild( std::unique_ptr< Element > child );

        void secondChild( std::unique_ptr< Element > child );

        virtual void destroy() override;
        virtual void range( Range const& range ) override;
        virtual Range fit( Range const& space ) override;
        virtual void update( float timeDiff ) override;
        virtual void render( ttb::State& state ) const override;
        virtual bool onEvent( Event& event ) override;
        virtual void onChildChanged( Element& child ) override;
        virtual Element* child( std::string const& id ) override;
        virtual Element const* child( std::string const& id ) const override;

    private:
        Priority( Framework& framework );

        std::unique_ptr< Element > m_newFirstChild;
        bool m_firstChildChanged{ false };

        std::unique_ptr< Element > m_newSecondChild;
        bool m_secondChildChanged{ false };

        std::unique_ptr< Element > m_firstChild;
        std::unique_ptr< Element > m_secondChild;
    };
}
