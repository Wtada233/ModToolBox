cmake_minimum_required(VERSION 3.16)
project(modtoolbox CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic -Wno-missing-field-initializers)

add_subdirectory(src/vendor/ftxui)

# Add the main executable
add_executable(modtoolbox
    src/main.cpp
    src/ui/main_menu.cpp
    src/ui/build_menu.cpp
    src/ui/config_menu.cpp
    src/ui/tools_menu.cpp
    src/ui/texture_editor.cpp
    src/core/project.cpp
    src/core/gradle.cpp
    src/core/config.cpp
    src/core/localization.cpp
)

target_include_directories(modtoolbox PRIVATE src)

# Link against FTXUI libraries
target_link_libraries(modtoolbox PRIVATE ftxui::screen ftxui::dom ftxui::component)