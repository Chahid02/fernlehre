cmake_minimum_required(VERSION 3.18)
project(groupcommunication VERSION 1.0.0)
 
add_executable(groupcommunication main.c gui.c middleware.c)

find_package(pthread)
target_link_libraries(groupcommunication pthread)