# gateserver/CMakeLists.txt

file(GLOB GATESERVER_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)

add_executable(GateServer ${GATESERVER_SRC})

# 添加公共头文件目录
target_include_directories(GateServer
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 链接公共库
target_link_libraries(GateServer PRIVATE ${COMMON_LIBS})
