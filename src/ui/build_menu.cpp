#include "build_menu.hpp"
#include "core/app_state.hpp"
#include "core/gradle.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "core/localization.hpp" // Added this include
#include <thread>
#include <future>
#include <optional>

extern modtoolbox::core::AppState app_state;

ftxui::Component CreateBuildMenu(std::function<void()> on_back) {
    using namespace ftxui;

    auto run_task = [&](const std::string& task) {
        // Check if a task is already running
        if (app_state.current_gradle_task_future.has_value() &&
            app_state.current_gradle_task_future->wait_for(std::chrono::seconds(0)) == std::future_status::timeout) {
            app_state.status_message = modtoolbox::core::Localization::Get("build_error_task_running");
            return;
        }

        // If a previous task completed, clear its future
        if (app_state.current_gradle_task_future.has_value()) {
            app_state.current_gradle_task_future->get(); // Ensure it's finished and clear state
            app_state.current_gradle_task_future.reset();
        }

        app_state.is_gradle_task_running.store(true);
        app_state.build_output = modtoolbox::core::Localization::Get("build_output_running_task_prefix") + task + modtoolbox::core::Localization::Get("build_output_running_task_suffix");
        app_state.status_message = modtoolbox::core::Localization::Get("build_status_task_started") + task;

        ScreenInteractive* screen = ScreenInteractive::Active();

        // Launch the task asynchronously
        app_state.current_gradle_task_future = std::async(std::launch::async, [&, task, screen] {
            RunGradleTask(app_state.project_path, task, [&](const std::string& output_chunk) {
                screen->Post([&, output_chunk] {
                    app_state.build_output += output_chunk;
                    screen->RequestAnimationFrame();
                });
            });
            // Task finished, reset the flag and update status
            screen->Post([&] {
                app_state.is_gradle_task_running.store(false);
                app_state.status_message = modtoolbox::core::Localization::Get("build_status_task_finished") + task + modtoolbox::core::Localization::Get("build_status_finished");
                screen->RequestAnimationFrame();
            });
        });
    };

    auto build_button = Button(modtoolbox::core::Localization::Get("build_button_label"), [=] { run_task("build"); });
    auto decompile_button = Button(modtoolbox::core::Localization::Get("decompile_button_label"), [=] { run_task("genSources"); });
    auto run_client_button = Button(modtoolbox::core::Localization::Get("run_client_button_label"), [=] { run_task("runClient"); });
    auto clean_button = Button(modtoolbox::core::Localization::Get("clean_button_label"), [=] { run_task("clean"); });
    auto back_button = Button(modtoolbox::core::Localization::Get("common_back"), on_back);

    auto buttons = Container::Horizontal({
        build_button,
        decompile_button,
        run_client_button,
        clean_button,
        back_button,
    });

    auto output_renderer = Renderer([&] {
        // Wrap paragraph in vbox to help with scrolling
        return vbox(ftxui::Elements{paragraph(app_state.build_output)}) | flex | border;
    });

    return Container::Vertical(ftxui::Components{
        buttons,
        output_renderer | yframe,
    });
}