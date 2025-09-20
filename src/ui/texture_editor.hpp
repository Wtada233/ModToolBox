#pragma once

#include "ftxui/component/component.hpp"
#include <functional>
#include <string>

// Creates the texture editor component.
// on_enter is a callback to be executed when the user presses enter to save.
ftxui::Component CreateTextureEditor(std::function<void()> on_enter);

// Saves the current texture data from the app state to a PNG file.
void SaveTextureToPng(const std::string& path);
