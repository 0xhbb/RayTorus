#include "Renderer.h"
#include "Raytracer.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <cmath>

namespace {
    constexpr int width = 80;
    constexpr int height = 22;
    const std::string chars = ".,-~:;=!*#$@";
} // namespace

void setup_ansi_console() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
    std::cout << "\x1b[?25l";
    std::cout << "\x1b[2J";
}

void render_frame(float time_a, float time_b) {
    std::string frame;
    frame.reserve(static_cast<size_t>(width) * static_cast<size_t>(height) * 25);

    const int num_chars = static_cast<int>(chars.length());

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            const PixelResult res = trace_pixel(i, j, width, height, time_a, time_b);

            if (!res.hit) {
                frame += ' ';
            } else {
                int lum_idx = static_cast<int>(res.luminance * static_cast<float>(num_chars));
                if (lum_idx >= num_chars) lum_idx = num_chars - 1;
                if (lum_idx < 0) lum_idx = 0;

                const float hue = time_a * 2.0f + static_cast<float>(i) * 0.05f + static_cast<float>(j) * 0.1f;
                const float r_f = std::sin(hue) * 0.5f + 0.5f;
                const float g_f = std::sin(hue + 2.094f) * 0.5f + 0.5f;
                const float b_f = std::sin(hue + 4.188f) * 0.5f + 0.5f;

                const float intensity = res.luminance * 0.85f + 0.15f;

                const int r = static_cast<int>(r_f * intensity * 255.0f);
                const int g = static_cast<int>(g_f * intensity * 255.0f);
                const int b = static_cast<int>(b_f * intensity * 255.0f);

                frame += "\033[38;2;" + std::to_string(r) + ";" +
                         std::to_string(g) + ";" + std::to_string(b) + "m" + chars[static_cast<size_t>(lum_idx)];
            }
        }
        if (j < height - 1) frame += '\n';
    }

    std::cout << "\x1b[H" << frame << "\033[0m" << std::flush;
}
