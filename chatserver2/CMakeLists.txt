
file(GLOB GATESERVER_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)

add_executable(ChatServer2 ${GATESERVER_SRC})

# 添加公共头文件目录
target_include_directories(ChatServer2
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 链接公共库
target_link_libraries(ChatServer2 PRIVATE ${COMMON_LIBS})
