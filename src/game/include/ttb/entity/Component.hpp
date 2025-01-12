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

        auto getEntity() -> Entity&;

        void update( float frameTime );

        void draw( ttb::State& state ) const;

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
