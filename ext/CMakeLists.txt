cmake_minimum_required(VERSION 3.13)

project(skinnyMutex)

add_library(libSkinnyMutex STATIC ./skinny-mutex/skinny_mutex.c)

#include the pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(libSkinnyMutex PUBLIC Threads::Threads)

#include the header
target_include_directories(libSkinnyMutex INTERFACE ./skinny-mutex/)
