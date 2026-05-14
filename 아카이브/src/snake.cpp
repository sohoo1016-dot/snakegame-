#include "snake.hpp"
#include <vector>
#include <utility>

enum Direction { UP = 0, DOWN, LEFT, RIGHT };

Snake::Snake(int x, int y) {
    body.clear();
    body.push_back({x, y}); // 머리 좌표 (행, 열)
    direction = RIGHT;
    length = 1;
    score = 0;
    alive = true;
}

// 맵에서 3(머리), 4(몸통)인 좌표를 찾아 뱀을 초기화
Snake::Snake(const Map& map) {
    body.clear();
    for (int x = 0; x < map.getWidth(); ++x) {
        for (int y = 0; y < map.getHeight(); ++y) {
            int cell = map.getMapData()[x][y];
            if (cell == 3) {
                body.insert(body.begin(), {x, y}); // 머리는 맨 앞에
            } else if (cell == 4) {
                body.push_back({x, y}); // 몸통은 뒤에
            }
        }
    }
    direction = LEFT; // 기본 방향
    length = body.size();
    score = 0;
    alive = true;
}


void Snake::set_direction(int dir) {
    // 반대 방향으로는 이동 불가
    if ((direction == UP && dir == DOWN) ||
        (direction == DOWN && dir == UP) ||
        (direction == LEFT && dir == RIGHT) ||
        (direction == RIGHT && dir == LEFT)) {
        alive = false;
        return;
    }
    direction = dir;
}

void Snake::move(int dx, int dy) {
    if (!alive) return;
    // 새 머리 좌표 계산
    int new_x = body.front().first + dx;
    int new_y = body.front().second + dy;
    body.insert(body.begin(), {new_x, new_y});
    // 꼬리 삭제 (grow 시에는 삭제하지 않음)
    if ((int)body.size() > length)
        body.pop_back();
}

void Snake::grow() {
    length++;
    score += 10;
    // grow 시 move에서 꼬리를 삭제하지 않으므로 자동으로 길어짐
}

bool Snake::check_collision() {
    // 머리가 몸통과 부딪히는지 검사
    auto head = body.front();
    for (size_t i = 1; i < body.size(); ++i) {
        if (body[i] == head) {
            alive = false;
            return true;
        }
    }
    return false;
}

// snake.cpp
void Snake::shrink() {
    if (body.size() > 1) {
        body.pop_back();
        length--;
        // 길이가 3보다 작아지면 게임 오버
        if (length < 3) {
            alive = false;
        }
    }
}
void Snake::set_head(int x, int y) {
    if (!body.empty()) {
        body[0] = {x, y};
    }
}