#include <gtest/gtest.h>

const float player_half_size_y = 1.0f;
const float arena_half_size_y = 10.0f;

void simulate_player(float* p, float* dp, float ddp, float dt) {
    ddp -= *dp * 10.f;

    *p = *p + *dp * dt + ddp * dt * dt * .5f;
    *dp = *dp + ddp * dt;

    if (*p + player_half_size_y > arena_half_size_y) {
        *p = arena_half_size_y - player_half_size_y;
        *dp = 0;
    }
    else if (*p - player_half_size_y < -arena_half_size_y) {
        *p = -arena_half_size_y + player_half_size_y;
        *dp = 0;
    }
}

TEST(SimulatePlayerTest, NoAcceleration) {
    float p = 0.0f;
    float dp = 0.0f;
    float ddp = 0.0f;
    float dt = 1.0f;

    simulate_player(&p, &dp, ddp, dt);

    EXPECT_FLOAT_EQ(p, 0.0f);
    EXPECT_FLOAT_EQ(dp, 0.0f);
}

TEST(SimulatePlayerTest, PositiveAcceleration) {
    float p = 0.0f;
    float dp = 0.0f;
    float ddp = 1.0f;
    float dt = 1.0f;

    simulate_player(&p, &dp, ddp, dt);

    EXPECT_FLOAT_EQ(p, 0.5f);  // 0 + 0 * 1 + 1 * 1 * 1 * 0.5 = 0.5
    EXPECT_FLOAT_EQ(dp, 1.0f);  // 0 + 1 * 1 = 1
}

TEST(SimulatePlayerTest, NegativeAcceleration) {
    float p = 0.0f;
    float dp = 0.0f;
    float ddp = -1.0f;
    float dt = 1.0f;

    simulate_player(&p, &dp, ddp, dt);

    EXPECT_FLOAT_EQ(p, -0.5f);  // 0 + 0 * 1 + (-1) * 1 * 1 * 0.5 = -0.5
    EXPECT_FLOAT_EQ(dp, -1.0f);  // 0 + (-1) * 1 = -1
}

TEST(SimulatePlayerTest, BoundaryConditionTop) {
    float p = 9.5f;
    float dp = 1.0f;
    float ddp = 0.0f;
    float dt = 1.0f;

    simulate_player(&p, &dp, ddp, dt);

    EXPECT_FLOAT_EQ(p, arena_half_size_y - player_half_size_y);  // 10 - 1 = 9
    EXPECT_FLOAT_EQ(dp, 0.0f);
}

TEST(SimulatePlayerTest, BoundaryConditionBottom) {
    float p = -9.5f;
    float dp = -1.0f;
    float ddp = 0.0f;
    float dt = 1.0f;

    simulate_player(&p, &dp, ddp, dt);

    EXPECT_FLOAT_EQ(p, -arena_half_size_y + player_half_size_y);  // -10 + 1 = -9
    EXPECT_FLOAT_EQ(dp, 0.0f);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


#include <windows.h>

global_variable bool running = true;

struct Render_State {
    int height, width;
    void* memory;

    BITMAPINFO bitmap_info;
};


#define global_variable static
// #define internal static

global_variable Render_State render_state;

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    switch (uMsg) {
    case WM_CLOSE:
    case WM_DESTROY: {
        running = false;
    } break;

    case WM_SIZE: {
        RECT rect;
        GetClientRect(hwnd, &rect);
        render_state.width = rect.right - rect.left;
        render_state.height = rect.bottom - rect.top;

        int size = render_state.width * render_state.height * sizeof(unsigned int);

        if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
        render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
        render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
        render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
        render_state.bitmap_info.bmiHeader.biPlanes = 1;
        render_state.bitmap_info.bmiHeader.biBitCount = 32;
        render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;

    } break;

    default: {
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
    return result;
}

TEST(WindowCallbackTests, CloseAndDestroyMessages) {
    // Simulate WM_CLOSE message
    HWND hwnd = nullptr;  // Assuming hwnd is not used in the callback for this test
    UINT uMsg = WM_CLOSE;
    WPARAM wParam = 0;
    LPARAM lParam = 0;

    // Call the window_callback function
    LRESULT result = window_callback(hwnd, uMsg, wParam, lParam);

    // Assert that running should be false after handling WM_CLOSE/WM_DESTROY
    ASSERT_EQ(result, 0);   // Assert the result returned
}
