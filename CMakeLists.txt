cmake_minimum_required(VERSION 3.16)
project(chatter_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)  # 启用 AUTOMOC，自动处理 Q_OBJECT
set(CMAKE_AUTORCC ON)  # 启用 AUTORCC，处理资源文件
set(CMAKE_AUTOUIC ON)  # 如果有 UI 文件，启用 AUTOUIC

# 查找 Qt6
find_package(Qt6 COMPONENTS Core Network Widgets Concurrent REQUIRED)

# 客户端可执行文件
add_executable(ChatClient
    client/main.cpp
    client/ChatClient.h
    client/ChatClient.cpp
    client/LoginWindow.h
    client/LoginWindow.cpp
    client/RegisterWindow.h
    client/RegisterWindow.cpp
    client/ChatWindow.h
    client/ChatWindow.cpp
    client/resources.qrc  # 新增：包含资源文件
)
target_include_directories(ChatClient PRIVATE client)
target_link_libraries(ChatClient PRIVATE Qt6::Core Qt6::Network Qt6::Widgets)

# 服务器可执行文件
add_executable(ChatServer
    server/main.cpp
    server/ChatServer.h
    server/ChatServer.cpp
)
target_include_directories(ChatServer PRIVATE server)
target_link_libraries(ChatServer PRIVATE Qt6::Core Qt6::Network Qt6::Concurrent)
