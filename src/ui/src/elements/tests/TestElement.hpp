#include <ttb/ui/Element.hpp>


namespace ttb
{
    class TestElement : public ttb::ui::Element
    {
    public:
        TestElement( ttb::ui::Root& root );

        virtual void render( ttb::State& state ) const override;
    };
}


namespace ttb
{
    inline TestElement::TestElement( ttb::ui::Root& root ) : Element{ root }
    {
    }

    inline void TestElement::render( ttb::State& state ) const
    {
    }
}
