cmake_minimum_required(VERSION 4.4.0)
project(Titian LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SOL2_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(assimp)

add_subdirectory(jolt/build)

add_library(lualib lua/onelua.c)
target_include_directories(lualib PUBLIC lua)
target_include_directories(lualib PUBLIC sol2/include)

file(GLOB_RECURSE imgui_sources CONFIGURE_DEPENDS imgui/*.cpp)
add_library(imguilib ${imgui_sources})
target_include_directories(imguilib PUBLIC imgui)

add_subdirectory(klibrary)
target_link_libraries(klibrarylib PUBLIC imguilib)

file(GLOB_RECURSE titian_sources CONFIGURE_DEPENDS titian/*.cpp)
add_library(titianlib ${titian_sources})
target_include_directories(titianlib PUBLIC titian)
target_link_libraries(titianlib PUBLIC assimp)
target_link_libraries(titianlib PUBLIC Jolt)
target_link_libraries(titianlib PUBLIC lualib)
target_link_libraries(titianlib PUBLIC klibrarylib)

file(GLOB_RECURSE fuze_sources CONFIGURE_DEPENDS fuze/*.cpp)
add_library(fuzelib ${fuze_sources})
target_include_directories(fuzelib PUBLIC fuze)
target_link_libraries(fuzelib PUBLIC titianlib)

file(GLOB_RECURSE sandbox_sources CONFIGURE_DEPENDS sandbox/*.cpp)
add_library(sandboxlib ${sandbox_sources})
target_include_directories(sandboxlib PUBLIC sandbox)
target_link_libraries(sandboxlib PUBLIC titianlib)

add_executable(titian
    main.cpp
)
target_link_libraries(titian PUBLIC titianlib)
target_link_libraries(titian PUBLIC fuzelib)
target_link_libraries(titian PUBLIC sandboxlib)
set_target_properties(titian PROPERTIES
    DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/workdir)
