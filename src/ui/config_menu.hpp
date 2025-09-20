#pragma once
#include "ftxui/component/component.hpp"
#include "core/app_state.hpp"
#include <functional>

namespace modtoolbox::ui {

ftxui::Component CreateConfigMenu(modtoolbox::core::AppState& app_state, std::function<void()> on_back);

} // namespace modtoolbox::ui