cmake_minimum_required(VERSION 3.28.3)

set(PATCH_VERSION "1" CACHE INTERNAL "PATCH_VERSION" )
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(join_server VERSION ${PROJECT_VERSION} LANGUAGES C CXX)

configure_file(version.h.in version.h)




set ( HW11_INCLUDE
   src/TcpConnection.hpp
   src/ConnectionManager.hpp
   src/AsyncTcpServer.hpp
   src/SQLCommand.hpp
   src/SQLCommandParser.hpp
   src/SQLExecutor.hpp
   src/SQLTableRow.hpp
   src/SQLOutput.hpp
)
set ( HW11_SOURCES   
   src/TcpConnection.cpp
   src/ConnectionManager.cpp
   src/AsyncTcpServer.cpp
   src/SQLCommandParser.cpp
   src/SQLExecutor.cpp
 )


find_package (Boost COMPONENTS asio)
find_package(SQLite3 REQUIRED)

include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR} ${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inlude)

add_executable(join_server ${HW11_INCLUDE} ${HW11_SOURCES} main.cpp)

target_link_libraries (join_server 
    SQLite::SQLite3
)

set_property(TARGET join_server PROPERTY CXX_STANDARD 23)

install(TARGETS join_server DESTINATION . )


include("functions.cmake")
generate_packages(${PROJECT_NAME})










