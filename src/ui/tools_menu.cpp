#include "tools_menu.hpp"
#include "ftxui/component/component.hpp"
#include "core/localization.hpp"

static int menu_selected_item = 0;

ftxui::Component CreateToolsMenu(
    std::function<void()> on_open_texture_editor,
    std::function<void()> on_back
) {
    using namespace ftxui;

    static std::vector<std::string> entries = {
        modtoolbox::core::Localization::Get("texture_editor_title"),
        modtoolbox::core::Localization::Get("common_back"),
    };

    MenuOption option;
    option.on_enter = [=] {
        if (menu_selected_item == 0) on_open_texture_editor();
        if (menu_selected_item == 1) on_back();
    };

    return Menu(&entries, &menu_selected_item, option);
}
