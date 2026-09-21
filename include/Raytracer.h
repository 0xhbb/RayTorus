#pragma once

struct PixelResult {
    bool hit;
    float luminance;
};

[[nodiscard]] PixelResult trace_pixel(int x, int y, int width, int height, float time_a, float time_b);
