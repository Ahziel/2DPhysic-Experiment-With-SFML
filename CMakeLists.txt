cmake_minimum_required(VERSION 3.13)

project(MyProject VERSION 1.0.0 LANGUAGES C CXX)

## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

if(SFML_DIR STREQUAL "")
    option(SFML_DIR "Path to SFML")
endif()

add_subdirectory(thirdparty)

set(PROJECT_HEADER_FILES
    "include/particule.h"
)

set(PROJECT_SOURCE_FILES
    "src/particule.cpp"
)

# Only for how it is show in visual
source_group(TREE "${CMAKE_SOURCE_DIR}/include/" PREFIX "Header Files" FILES ${PROJECT_HEADER_FILES})
source_group(TREE "${CMAKE_SOURCE_DIR}/src/" PREFIX "Source Files" FILES ${PROJECT_SOURCE_FILES})

# Pack everything in the same place for the actual target generation
set(PROJECT_FILES
    ${PROJECT_HEADER_FILES}
    ${PROJECT_SOURCE_FILES}
)

add_executable(SFMLTest main.cpp ${PROJECT_FILES})
target_include_directories(SFMLTest PUBLIC "${CMAKE_SOURCE_DIR}/include/")

target_link_libraries(SFMLTest imguiSFML sfml-graphics sfml-window sfml-system sfml-audio)
add_dependencies(SFMLTest imguiSFML)
