#include <iostream>
#include "game.hpp"

int main(int argc, char* argv[]) {
    int map_index = 0;
    int tick_ms = 150; // 기본값: 150ms

    if (argc > 1) {
        map_index = std::atoi(argv[1]);
        if (map_index < 0) map_index = 0;
    }
    if (argc > 2) {
        tick_ms = std::atoi(argv[2]) * 50; // 예: 3이면 150ms, 1이면 50ms
        if (tick_ms < 10) tick_ms = 10;
    }

    Game game(map_index, tick_ms);
    game.run();
    return 0;
}