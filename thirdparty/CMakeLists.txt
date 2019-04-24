find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

set(IMGUI_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")

set(IMGUI_HEADER_FILES
    "imgui/imconfig.h"
    "imgui/imgui.h"
    "imgui/imgui-SFML.h"
    "imgui/imgui_internal.h"
    "imgui/imstb_rectpack.h"
    "imgui/imstb_textedit.h"
    "imgui/imstb_truetype.h"
)

set(IMGUI_SOURCE_FILES
    "imgui/imgui.cpp"
    "imgui/imgui-SFML.cpp"
    "imgui/imgui_demo.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_widgets.cpp"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/imgui/" PREFIX "Header Files" FILES ${IMGUI_HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/imgui/" PREFIX "Source Files" FILES ${IMGUI_SOURCE_FILES})

add_library(imguiSFML STATIC ${IMGUI_HEADER_FILES} ${IMGUI_SOURCE_FILES})

target_link_libraries(imguiSFML sfml-graphics sfml-window sfml-system)

target_include_directories(imguiSFML
    PUBLIC
        $<BUILD_INTERFACE:${IMGUI_INCLUDE_DIR}>
)