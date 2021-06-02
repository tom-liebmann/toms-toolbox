#pragma once

#include "Element.hpp"


namespace ttb::ui
{
    class ElementWrapper : public Element
    {
    public:
        ElementWrapper( Framework& framework );

        virtual ~ElementWrapper();

        using Element::range;

        virtual void range( Range const& range ) override;
        virtual void destroy() override;
        virtual Range fit( Range const& space ) override;
        virtual void update( float timeDiff ) override;
        virtual void render( ttb::State& state ) const override;
        virtual bool onEvent( Event& event ) override;

    protected:
        Element* wrappedChild();

        Element const* wrappedChild() const;

        void wrappedChild( std::unique_ptr< Element > element );

    private:
        std::unique_ptr< Element > m_child;

        ttb::Matrix< float, 3, 3 > m_transform;
    };
}
