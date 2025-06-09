cmake_minimum_required(VERSION 3.14)

project(BannerServer)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

# Указываем файлы проекта
add_executable(BannerServer
    main.cpp
    server.cpp
    server.h   # обязательно добавить заголовочный файл с Q_OBJECT!
)

# Подключаем нужные Qt-библиотеки
target_link_libraries(BannerServer Qt6::Core Qt6::Network)
