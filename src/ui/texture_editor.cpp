#include "texture_editor.hpp"
#include "core/app_state.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/elements.hpp"
#include "core/localization.hpp" // Added this include

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "vendor/stb_image_write.h"

#include <vector>

extern modtoolbox::core::AppState app_state;

struct PaletteEntry {
    ftxui::Color ftx_color;
    uint8_t r, g, b;
};

// A simple color palette with corresponding RGB values.
const std::vector<PaletteEntry> g_palette = {
    {ftxui::Color::Default, 0, 0, 0},         // 0: Transparent
    {ftxui::Color::Red, 255, 0, 0},           // 1
    {ftxui::Color::Green, 0, 255, 0},         // 2
    {ftxui::Color::Blue, 0, 0, 255},          // 3
    {ftxui::Color::Yellow, 255, 255, 0},      // 4
    {ftxui::Color::Cyan, 0, 255, 255},        // 5
    {ftxui::Color::Magenta, 255, 0, 255},     // 6
    {ftxui::Color::White, 255, 255, 255},     // 7
    {ftxui::Color::GrayDark, 85, 85, 85},     // 8
    {ftxui::Color::GrayLight, 170, 170, 170}, // 9
    {ftxui::Color::RGB(1, 1, 1), 1, 1, 1},    // 10: Black (off-black)
};

// Saves the current texture data from the app state to a PNG file.
void SaveTextureToPng(const std::string& path) {
    constexpr int width = 16;
    constexpr int height = 16;
    constexpr int channels = 4; // RGBA
    std::vector<uint8_t> image_data(width * height * channels);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int color_index = app_state.texture_pixels[y][x];
            const auto& palette_entry = (color_index >= 0 && color_index < static_cast<int>(g_palette.size())) ? g_palette[color_index] : g_palette[0];
            
            auto* pixel = &image_data[(y * width + x) * channels];
            pixel[0] = palette_entry.r;
            pixel[1] = palette_entry.g;
            pixel[2] = palette_entry.b;
            pixel[3] = (color_index == 0) ? 0 : 255; // Alpha channel for transparency
        }
    }

    stbi_write_png(path.c_str(), width, height, channels, image_data.data(), width * channels);
}

ftxui::Component CreateTextureEditor(std::function<void()> on_enter) {
    using namespace ftxui;

    class Editor : public ComponentBase {
    public:
        Editor(std::function<void()> on_enter_handler) : on_enter_handler_(on_enter_handler) {}
        bool Focusable() const override { return true; }

    private:
        Element OnRender() override {
            // Render the pixel grid
            std::vector<std::vector<Element>> rows;
            for (int y = 0; y < 16; ++y) {
                std::vector<Element> current_row;
                for (int x = 0; x < 16; ++x) {
                    int color_index = app_state.texture_pixels[y][x];
                    const auto& palette_entry = (color_index >= 0 && color_index < static_cast<int>(g_palette.size())) ? g_palette[color_index] : g_palette[0];
                    Element pixel = text("  ") | bgcolor(palette_entry.ftx_color);

                    if (x == app_state.texture_cursor_x && y == app_state.texture_cursor_y) {
                        pixel = pixel | inverted;
                    }
                    current_row.push_back(pixel);
                }
                rows.push_back(current_row);
            }
            auto pixel_grid = gridbox(rows);

            // Render the palette display
            Elements palette_elements;
            for(size_t i = 0; i < g_palette.size(); ++i) {
                Element color_box = text(" ") | bgcolor(g_palette[i].ftx_color);
                if (i == static_cast<size_t>(app_state.current_color_index)) {
                    color_box = color_box | border;
                } else {
                    color_box = color_box | borderEmpty;
                }
                palette_elements.push_back(color_box);
            }
            auto palette_display = hbox(palette_elements);

            return vbox({
                text(modtoolbox::core::Localization::Get("texture_editor_title")) | bold | hcenter,
                separator(),
                pixel_grid | center,
                separator(),
                palette_display | center,
                separator(),
                text(modtoolbox::core::Localization::Get("texture_editor_controls")) | dim | hcenter,
            });
        }

        bool OnEvent(Event event) override {
            if (event.is_mouse()) {
                return false;
            }

            if (event.is_character()) {
                std::string ch = event.character();
                if (ch >= "0" && ch <= "9") {
                    int color_index = std::stoi(ch);
                    if (color_index == 0) { // '0' key maps to the 10th color (black)
                        if (10 < g_palette.size()) {
                           app_state.current_color_index = 10;
                        }
                    } else { // '1' through '9' keys
                        if (static_cast<size_t>(color_index) < g_palette.size()) {
                           app_state.current_color_index = color_index;
                        }
                    }
                    return true;
                } else if (ch == "t") {
                    app_state.current_color_index = 0; // Transparent
                    return true;
                }
            }

            if (event == Event::ArrowUp) {
                app_state.texture_cursor_y = std::max(0, app_state.texture_cursor_y - 1);
                return true;
            }
            if (event == Event::ArrowDown) {
                app_state.texture_cursor_y = std::min(15, app_state.texture_cursor_y + 1);
                return true;
            }
            if (event == Event::ArrowLeft) {
                app_state.texture_cursor_x = std::max(0, app_state.texture_cursor_x - 1);
                return true;
            }
            if (event == Event::ArrowRight) {
                app_state.texture_cursor_x = std::min(15, app_state.texture_cursor_x + 1);
                return true;
            }
            if (event == Event::Character(' ')) {
                app_state.texture_pixels[app_state.texture_cursor_y][app_state.texture_cursor_x] = app_state.current_color_index;
                return true;
            }
            if (event == Event::Return) {
                on_enter_handler_();
                return true;
            }

            return false;
        }

        std::function<void()> on_enter_handler_;
    };

    return Make<Editor>(on_enter);
}
