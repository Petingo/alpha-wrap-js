cmake_minimum_required(VERSION 3.16)
project(alpha_wrap_js)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-DCGAL_DISABLE_ROUNDING_MATH_CHECK)  # emscripten doesn't support -frounding-math

# find_package doesn't work:
# https://github.com/emscripten-core/emscripten/issues/13310
set(CGAL_DIR /home/wsl/CGAL-5.6/include)
set(BOOST_DIR /home/wsl/boost_1_84_0)

if (DEFINED EMSCRIPTEN)
    add_executable(alpha_wrap src/alpha_wrap.cpp src/WebIDL/alpha_wrap_glue_wrapper.cpp)
    target_include_directories(alpha_wrap PRIVATE ${CGAL_DIR})
    target_include_directories(alpha_wrap PRIVATE ${BOOST_DIR})
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set_target_properties(alpha_wrap PROPERTIES LINK_FLAGS  "-s MODULARIZE --post-js ../src/WebIDL/alpha_wrap_glue.js")
endif()

# Test
add_executable(alpha_wrap_test src/alpha_wrap_test.cpp src/alpha_wrap.cpp)
target_include_directories(alpha_wrap_test PRIVATE ${CGAL_DIR})
target_include_directories(alpha_wrap_test PRIVATE ${BOOST_DIR})