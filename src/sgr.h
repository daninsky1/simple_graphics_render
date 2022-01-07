#pragma once


constexpr int MAX_WIN_STR_SIZE = 256;

enum class Mode {
    normal,
    insert,
    mark
};

namespace San {
    struct State {
        char window_name[MAX_WIN_STR_SIZE];
        Mode mode;
    };

    struct BkBuffer{
        // NOTE(daniel): Pixels are always 32-bits wide, memory order BB GG RR XX
        void* memory;
        int width;
        int height;
        int pitch;
        int bytes_per_pixel;
    };

    struct KeyboardInput {
        bool is_down;
        bool is_deadkey;
    };

    struct MouseInput {

    };

    struct Input {
        KeyboardInput keyboard;
        MouseInput mouse;
    };

    struct TextBuffer {
        char* tbuffer;
    };

    void init();

    void update_state();

    void render();
}



