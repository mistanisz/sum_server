cmake_minimum_required(VERSION 3.22)
project(sum_server)

set(CMAKE_CXX_STANDARD 11)

add_executable(sum_server main.cpp src/Server.cpp src/Server.h src/utils.cpp src/utils.h src/Handler.h src/SumHandler.cpp src/SumHandler.h)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(sum_server PRIVATE Threads::Threads)
