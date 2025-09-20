#pragma once

#include "ftxui/component/component.hpp"
#include <functional>

// Creates the tools menu component.
ftxui::Component CreateToolsMenu(
    std::function<void()> on_open_texture_editor,
    std::function<void()> on_back
);
