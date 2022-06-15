project(AVL_CPP)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_PROJECT_VERSION_MAJOR 0)
set(CMAKE_PROJECT_VERSION_MINOR 1)

cmake_minimum_required(VERSION 3.16)

include(FetchContent)
fetchcontent_declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
fetchcontent_makeavailable(googletest)


set(CMAKE_CXX_STANDARD 20)

add_library(libavl INTERFACE)

add_executable(
  avl_cpp
  src/main.cpp
)

target_include_directories(
  libavl
  INTERFACE include
)

add_executable(
  avl_cpp_test
  test/NodeTest.cpp
)

target_link_libraries(avl_cpp libavl)
target_link_libraries(avl_cpp_test libavl gtest_main)

include(GoogleTest)
gtest_discover_tests(avl_cpp_test)
