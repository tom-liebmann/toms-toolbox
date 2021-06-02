#pragma once

#include <ttb/ui/Element.hpp>
#include <ttb/utils/UniqueCreate.hpp>

#include <map>


namespace ttb::ui
{
    class Switch : public Element, public UniqueCreate< Switch >
    {
    public:
        using Index = uint32_t;

        std::unique_ptr< Element > release( Index index );

        void set( Index index, std::unique_ptr< Element > child );

        void activate( Index index );

        virtual void destroy() override;
        using Element::range;
        virtual void range( Range const& range ) override;
        virtual Range fit( Range const& space ) override;
        virtual void update( float timeDiff ) override;
        virtual void render( ttb::State& state ) const override;
        virtual bool onEvent( Event const& event ) override;
        virtual void onChildChanged( Element& child ) override;
        virtual Element* child( std::string const& id ) override;
        virtual Element const* child( std::string const& id ) const override;

    private:
        Switch( Framework& framework );

        Index m_activeIndex{ 0 };
        Element* m_activeChild{ nullptr };

        std::map< Index, std::unique_ptr< Element > > m_children;

        friend UniqueCreate;
    };
}
