#pragma once

#include <ttb/net/DataReader.hpp>


namespace ttb
{
    namespace events
    {
        class Data : public Event
        {
        public:
            Data( DataReader& reader );

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
        inline Data::Data( DataReader& reader ) : m_reader( reader )
        {
        }

        inline size_t Data::read( void* data, size_t size )
        {
            return m_reader.read( data, size );
        }

        inline Event::Type Data::type() const
        {
            return DATA;
        }
    }
}
