cmake_minimum_required(VERSION 3.14)
project(AeroChat)

# ----------------------
# 基本设置
# ----------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ----------------------
# 查找 hiredis
# ----------------------
find_path(HIREDIS_INCLUDE_DIR
    NAMES hiredis/hiredis.h
    PATHS /usr/include /usr/local/include
)

find_library(HIREDIS_LIBRARY
    NAMES hiredis
    PATHS /usr/lib /usr/local/lib
)

if(NOT HIREDIS_INCLUDE_DIR OR NOT HIREDIS_LIBRARY)
    message(FATAL_ERROR "hiredis not found!")
endif()

# ----------------------
# 查找 MySQL Connector/C++
# ----------------------
# 查找 MySQL Connector/C++
find_path(MYSQLCPP_INCLUDE_DIR
    NAMES mysql_driver.h
    PATHS /usr/include /usr/local/include
)

find_library(MYSQLCPP_LIBRARY
    NAMES mysqlcppconn
    PATHS /usr/lib/x86_64-linux-gnu /usr/local/lib
)

if(NOT MYSQLCPP_INCLUDE_DIR OR NOT MYSQLCPP_LIBRARY)
    message(FATAL_ERROR "MySQL Connector/C++ not found!")
endif()


# ----------------------
# 查找 Boost 1.86
# ----------------------
set(BOOST_ROOT /usr)
set(BOOST_INCLUDEDIR /usr/include)
set(BOOST_LIBRARYDIR /usr/lib)

find_package(Boost 1.86 REQUIRED COMPONENTS filesystem system)

# ----------------------
# 查找其他依赖
# ----------------------
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# ----------------------
# 公共库
# ----------------------
set(COMMON_LIBS
    Boost::system
    Boost::filesystem
    pthread
    gRPC::grpc++
    jsoncpp
    ${HIREDIS_LIBRARY}
    ${MYSQLCPP_LIBRARY}
)

# ----------------------
# 子模块
# ----------------------
add_subdirectory(chatserver1)
add_subdirectory(chatserver2)
add_subdirectory(gateserver)
add_subdirectory(statusserver)

