#pragma once

#include "ftxui/component/component.hpp"
#include "../core/app_state.hpp"
#include <functional>

/**
 * @brief Creates the main menu component.
 *
 * @param app_state A reference to the shared application state.
 * @param on_switch_project A callback function to trigger when the user wants to switch projects.
 * @param on_open_config A callback function to trigger when the user wants to open configuration.
 * @param on_quit A callback function to trigger when the user wants to quit.
 * @return The main menu component.
 */
ftxui::Component CreateMainMenu(
    std::function<void()> on_switch_project,
    std::function<void()> on_open_config,
    std::function<void()> on_open_build,
    std::function<void()> on_open_tools,
    std::function<void()> on_quit
);
