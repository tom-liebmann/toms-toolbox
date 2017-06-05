#pragma once

#include <string>

#ifndef WIN32
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#else
#include <winsock2.h>
#endif

namespace ttb
{
    class Socket
    {
    public:
        static void init();
        static void destroy();
    };
}
