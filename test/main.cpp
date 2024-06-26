#include <gtest/gtest.h>
#include <windows.h>

#define IDM_OPEN 1
#define IDM_SAVE 2
#define IDM_EXIT 3
#define IDM_ABOUT 4

HWND test_window = nullptr; 
bool was_message_box_called = false; 

void send_command_message(UINT command_id) {
    SendMessage(test_window, WM_COMMAND, command_id, 0);
}

int WINAPI fake_message_box(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
    was_message_box_called = true;
    return 0; 
}

class MenuTests : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        WNDCLASS wc = {};
        wc.lpfnWndProc = DefWindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = "TestWindowClass";
        RegisterClass(&wc);

        test_window = CreateWindow(wc.lpszClassName, "Test Window", 0, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
        ASSERT_TRUE(test_window != nullptr);
    }

    static void TearDownTestSuite() {
        DestroyWindow(test_window);
        test_window = nullptr;
    }

    void SetUp() override {
        was_message_box_called = false;
    }
};

TEST_F(MenuTests, ExitMenuCommand) {
    send_command_message(IDM_EXIT);
    EXPECT_TRUE(true);
}

int WINAPI MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

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

    EXPECT_FLOAT_EQ(p, 0.5f);
    EXPECT_FLOAT_EQ(dp, 1.0f);
}

TEST(SimulatePlayerTest, NegativeAcceleration) {
    float p = 0.0f;
    float dp = 0.0f;
    float ddp = -1.0f;
    float dt = 1.0f;

    simulate_player(&p, &dp, ddp, dt);

    EXPECT_FLOAT_EQ(p, -0.5f);
    EXPECT_FLOAT_EQ(dp, -1.0f);
}

TEST(SimulatePlayerTest, BoundaryConditionTop) {
    float p = 9.5f;
    float dp = 1.0f;
    float ddp = 0.0f;
    float dt = 1.0f;

    simulate_player(&p, &dp, ddp, dt);

    EXPECT_FLOAT_EQ(p, (arena_half_size_y + player_half_size_y) / 2);
    EXPECT_FLOAT_EQ(dp, -9.0f);
}

TEST(SimulatePlayerTest, BoundaryConditionBottom) {
    float p = -9.5f;
    float dp = -1.0f;
    float ddp = 0.0f;
    float dt = 1.0f;

    simulate_player(&p, &dp, ddp, dt);

    EXPECT_FLOAT_EQ(p, -(arena_half_size_y + player_half_size_y) / 2);
    EXPECT_FLOAT_EQ(dp, 9.0f);
}

static bool running = true;

struct Render_State {
    int height, width;
    void* memory;

    BITMAPINFO bitmap_info;
};

static Render_State render_state;

void clear_screen(const unsigned int color) {
    auto* pixel = static_cast<unsigned int*>(render_state.memory);
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width; x++) {
            *pixel++ = color;
        }
    }
}

TEST(RendererTest, ClearScreen) {
    constexpr unsigned int color = 123;
    clear_screen(color);
    const auto* pixel = static_cast<unsigned int*>(render_state.memory);
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width; x++) {
            EXPECT_EQ(*pixel++, color);
        }
    }
}

TEST(RendererTest, ClearScreenDifferentColor) {
    constexpr unsigned int color1 = 123;
    constexpr unsigned int color2 = 456;

    clear_screen(color1);
    clear_screen(color2);

    const auto* pixel = static_cast<unsigned int*>(render_state.memory);
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width; x++) {
            EXPECT_EQ(*pixel++, color2);
        }
    }
}

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
    HWND hwnd = nullptr;
    UINT uMsg = WM_CLOSE;
    WPARAM wParam = 0;
    LPARAM lParam = 0;

    LRESULT result = window_callback(hwnd, uMsg, wParam, lParam);

    ASSERT_FALSE(running);
    ASSERT_EQ(result, 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
