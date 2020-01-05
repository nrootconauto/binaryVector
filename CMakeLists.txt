cmake_minimum_required(VERSION 3.13)
#debug build
set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)
#SET(CMAKE_CXX_COMPILER /home/tc/.zapcc/build/bin/zapcc++)

#project
project(binaryGraph)
#export the compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#recusivly get files in ./src/ and ./headers/
file(GLOB_RECURSE testSources "./src/*.cpp")
list(FILTER testSources INCLUDE REGEX "^[^\.].+\.cpp$")

#add the files to the project
add_executable(test ./src/test.cpp)

# make the binary vector library
add_library(binaryVector STATIC ./src/binaryVector.cpp ./)
#add bitarray lib
target_include_directories(binaryVector PUBLIC ./include)

target_link_libraries(test PUBLIC binaryVector)
