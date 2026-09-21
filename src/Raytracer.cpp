#include "Raytracer.h"
#include "Vec3.h"
#include <algorithm>
#include <cmath>

namespace {
    [[nodiscard]] float sd_torus(const Vec3& p, float r1, float r2) {
        const float len_xz = std::sqrt(p.x * p.x + p.z * p.z);
        const float qx = len_xz - r1;
        return std::sqrt(qx * qx + p.y * p.y) - r2;
    }

    [[nodiscard]] Vec3 rotate(const Vec3& p, float a, float b) {
        const float cosB = std::cos(b), sinB = std::sin(b);
        const float x1 = p.x * cosB - p.y * sinB;
        const float y1 = p.x * sinB + p.y * cosB;
        const float z1 = p.z;

        const float cosA = std::cos(a), sinA = std::sin(a);
        const float x2 = x1;
        const float y2 = y1 * cosA - z1 * sinA;
        const float z2 = y1 * sinA + z1 * cosA;

        return {x2, y2, z2};
    }

    [[nodiscard]] float map_scene(const Vec3& p, float a, float b) {
        const Vec3 pr = rotate(p, a, b);
        return sd_torus(pr, 1.2f, 0.5f);
    }

    [[nodiscard]] Vec3 estimate_normal(const Vec3& p, float a, float b) {
        constexpr float eps = 0.001f;
        const float nx = map_scene({p.x + eps, p.y, p.z}, a, b) - map_scene({p.x - eps, p.y, p.z}, a, b);
        const float ny = map_scene({p.x, p.y + eps, p.z}, a, b) - map_scene({p.x, p.y - eps, p.z}, a, b);
        const float nz = map_scene({p.x, p.y, p.z + eps}, a, b) - map_scene({p.x, p.y, p.z - eps}, a, b);
        return Vec3(nx, ny, nz).normalize();
    }
} // namespace

PixelResult trace_pixel(int x, int y, int width, int height, float time_a, float time_b) {
    const float uv_x = (static_cast<float>(x) / static_cast<float>(width) * 2.0f - 1.0f) *
                       (static_cast<float>(width) / static_cast<float>(height)) * 0.5f;
    const float uv_y = -(static_cast<float>(y) / static_cast<float>(height) * 2.0f - 1.0f);

    const Vec3 ro(0.0f, 0.0f, -4.0f);
    const Vec3 rd = Vec3(uv_x, uv_y, 1.0f).normalize();

    float t = 0.0f;
    constexpr float max_d = 10.0f;
    constexpr int max_steps = 64;

    for (int i = 0; i < max_steps; ++i) {
        const Vec3 p = ro + rd * t;
        const float d = map_scene(p, time_a, time_b);

        if (d < 0.001f) {
            const Vec3 n = estimate_normal(p, time_a, time_b);
            const Vec3 light_dir = Vec3(0.5f, 1.0f, -1.0f).normalize();

            const float lum = std::max(0.0f, n.dot(light_dir));
            return {true, lum};
        }
        if (t > max_d) break;

        t += d;
    }
    return {false, 0.0f};
}
