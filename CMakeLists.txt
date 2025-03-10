cmake_minimum_required(VERSION 3.5.0)
project(bi_ring VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include(FetchContent)
FetchContent_Declare( 
   googletest
   GIT_REPOSITORY      https://github.com/google/googletest.git
   GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

set(PROJECT_SOURCES bi_ring.h br_usage.h)

add_executable(testGoogle testGoogle.cpp ${PROJECT_SOURCES})
add_executable(bi_ring main.cpp  ${PROJECT_SOURCES})

target_link_libraries(testGoogle PUBLIC gtest_main)

