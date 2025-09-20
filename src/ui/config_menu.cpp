#include "config_menu.hpp"
#include "core/app_state.hpp"
#include "core/config.hpp" // For ReadModJson/WriteModJson
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "core/localization.hpp" // Added this include

namespace modtoolbox::ui {

ftxui::Component CreateConfigMenu(modtoolbox::core::AppState& app_state, std::function<void()> on_back) {
    using namespace ftxui;

    // Input fields for mod details
    auto mod_name_input = Input(&app_state.config_mod_name, modtoolbox::core::Localization::Get("config_mod_name_placeholder"));
    auto mod_id_input = Input(&app_state.config_mod_id, modtoolbox::core::Localization::Get("config_mod_id_placeholder"));
    auto mod_version_input = Input(&app_state.config_mod_version, modtoolbox::core::Localization::Get("config_version_placeholder"));
    auto mod_description_input = Input(&app_state.config_mod_description, modtoolbox::core::Localization::Get("config_description_placeholder"));
    auto mod_authors_input = Input(&app_state.config_mod_authors, modtoolbox::core::Localization::Get("config_authors_placeholder"));

    auto save_button = Button(modtoolbox::core::Localization::Get("config_save_button_label"), [&] {
        if (app_state.project_path != "No Project Opened") {
            std::string mod_json_path = app_state.project_path + "/src/main/resources/fabric.mod.json";
            if (WriteModJson(mod_json_path, app_state.config_mod_name, app_state.config_mod_id, app_state.config_mod_version, app_state.config_mod_description, app_state.config_mod_authors)) {
                app_state.status_message = modtoolbox::core::Localization::Get("config_fabric_mod_json_saved_successfully");
            } else {
                app_state.status_message = modtoolbox::core::Localization::Get("mod_config_save_error");
            }
        }
    });

    auto reload_button = Button(modtoolbox::core::Localization::Get("config_reload_button_label"), [&] {
        if (app_state.project_path != "No Project Opened") {
            std::string mod_json_path = app_state.project_path + "/src/main/resources/fabric.mod.json";
            if (ReadModJson(mod_json_path, app_state.config_mod_name, app_state.config_mod_id, app_state.config_mod_version, app_state.config_mod_description, app_state.config_mod_authors)) {
                app_state.status_message = "fabric.mod.json reloaded successfully!";
            } else {
                app_state.status_message = modtoolbox::core::Localization::Get("config_error_failed_to_reload_fabric_mod_json");
            }
        } else {
            app_state.status_message = "Error: No project is open.";
        }
    });

    auto buttons = Container::Horizontal({
        save_button,
        reload_button,
        Button(modtoolbox::core::Localization::Get("common_back"), on_back),
    });

    auto container = Container::Vertical({
        mod_name_input,
        mod_id_input,
        mod_version_input,
        mod_description_input,
        mod_authors_input,
        buttons,
    });

    return Renderer(container, [=] {
        return vbox({
            text(modtoolbox::core::Localization::Get("config_mod_configuration_title")) | bold | hcenter,
            separator(),
            mod_name_input->Render(),
            mod_id_input->Render(),
            mod_version_input->Render(),
            mod_description_input->Render(),
            mod_authors_input->Render(),
            separator(),
            buttons->Render(),
        }) | border;
    });
}

} // namespace modtoolbox::ui