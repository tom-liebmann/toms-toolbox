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

        auto getEntity() -> Entity&;

    private:
        Entity& m_entity;
    };
}


namespace ttb
{
    inline auto Component::getEntity() -> Entity&
    {
        return m_entity;
    }
}
