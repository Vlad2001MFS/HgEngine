cmake_minimum_required(VERSION 3.17)
project(HgEngine)

file(GLOB_RECURSE HgEngine_HEADERS "${PROJECT_SOURCE_DIR}/src/**.hpp")
file(GLOB_RECURSE HgEngine_HEADERS2 "${PROJECT_SOURCE_DIR}/src/**.h")
file(GLOB_RECURSE HgEngine_SOURCES "${PROJECT_SOURCE_DIR}/src/**.cpp")

add_library(HgEngine STATIC ${HgEngine_HEADERS} ${HgEngine_HEADERS2} ${HgEngine_SOURCES})
set_target_properties(HgEngine PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

add_subdirectory("${PROJECT_SOURCE_DIR}/../HandyFramework" "${PROJECT_SOURCE_DIR}/_build/HandyFramework")

target_include_directories(HgEngine PUBLIC "${PROJECT_SOURCE_DIR}/../HandyFramework/src")
target_link_libraries(HgEngine PUBLIC
    HandyFramework
    "SDL2"
    "SDL2_mixer"
    "SDL2_ttf"
    "GL"
    "GLEW"
)
