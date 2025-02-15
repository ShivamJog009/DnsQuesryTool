cmake_minimum_required(VERSION 3.10)
project(dns_query_tool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(dns_query_tool src/main.cpp
    src/DnsResolver.h src/DnsResolver.cpp
)

# Explicitly link WinSock for Windows
if(WIN32)
    target_link_libraries(dns_query_tool ws2_32)
endif()
