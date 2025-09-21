#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"

#include "core/app_state.hpp"
#include "core/project.hpp"
#include "core/config.hpp"
#include "core/gradle.hpp"
#include "core/localization.hpp"

#include "ui/main_menu.hpp"
#include "ui/config_menu.hpp"
#include "ui/build_menu.hpp"
#include "ui/tools_menu.hpp"
#include "ui/texture_editor.hpp"

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include <vendor/json.hpp>
#include <memory>

// Global state accessible by different modules.
modtoolbox::core::AppState app_state;

int main() {
    // Set global color support.
    ftxui::Terminal::SetColorSupport(ftxui::Terminal::TrueColor);

    // Initialize localization
    modtoolbox::core::Localization::Init();

    // Initialize app_state fields that depend on localization
  modtoolbox::core::AppState app_state;
    app_state.status_message = modtoolbox::core::Localization::Get("app_state_welcome_message");
    app_state.build_output = modtoolbox::core::Localization::Get("app_state_no_build_output");

    using namespace ftxui;

    auto screen = ScreenInteractive::FullscreenAlternateScreen();

    // --- STATE ---
    bool show_project_modal = false;
    bool show_texture_save_modal = false;

    // --- CALLBACKS ---
    auto quit_callback = screen.ExitLoopClosure();
    auto to_main_view_callback = [&] { app_state.current_view = modtoolbox::core::View::Main; };

    auto switch_project_callback = [&] {
        app_state.project_path_input = app_state.project_path == "No Project Opened" ? "" : app_state.project_path;
        show_project_modal = true;
    };

    auto to_build_view_callback = [&] {
        if (app_state.project_path != "No Project Opened") {
            app_state.current_view = modtoolbox::core::View::Build;
        }
        app_state.status_message = modtoolbox::core::Localization::Get("project_status_error_no_project");
    };

    auto to_config_view_callback = [&] {
        if (app_state.project_path != "No Project Opened") {
            // Read existing mod.json entries when opening the editor
            std::string mod_json_path = app_state.project_path + "/src/main/resources/fabric.mod.json";
            if (ReadModJson(mod_json_path, app_state.config_mod_name, app_state.config_mod_id, app_state.config_mod_version, app_state.config_mod_description, app_state.config_mod_authors)) {
                app_state.status_message = modtoolbox::core::Localization::Get("mod_config_loaded");
            } else {
                app_state.status_message = modtoolbox::core::Localization::Get("mod_config_load_error");
                app_state.config_mod_name.clear();
                app_state.config_mod_id.clear();
                app_state.config_mod_version.clear();
                app_state.config_mod_authors.clear();
            }
            app_state.current_view = modtoolbox::core::View::Configuration;
        } else {
            app_state.status_message = modtoolbox::core::Localization::Get("error_no_project_open");
        }
    };

    auto to_tools_view_callback = [&] { app_state.current_view = modtoolbox::core::View::Tools; };
    auto to_texture_editor_callback = [&] { app_state.current_view = modtoolbox::core::View::TextureEditor; };

    // --- TEXTURE EDITOR MODAL ---
    auto on_texture_editor_enter = [&] { show_texture_save_modal = true; };
    auto on_texture_save_submit = [&] {
        // Trim trailing newline characters
        while (!app_state.texture_save_path_input.empty() &&
               (app_state.texture_save_path_input.back() == '\n' || app_state.texture_save_path_input.back() == '\r')) {
            app_state.texture_save_path_input.pop_back();
        }

        if (!app_state.texture_save_path_input.empty()) {
            SaveTextureToPng(app_state.texture_save_path_input);
            app_state.status_message = modtoolbox::core::Localization::Get("texture_editor_save_success") + app_state.texture_save_path_input;
            app_state.texture_save_path_input.clear();
            show_texture_save_modal = false;
        }
    };
    InputOption texture_save_options;
    texture_save_options.on_enter = on_texture_save_submit;
    auto texture_save_input = Input(&app_state.texture_save_path_input, modtoolbox::core::Localization::Get("texture_editor_save_path_placeholder"), texture_save_options);
    auto texture_save_ok_button = Button(modtoolbox::core::Localization::Get("common_save"), on_texture_save_submit);
    auto texture_save_cancel_button = Button(modtoolbox::core::Localization::Get("common_cancel"), [&] {
        app_state.texture_save_path_input.clear();
        show_texture_save_modal = false;
    });
    ftxui::Components texture_save_modal_children = {
        texture_save_input,
        Container::Horizontal({texture_save_ok_button, texture_save_cancel_button}),
    };
    auto texture_save_modal_component = Container::Vertical(texture_save_modal_children);

    // --- PROJECT MODAL ---
    auto on_project_submit = [&] {
        while (!app_state.project_path_input.empty() &&
               (app_state.project_path_input.back() == '\n' || app_state.project_path_input.back() == '\r')) {
            app_state.project_path_input.pop_back();
        }
        if (IsValidProject(app_state.project_path_input)) {
            app_state.project_path = app_state.project_path_input;
            // Load configuration from fabric.mod.json
            std::string temp_version, temp_description, temp_authors;
            if (!ReadModJson(app_state.project_path + "/src/main/resources/fabric.mod.json", app_state.config_mod_name, app_state.config_mod_id, temp_version, temp_description, temp_authors)) {
                app_state.status_message = modtoolbox::core::Localization::Get("warning_fabric_mod_json_not_found");
            } else {
                app_state.status_message = modtoolbox::core::Localization::Get("project_opened_successfully") + app_state.project_path + modtoolbox::core::Localization::Get("configuration_loaded");
            }
        } else {
            app_state.status_message = modtoolbox::core::Localization::Get("error_invalid_project_path");
        }
        app_state.project_path_input.clear();
        show_project_modal = false;
    };
    InputOption project_input_options;
    project_input_options.on_enter = on_project_submit;
    auto project_input = Input(&app_state.project_path_input, modtoolbox::core::Localization::Get("project_path_input_placeholder"), project_input_options);
    auto ok_button = Button(modtoolbox::core::Localization::Get("common_ok"), on_project_submit);
    auto cancel_button = Button(modtoolbox::core::Localization::Get("common_cancel"), [&] {
        app_state.project_path_input.clear();
        show_project_modal = false;
    });
    ftxui::Components project_modal_children = {
        project_input,
        Container::Horizontal({ok_button, cancel_button}),
    };
    auto project_modal_component = Container::Vertical(project_modal_children);

    // --- VIEW COMPONENTS ---
    auto main_menu = CreateMainMenu(switch_project_callback, to_build_view_callback, to_config_view_callback, to_tools_view_callback, quit_callback);
    auto build_menu = CreateBuildMenu(to_main_view_callback);
    auto config_menu = modtoolbox::ui::CreateConfigMenu(app_state, to_main_view_callback);
    auto tools_menu = CreateToolsMenu(to_texture_editor_callback, to_main_view_callback);
    auto texture_editor = CreateTextureEditor(on_texture_editor_enter);

    // --- MAIN CONTAINER ---
    ftxui::Components main_tabs = ftxui::Components{
        main_menu,
        config_menu,
        build_menu,
        tools_menu,
        texture_editor,
    };
    auto main_container = Container::Tab(main_tabs, (int*)&app_state.current_view);

    // --- RENDERER ---
    auto main_renderer = Renderer(main_container, [&] {
        Element current_view_renderer;
        switch (app_state.current_view) {
            case modtoolbox::core::View::Main:          current_view_renderer = main_menu->Render(); break;
            case modtoolbox::core::View::Configuration: current_view_renderer = config_menu->Render(); break;
            case modtoolbox::core::View::Build:         current_view_renderer = build_menu->Render(); break;
            case modtoolbox::core::View::Tools:         current_view_renderer = tools_menu->Render(); break;
            case modtoolbox::core::View::TextureEditor: current_view_renderer = texture_editor->Render(); break;
        }

        ftxui::Elements main_renderer_elements = {
            text(modtoolbox::core::Localization::Get("app_title")) | bold | hcenter,
            separator(),
            hbox(text(modtoolbox::core::Localization::Get("project_label")), text(app_state.project_path) | bold),
            separator(),
            current_view_renderer | flex,
            separator(),
            text(app_state.status_message) | dim
        };
        return vbox(main_renderer_elements) | border;
    });

    // --- MODAL RENDERERS ---
    auto project_modal_renderer = Renderer(project_modal_component, [&] {
        ftxui::Elements project_modal_elements = {
            text(modtoolbox::core::Localization::Get("modal_enter_new_project_path")),
            separator(),
            project_input->Render(),
            separator(),
            hbox(ok_button->Render(), filler(), cancel_button->Render())
        };
        return vbox(project_modal_elements) | size(WIDTH, GREATER_THAN, 40) | border | center;
    });

    auto texture_save_modal_renderer = Renderer(texture_save_modal_component, [&] {
        ftxui::Elements texture_save_modal_elements = {
            text(modtoolbox::core::Localization::Get("modal_enter_save_path_for_texture")),
            separator(),
            texture_save_input->Render(),
            separator(),
            hbox(texture_save_ok_button->Render(), filler(), texture_save_cancel_button->Render())
        };
        return vbox(texture_save_modal_elements) | border | center;
    });

    // --- LAYOUT & EVENT HANDLING ---
    auto layout = Modal(main_renderer, project_modal_renderer, &show_project_modal);
    layout = Modal(layout, texture_save_modal_renderer, &show_texture_save_modal);

    auto main_component = CatchEvent(layout, [&](Event event) {
        if (event == Event::Escape) {
            if (show_project_modal) {
                show_project_modal = false;
                app_state.project_path_input.clear();
                return true;
            }
            if (show_texture_save_modal) {
                show_texture_save_modal = false;
                app_state.texture_save_path_input.clear();
                return true;
            }
            if (app_state.current_view == modtoolbox::core::View::Main) {
                screen.Exit();
            } else if (app_state.current_view == modtoolbox::core::View::TextureEditor) {
                app_state.current_view = modtoolbox::core::View::Tools; // Go back to tools menu from editor
            } else {
                app_state.current_view = modtoolbox::core::View::Main; // Go back to main menu from other views
            }
            return true;
        }
        return false;
    });

    screen.Loop(main_component);

    return 0;
}
