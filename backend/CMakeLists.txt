cmake_minimum_required(VERSION 3.16)
project(MyApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(cpprestsdk REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)

# Hardcoded mysqlcppconn paths (Ubuntu 22.04 default locations)
set(MYSQLCPPCONN_INCLUDE_DIRS "/usr/include/cppconn")
set(MYSQLCPPCONN_LIBRARIES "/usr/lib/x86_64-linux-gnu/libmysqlcppconn.so")

# Include directories
include_directories(${CPPREST_INCLUDE_DIRS})
include_directories(${MYSQLCPPCONN_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(src/udp)
include_directories(src)

# Add executable
add_executable(myapp
    src/main.cpp
    src/Database.cpp
    src/UserController.cpp
    src/UserService.cpp
    src/User.cpp
    src/udp/UdpPacket.cpp
    src/udp/UdpReceiver.cpp
    src/UdpServer.cpp
    src/PacketProcessor.cpp
)

# Link libraries
target_link_libraries(myapp
    ${MYSQLCPPCONN_LIBRARIES}
    pthread
    ssl
    crypto
    cpprestsdk::cpprest
    ${Boost_LIBRARIES}
)

# Compiler flags
target_compile_options(myapp PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
