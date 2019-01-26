#pragma once

#include <ttb/net/DataReader.hpp>


namespace ttb
{
    namespace events
    {
        template < typename TEventDefinition >
        class Data : public Event
        {
        public:
            Data( DataReader& reader );

            size_t available() const;

            size_t read( void* data, size_t size );

            // Override: Event
            virtual Type type() const override;

        private:
            DataReader& m_reader;
        };
    }
}


namespace ttb
{
    namespace events
    {
        template < typename TEventDefinition >
        inline Data< TEventDefinition >::Data( DataReader& reader ) : m_reader( reader )
        {
        }

        template < typename TEventDefinition >
        inline size_t Data< TEventDefinition >::available() const
        {
            return m_reader.available();
        }

        template < typename TEventDefinition >
        inline size_t Data< TEventDefinition >::read( void* data, size_t size )
        {
            return m_reader.read( data, size );
        }

        template < typename TEventDefinition >
        inline Event::Type Data< TEventDefinition >::type() const
        {
            return TEventDefinition::DATA;
        }
    }
}
