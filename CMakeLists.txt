cmake_minimum_required(VERSION 3.16)
project(alpha_wrap_js)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DCGAL_DISABLE_ROUNDING_MATH_CHECK)  # emscripten doesn't support -frounding-math
add_definitions(-DCGAL_USE_SSE2)
#add_definitions(-DCGAL_ALWAYS_ROUND_TO_NEAREST)
add_definitions(-DCGAL_HAS_NO_THREADS)
add_definitions(-DCGAL_NO_ASSERTIONS)
add_definitions(-DCGAL_FORWARD)
add_definitions(-DBOOST_MATH_DISABLE_STD_FPCLASSIFY)
add_definitions(-DBOOST_NO_NATIVE_LONG_DOUBLE_FP_CLASSIFY)
add_definitions(-DBOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS)

#

#add_definitions(-DCGAL_FPU_HAS_EXCESS_PRECISION)

add_definitions(-DCGAL_AW3_DEBUG_INITIALIZATION)
add_definitions(-DCGAL_AW3_DEBUG_DUMP_EVERY_STEP)

set(CGAL_INCLUDE_DIR /home/wsl/CGAL-5.6/include)
set(BOOST_INCLUDE_DIR /home/wsl/boost_1_84_0)

if (DEFINED EMSCRIPTEN)
    # find_package doesn't work for emscripten:
    # https://github.com/emscripten-core/emscripten/issues/13310

    set(GMP_INCLUDE_DIR /home/wsl/gmp-em/include)
    set(GMP_LIBRARIES -L/home/wsl/gmp-em/lib;libgmp.a)
    set(MPFR_INCLUDE_DIR /home/wsl/mpfr-em/include)
    set(MPFR_LIBRARIES -L/home/wsl/mpfr-em/lib;libmpfr.a)

    add_executable(alpha_wrap
            src/alpha_wrap.cpp
            src/WebIDL/alpha_wrap_glue_wrapper.cpp
            src/serializer.cpp
            src/serializer.h)

    # cgal & boost
    target_include_directories(alpha_wrap PRIVATE ${CGAL_INCLUDE_DIR})
    target_include_directories(alpha_wrap PRIVATE ${BOOST_INCLUDE_DIR})

    # mpfr
    target_link_libraries(alpha_wrap PRIVATE ${MPFR_LIBRARIES})
    target_include_directories(alpha_wrap PRIVATE ${MPFR_INCLUDE_DIR})

    # fmp
    target_include_directories(alpha_wrap PRIVATE ${GMP_INCLUDE_DIR})
    target_link_libraries(alpha_wrap PRIVATE ${GMP_LIBRARIES})

    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set_target_properties(alpha_wrap PROPERTIES LINK_FLAGS "-s MODULARIZE -s 'EXPORT_NAME=\"createAlphaWrapModule\"' -s EXPORT_ES6=1 --post-js ../src/WebIDL/alpha_wrap_glue.js --ignore-dynamic-linking ")
    set_target_properties(alpha_wrap PROPERTIES COMPILE_FLAGS "-g -O0 -msimd128 -msse2 -v -U__GNUG__")
#    set_target_properties(alpha_wrap PROPERTIES LINK_FLAGS )


    add_executable(test src/test.cpp)
    set_target_properties(test PROPERTIES COMPILE_FLAGS "-msimd128 -msse2 -s MODULARIZE -s 'EXPORT_NAME=\"createAlphaWrapModule\"' -s EXPORT_ES6=1 --post-js ../src/WebIDL/alpha_wrap_glue.js")


else()

    add_executable(alpha_wrap_test
            src/alpha_wrap_test.cpp
            src/alpha_wrap.cpp
            src/serializer.cpp
            src/serializer.h
            src/test.cpp
            src/test.h)

    if(DEFINED TEST_USE_SYSTEM_CGAL_PKG)
        find_package(CGAL REQUIRED)
        target_link_libraries(alpha_wrap_test CGAL::CGAL)

    else()
        set(GMP_INCLUDE_DIR /home/wsl/gmp-c/include)
        set(GMP_LIBRARIES -L/home/wsl/gmp-c/lib;libgmp.a)
        set(MPFR_INCLUDE_DIR /home/wsl/mpfr-c/include)
        set(MPFR_LIBRARIES -L/home/wsl/mpfr-c/lib;libmpfr.a)

        # cgal & boost
        target_include_directories(alpha_wrap_test PRIVATE ${CGAL_INCLUDE_DIR})
        target_include_directories(alpha_wrap_test PRIVATE ${BOOST_INCLUDE_DIR})

        # mpfr
        target_include_directories(alpha_wrap_test PRIVATE ${MPFR_INCLUDE_DIR})
        target_link_libraries(alpha_wrap_test PRIVATE ${MPFR_LIBRARIES})

        # gmp
        target_include_directories(alpha_wrap_test PRIVATE ${GMP_INCLUDE_DIR})
        target_link_libraries(alpha_wrap_test PRIVATE ${GMP_LIBRARIES})

    endif()

endif()

