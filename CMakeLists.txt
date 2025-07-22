cmake_minimum_required(VERSION 3.11)
project(dominators)
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address -fsanitize=undefined")
endif()

find_package(Boost REQUIRED)

set(SRCS
	${CMAKE_SOURCE_DIR}/dominators/src/dominators.cpp
)

add_executable(dominators.x
	${CMAKE_SOURCE_DIR}/dominators/src/dominators.cpp
)

target_compile_features(dominators.x PUBLIC cxx_std_20)
if((NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
        target_compile_options(dominators.x PUBLIC -std=c++20 -Wall -g -O0)
endif()

target_include_directories(dominators.x PUBLIC "${CMAKE_SOURCE_DIR}/dominators/include")
target_link_libraries(dominators.x PUBLIC boost::boost)

set(CLANG_FORMAT_SRCS
        ${SRCS}
	${CMAKE_SOURCE_DIR}/dominators/include/*.h
)

add_custom_target(
        clangformat
        ALL
        COMMAND clang-format -style=file -i ${CLANG_FORMAT_SRCS}
)
