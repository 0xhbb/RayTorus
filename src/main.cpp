#include "Renderer.h"
#include <chrono>
#include <thread>

int main() {
    setup_ansi_console();

    float A = 0.0f;
    float B = 0.0f;

    while (true) {
        render_frame(A, B);

        A += 0.04f;
        B += 0.02f;

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    return 0;
}
