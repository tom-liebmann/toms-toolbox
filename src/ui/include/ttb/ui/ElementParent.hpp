#pragma once


namespace ttb::ui
{
    class Element;
}


namespace ttb::ui
{
    class ElementParent
    {
    public:
        virtual ~ElementParent();

        virtual void onChildChanged( Element& child ) = 0;
    };
}
