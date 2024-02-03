cmake_minimum_required(VERSION 3.16)
project(alpha_wrap_js)

set(CMAKE_CXX_STANDARD 17)

#add_definitions(-DCGAL_DISABLE_ROUNDING_MATH_CHECK)  # emscripten doesn't support -frounding-math

set(CGAL_INCLUDE_DIR /home/wsl/CGAL-5.6/include)
set(BOOST_INCLUDE_DIR /home/wsl/boost_1_84_0)

add_definitions(-DCGAL_AW3_DEBUG_INITIALIZATION)

if (DEFINED EMSCRIPTEN)
    add_definitions(-DCGAL_DISABLE_ROUNDING_MATH_CHECK)

    # find_package doesn't work for emscripten:
    # https://github.com/emscripten-core/emscripten/issues/13310

    set(GMP_INCLUDE_DIR /home/wsl/gmp-em/include)
    set(GMP_LIBRARIES -L/home/wsl/gmp-em/lib;libgmp.a)
    set(MPFR_INCLUDE_DIR /home/wsl/mpfr-em/include)
    set(MPFR_LIBRARIES -L/home/wsl/mpfr-em/lib;libmpfr.a)

    add_executable(alpha_wrap src/alpha_wrap.cpp src/WebIDL/alpha_wrap_glue_wrapper.cpp)
#    target_link_libraries(alpha_wrap PRIVATE ${CGAL_DIR})
#    target_include_directories(alpha_wrap PRIVATE ${CGAL_DIR})
#    target_include_directories(alpha_wrap PRIVATE ${BOOST_DIR})
    target_include_directories(alpha_wrap PRIVATE ${CGAL_INCLUDE_DIR})
    target_include_directories(alpha_wrap PRIVATE ${BOOST_INCLUDE_DIR})
    target_include_directories(alpha_wrap PRIVATE ${MPFR_INCLUDE_DIR})
    target_include_directories(alpha_wrap PRIVATE ${GMP_INCLUDE_DIR})
    target_link_libraries(alpha_wrap PRIVATE ${MPFR_LIBRARIES})
    target_link_libraries(alpha_wrap PRIVATE ${GMP_LIBRARIES})

    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set_target_properties(alpha_wrap PROPERTIES LINK_FLAGS  "-O2 -s MODULARIZE --post-js ../src/WebIDL/alpha_wrap_glue.js")

#    add_executable(alpha_wrap_test src/alpha_wrap_test.cpp src/alpha_wrap.cpp)
#    target_include_directories(alpha_wrap_test PRIVATE ${CGAL_DIR})
#    target_include_directories(alpha_wrap_test PRIVATE ${BOOST_DIR})

else()
    set(GMP_INCLUDE_DIR /home/wsl/gmp-c/include)
    set(GMP_LIBRARIES -L/home/wsl/gmp-c/lib;libgmp.a)
    set(MPFR_INCLUDE_DIR /home/wsl/mpfr-c/include)
    set(MPFR_LIBRARIES -L/home/wsl/mpfr-c/lib;libmpfr.a)
    # Test
#    find_package(CGAL REQUIRED)
    add_executable(alpha_wrap_test src/alpha_wrap_test.cpp src/alpha_wrap.cpp)
#    target_link_libraries(alpha_wrap_test CGAL::CGAL)
#    message(${CGAL})
    target_include_directories(alpha_wrap_test PRIVATE ${CGAL_INCLUDE_DIR})
    target_include_directories(alpha_wrap_test PRIVATE ${BOOST_INCLUDE_DIR})
    target_include_directories(alpha_wrap_test PRIVATE ${MPFR_INCLUDE_DIR})
    target_include_directories(alpha_wrap_test PRIVATE ${GMP_INCLUDE_DIR})
    target_link_libraries(alpha_wrap_test PRIVATE ${MPFR_LIBRARIES})
    target_link_libraries(alpha_wrap_test PRIVATE ${GMP_LIBRARIES})

endif()

