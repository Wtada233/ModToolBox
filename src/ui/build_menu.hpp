#pragma once
#include "ftxui/component/component.hpp"
#include <functional>

// Creates the build menu component.
ftxui::Component CreateBuildMenu(std::function<void()> on_back);
