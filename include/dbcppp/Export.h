#pragma once

#ifdef _WIN32
#    ifdef DBCPPP_STATIC_DEFINE
#        define DBCPPP_API
#    elif defined(DBCPPP_EXPORT)
#        define DBCPPP_API __declspec(dllexport)
#    else
#        define DBCPPP_API __declspec(dllimport)
#    endif
#else
#   define DBCPPP_API
#endif
