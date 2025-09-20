#include "main_menu.hpp"
#include "ftxui/component/component.hpp"
#include "core/localization.hpp"

// This variable will hold the state of the selected menu item.
// It's static so it persists between renders.
static int menu_selected_item = 0;

ftxui::Component CreateMainMenu(
    std::function<void()> on_switch_project,
    std::function<void()> on_open_build,
    std::function<void()> on_open_config,
    std::function<void()> on_open_tools,
    std::function<void()> on_quit
) {
    using namespace ftxui;

    // The menu entries can also be static as they don't change.
    static std::vector<std::string> entries = {
        modtoolbox::core::Localization::Get("main_menu_build"),
        modtoolbox::core::Localization::Get("main_menu_configuration"),
        modtoolbox::core::Localization::Get("main_menu_tools"),
        modtoolbox::core::Localization::Get("main_menu_switch_project"),
        modtoolbox::core::Localization::Get("main_menu_exit"),
    };

    // Use MenuOption to handle the on_enter event.
    MenuOption option;
    option.on_enter = [&, on_switch_project, on_open_config, on_open_build, on_open_tools, on_quit] {
        // When enter is pressed, check the value of our state variable.
        if (menu_selected_item == 0) on_open_build();
        if (menu_selected_item == 1) on_open_config();
        if (menu_selected_item == 2) on_open_tools();
        if (menu_selected_item == 3) on_switch_project();
        if (menu_selected_item == 4) on_quit();
    };

    // Pass the entries, a pointer to the selected item state, and the options.
    return Menu(&entries, &menu_selected_item, option);
}
