cmake_minimum_required(VERSION 3.25)
project(EqbalATM)


set(CMAKE_CXX_STANDDARD 17)
set(CMAKE_CXX_STANDDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

function(add_sanitizers target)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${target} PRIVATE "-fsanitize=address,leak,undefined" -fno-omit-frame-pointer)
    target_link_options(${target} PRIVATE "-fsanitize=address,leak,undefined")
  endif()
endfunction()

function(add_compiler_warnings target)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
            -Wshadow
            -Wconversion
        )
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${target} PRIVATE
            /W4 /WX
        )
  endif()
endfunction()


if(NOT CMAKE_BUILD_TYPE)
  set(default_build_type "Debug")
  message(STATUS "Set the build type to `${default_build_type}` as none was specified.")
  set(CMAKE_BUILD_TYPE ${default_build_type} CACHE STRING "Chooce the build type." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
  "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
message(STATUS "EqbalATM Build Type: ${CMAKE_BUILD_TYPE}")

# Set the version for EqbalATM
set(EqbalATM_Version_Major 0)
set(EqbalATM_Version_Minor 1)
set(EqbalATM_Version_Patch 0)
set(EqbalATM_Version_Status "-dev")
set(PROJECT_VERSION
  "${EqbalATM_Version_Major}.${EqbalATM_Version_Minor}.${EqbalATM_Version_Patch}${EqbalATM_Version_Status}"
)
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  message(STATUS "Setting Debug flags")
  add_compile_options(-Og)
else()
  add_compile_options(-O3)
endif()

set(M_SRC
"${CMAKE_SOURCE_DIR}/src/main.cpp"
"${CMAKE_SOURCE_DIR}/src/utils.cpp"
"${CMAKE_SOURCE_DIR}/src/utils.hpp"
"${CMAKE_SOURCE_DIR}/src/db.cpp"
"${CMAKE_SOURCE_DIR}/src/db.hpp"
"${CMAKE_SOURCE_DIR}/src/atm.cpp"
"${CMAKE_SOURCE_DIR}/src/atm.hpp"
)

add_executable(
  ${PROJECT_NAME}
  ${M_SRC}
)
target_link_libraries(${PROJECT_NAME} PRIVATE "m")
add_sanitizers(${PROJECT_NAME})
add_compiler_warnings(${PROJECT_NAME})
