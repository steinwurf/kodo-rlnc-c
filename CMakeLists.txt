cmake_minimum_required(VERSION 3.12)
project(kodo_rlnc_c)

# Use waf to resolve dependencies
if(NOT DEFINED STEINWURF_RESOLVE)
  message(STATUS "Resolving dependencies...")
  execute_process(COMMAND python waf resolve
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  set(STEINWURF_RESOLVE "${CMAKE_CURRENT_SOURCE_DIR}/resolve_symlinks")
  set(STEINWURF_TOP_NAME kodo_rlnc_c)
endif()

# fifi dependency
if(NOT TARGET steinwurf::fifi)
  add_subdirectory("${STEINWURF_RESOLVE}/fifi" fifi)
endif()

# kodo-rlnc dependency
if(NOT TARGET steinwurf::kodo_rlnc)
  add_subdirectory("${STEINWURF_RESOLVE}/kodo-rlnc" kodo-rlnc)
endif()

# Define library
file(GLOB_RECURSE kodo_rlnc_c_sources ./src/*.cpp)

# Is this the top-level steinwurf project?
if(${PROJECT_NAME} STREQUAL ${STEINWURF_TOP_NAME})
  # Create static library
  add_library(kodo_rlnc_c STATIC ${kodo_rlnc_c_sources})
  # Get all steinwurf object libraries and link directly with them.
  get_property(steinwurf_object_libraries GLOBAL
               PROPERTY steinwurf::object_libraries)
  target_link_libraries(kodo_rlnc_c PRIVATE ${steinwurf_object_libraries})

  # Install library
  install(FILES $<TARGET_FILE:kodo_rlnc_c>
          DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
else()
  # Create object library
  add_library(kodo_rlnc_c OBJECT ${kodo_rlnc_c_sources})
  # Add this library to a global list of steinwurf object libraries
  set_property(GLOBAL APPEND PROPERTY steinwurf::object_libraries
                                      steinwurf::kodo_rlnc_c)
  # Link object dependencies
  target_link_libraries(kodo_rlnc_c PRIVATE steinwurf::fifi)
  target_link_libraries(kodo_rlnc_c PRIVATE steinwurf::kodo_rlnc)
endif()

# Indicate that project is being built as a static library.
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  target_compile_definitions(kodo_rlnc_c PUBLIC KODO_RLNC_C_STATIC)
endif()

target_include_directories(kodo_rlnc_c INTERFACE src)
target_compile_features(kodo_rlnc_c PUBLIC cxx_std_14)
add_library(steinwurf::kodo_rlnc_c ALIAS kodo_rlnc_c)

# Install headers excluding "detail" as these are internal to the library.
install(
  DIRECTORY ./src/kodo_rlnc_c
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  FILES_MATCHING
  PATTERN *.h
  PATTERN ./src/kodo_rlnc_c/detail EXCLUDE)

# Is top level project?
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  # Build executables
  add_executable(encode_decode_simple
                 examples/encode_decode_simple/encode_decode_simple.c)
  target_link_libraries(encode_decode_simple kodo_rlnc_c)
endif()
