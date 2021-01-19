cmake_minimum_required (VERSION 3.10)
project(kodo_rlnc_c)

if(NOT DEFINED STEINWURF_RESOLVE)
    message(STATUS "Resolving dependencies...")
    execute_process(
        COMMAND python waf resolve
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set(STEINWURF_RESOLVE "${CMAKE_CURRENT_SOURCE_DIR}/resolve_symlinks")
endif()

file(GLOB kodo_rlnc_c_headers ./src/kodo_rlnc_c/*.h)

file(GLOB_RECURSE kodo_rlnc_c_sources ./src/*.cpp)
add_library(kodo_rlnc_c STATIC ${kodo_rlnc_c_sources})
add_library(steinwurf::kodo_rlnc_c ALIAS kodo_rlnc_c)

if (NOT TARGET kodo_rlnc)
add_subdirectory("${STEINWURF_RESOLVE}/kodo-rlnc" kodo_rlnc)
endif()

if (NOT TARGET fifi)
add_subdirectory("${STEINWURF_RESOLVE}/fifi" fifi)
endif()

target_link_libraries(kodo_rlnc_c
    PRIVATE steinwurf::kodo_rlnc
    PRIVATE steinwurf::fifi
)
target_compile_definitions(kodo_rlnc_c PUBLIC KODO_RLNC_C_STATIC)
target_compile_features(kodo_rlnc_c PUBLIC cxx_std_14)

target_include_directories(kodo_rlnc_c INTERFACE src)

add_executable(encode_decode_simple ./examples/encode_decode_simple/encode_decode_simple.c)
target_link_libraries(encode_decode_simple steinwurf::kodo_rlnc_c)

install(FILES ${kodo_rlnc_c_headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/kodo_rlnc_c)
install(FILES $<TARGET_FILE:kodo_rlnc_c> DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
