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

add_executable(
  avl_cpp
  src/main.cpp
)

add_executable(
  avl_cpp_test
  test/NodeTest.cpp
)

set_property(
  TARGET avl_cpp
  PROPERTY CXX_STANDARD 20
)

set_property(
  TARGET avl_cpp_test
  PROPERTY CXX_STANDARD 20
)

target_include_directories(
  avl_cpp
  PUBLIC src
  PRIVATE gtest_main
)

target_include_directories(
  avl_cpp_test
  PRIVATE gtest_main
  PRIVATE src
)

target_link_libraries(avl_cpp_test gtest_main)

include(GoogleTest)
gtest_discover_tests(avl_cpp_test)
