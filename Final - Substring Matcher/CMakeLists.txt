cmake_minimum_required(VERSION 3.5)

project(SubtringMatcher_Naive_and_KMP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(SubtringMatcher_Naive_and_KMP main.cpp
                                             sample.h
                                             sample.cpp
                                             substringmatcher.h
                                             substringmatcher.cpp
                                             substringtester.h
                                             substringtester.cpp
                                             timer.h
                                             timer.cpp)

target_link_libraries(SubtringMatcher_Naive_and_KMP PRIVATE Threads::Threads)
