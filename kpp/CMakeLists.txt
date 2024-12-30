cmake_minimum_required(VERSION 3.10)
project(NavalBattle)

# Настройка стандарта C++ (используем C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Серверная часть
add_executable(server
    server.cpp
    GameManager.cpp
    Player.cpp
)

# Клиентская часть
add_executable(client
    client.cpp
    GameClient.cpp
)