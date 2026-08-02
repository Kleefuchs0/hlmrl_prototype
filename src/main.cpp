#include <raylib.h>

void game_loop() {
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
        EndDrawing();
    }
}

int main() {
    InitWindow(640, 360, "hlmrl");
    game_loop();
    CloseWindow();
    return 0;
}
