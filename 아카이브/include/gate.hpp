#pragma once
#include <vector>
#include <utility>

class Gate {
public:
    void spawn(std::vector<std::vector<int>>& map);
    std::pair<int, int> get_entry() const;
    std::pair<int, int> get_exit() const;
    bool is_gate(int x, int y) const;
    // 진입 게이트 좌표, 진입 방향, 맵을 받아 출구 앞 좌표와 새로운 방향 반환
    std::pair<std::pair<int, int>, int> teleport_with_dir(
        int in_x, int in_y, int in_dir, const std::vector<std::vector<int>>& map) const;
    std::pair<int, int> teleport(int x, int y) const;
private:
    std::vector<std::pair<int, int>> wall_positions;
    std::pair<int, int> gate1, gate2;
    bool exists = false;
};