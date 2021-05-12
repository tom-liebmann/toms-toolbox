#pragma once

#include <iostream>
#include <sstream>
#include <string>


namespace ttb
{
    class Logger : public std::ostream
    {
    public:
        static Logger& instance( char loglevel );

    private:
        class Buffer : public std::stringbuf
        {
        public:
            Buffer( char loglevel );

            ~Buffer();

            virtual int sync() override;

        private:
            void flush();

            char m_loglevel;
        };

        explicit Logger( char loglevel );

        Buffer m_buffer;
    };


    Logger& logger( char loglevel );
}
