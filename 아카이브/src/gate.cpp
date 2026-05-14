#include "gate.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};

// 게이트 방향 정보 저장용
struct GateInfo {
    std::pair<int, int> pos;
    bool is_edge;
    int edge_dir; // 0=UP, 1=DOWN, 2=LEFT, 3=RIGHT, -1=내부
};

GateInfo gate_info1, gate_info2;

void Gate::spawn(std::vector<std::vector<int>>& map) {
    wall_positions.clear();
    int rows = map.size();
    int cols = map[0].size();

    // 벽(1) 위치 수집
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (map[i][j] == 1) {
                wall_positions.push_back({i, j});
            }
        }
    }

    if (wall_positions.size() < 2) return;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int idx1 = std::rand() % wall_positions.size();
    int idx2;
    do {
        idx2 = std::rand() % wall_positions.size();
    } while (idx2 == idx1);

    gate1 = wall_positions[idx1];
    gate2 = wall_positions[idx2];
    exists = true;
    map[gate1.first][gate1.second] = 7;
    map[gate2.first][gate2.second] = 7;

    // 게이트1 정보
    gate_info1.pos = gate1;
    gate_info1.is_edge = false;
    gate_info1.edge_dir = -1;
    if (gate1.first == 0) { gate_info1.is_edge = true; gate_info1.edge_dir = 1; }
    else if (gate1.first == rows - 1) { gate_info1.is_edge = true; gate_info1.edge_dir = 0; }
    else if (gate1.second == 0) { gate_info1.is_edge = true; gate_info1.edge_dir = 3; }
    else if (gate1.second == cols - 1) { gate_info1.is_edge = true; gate_info1.edge_dir = 2; }

    // 게이트2 정보
    gate_info2.pos = gate2;
    gate_info2.is_edge = false;
    gate_info2.edge_dir = -1;
    if (gate2.first == 0) { gate_info2.is_edge = true; gate_info2.edge_dir = 1; }
    else if (gate2.first == rows - 1) { gate_info2.is_edge = true; gate_info2.edge_dir = 0; }
    else if (gate2.second == 0) { gate_info2.is_edge = true; gate_info2.edge_dir = 3; }
    else if (gate2.second == cols - 1) { gate_info2.is_edge = true; gate_info2.edge_dir = 2; }
}

std::pair<int, int> Gate::get_entry() const {
    return gate1;
}

std::pair<int, int> Gate::get_exit() const {
    return gate2;
}

bool Gate::is_gate(int x, int y) const {
    return (exists && ((gate1.first == x && gate1.second == y) || (gate2.first == x && gate2.second == y)));
}


std::pair<std::pair<int, int>, int> Gate::teleport_with_dir(
    int in_x, int in_y, int in_dir, const std::vector<std::vector<int>>& map) const
{
    // 출구 게이트 정보 선택
    GateInfo out_info;
    if (gate1.first == in_x && gate1.second == in_y)
        out_info = gate_info2;
    else
        out_info = gate_info1;

    int rows = map.size();
    int cols = map[0].size();
    int gx = out_info.pos.first;
    int gy = out_info.pos.second;

    // 4방향 모두 검사해서 나올 수 있는 곳이 있으면 그 자리와 방향으로 나감
    for (int dir = 0; dir < 4; ++dir) {
        int nx = gx + dx[dir];
        int ny = gy + dy[dir];
        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
            int cell = map[nx][ny];
            // 벽, 뱀 몸통/머리, 게이트가 아니면 이동 가능
            if (cell != 1 && cell != 3 && cell != 4 && cell != 7) {
                return {{nx, ny}, dir};
            }
        }
    }
    // 모두 막혀있으면 게이트 좌표와 기존 방향 반환 (게임오버 처리)
    return {out_info.pos, in_dir};
}

std::pair<int, int> Gate::teleport(int x, int y) const {
    if (gate1.first == x && gate1.second == y)
        return gate2;
    else if (gate2.first == x && gate2.second == y)
        return gate1;
    else
        return {x, y}; // 게이트가 아니면 그대로 반환
}