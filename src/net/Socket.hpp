#pragma once

#include <string>

#ifndef WIN32
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netdb.h>
#else
    #include <winsock2.h>
#endif

namespace tg
{
    class Socket
    {
        public:
            static void init();
            static void destroy();
    };
}
