#pragma once


namespace ttb
{
    class Entity;
}


namespace ttb
{
    class Component
    {
    public:
        Component( Entity& entity );

        virtual ~Component();

        virtual void init();

        virtual void destroy();

    private:
        Entity& m_entity;
    };
}
