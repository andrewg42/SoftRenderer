cmake_minimum_required(VERSION 3.20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_FLAGS "-g")

project(SoftRenderer LANGUAGES C CXX)

if(WIN32)
    add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
endif()

if(NOT MSVC)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        message(STATUS "Found CCache: ${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PROGRAM})
    endif()
endif()

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE headers CONFIGURE_DEPENDS include/*.h)

add_executable(main ${sources} ${headers})
target_include_directories(main PUBLIC include)
#target_compile_options(main PUBLIC -Wall)

set(THIRD_PARTY third_party)
add_subdirectory(${THIRD_PARTY})

add_custom_target(clang-format
    COMMAND clang-format -style=file -i ${sources} ${headers}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

#find_package(OpenMP)
#if(OpenMP_CXX_FOUND)
#    target_link_libraries(main PUBLIC OpenMP::OpenMP_CXX)
#endif()
