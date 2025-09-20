#pragma once
#include <string>
#include <atomic>
#include <future>
#include <optional>
#include "core/localization.hpp"

namespace modtoolbox::core {

// Represents the currently visible UI screen.
enum class View {
    Main,
    Configuration,
    Build,
    Tools,
    TextureEditor,
};

// A simple struct to hold the application's shared state.
struct AppState {
    View current_view = View::Main;
    std::string project_path;
    std::string project_path_input = "/home/user/project";
    std::string status_message;

    // State for the configuration menu inputs
    std::string config_mod_name;
    std::string config_mod_id;

    // State for the configuration menu inputs
    std::string config_mod_version;
    std::string config_mod_description;
    std::string config_mod_authors;



    // State for the build menu
    std::string build_output;
    std::atomic<bool> is_gradle_task_running = false;
    std::optional<std::future<void>> current_gradle_task_future;

    // State for the texture editor
    std::vector<std::vector<int>> texture_pixels = std::vector<std::vector<int>>(16, std::vector<int>(16, 0));
    int texture_cursor_x = 0;
    int texture_cursor_y = 0;
    int current_color_index = 1;
    std::string texture_save_path_input;
};

} // namespace modtoolbox::core