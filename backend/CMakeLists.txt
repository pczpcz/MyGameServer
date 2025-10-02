cmake_minimum_required(VERSION 3.16)
project(MyApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
find_package(cpprestsdk REQUIRED)

# Hardcoded mysqlcppconn paths (Ubuntu 22.04 default locations)
set(MYSQLCPPCONN_INCLUDE_DIRS "/usr/include/cppconn")
set(MYSQLCPPCONN_LIBRARIES "/usr/lib/x86_64-linux-gnu/libmysqlcppconn.so")

# Include directories
include_directories(${CPPREST_INCLUDE_DIRS})
include_directories(${MYSQLCPPCONN_INCLUDE_DIRS})

# Add executable
add_executable(myapp
    src/main.cpp
    src/Database.cpp
    src/UserController.cpp
    src/UserService.cpp
    src/User.cpp
)

# Link libraries
target_link_libraries(myapp
    ${MYSQLCPPCONN_LIBRARIES}
    pthread
    ssl
    crypto
    cpprestsdk::cpprest
)

# Compiler flags
target_compile_options(myapp PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
