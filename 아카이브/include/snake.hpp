#pragma once

#include <vector>
#include <utility>
#include "map.hpp"


class Snake{
    public:
        Snake() : direction(0), length(1), score(0), alive(true) {} // 기본 생성자
        Snake(int x, int y);
        Snake(const Map& map);
        void move(int dx, int dy);
        void grow();
        bool check_collision();
        void set_direction(int dir);
        int get_direction() const { return direction; } // 현재 방향 반환
        std::pair<int, int> get_head() const { return body.front(); } // 머리 좌표 반환
        void set_alive(bool val) { alive = val; }
        bool get_alive() const { return alive; } // 생존 여부 반환
        const std::vector<std::pair<int, int>>& get_body() const { return body; }
        void shrink();

        void set_head(int x, int y);

        // 기타 뱀 관련 메소드
    private:
        std::vector<std::pair<int, int>> body; // 뱀의 몸통 좌표
        int direction; // 현재 방향
        int length; // 뱀의 길이
        int score; // 점수
        bool alive; // 생존 여부
        // 기타 상태 변수
};