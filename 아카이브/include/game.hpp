#pragma once
#include "snake.hpp"
#include "map.hpp"
#include "contents.hpp"
#include <gate.hpp>
// game.hpp에 멤버 변수 추가
#include <chrono>
#include <thread>
extern std::pair<int, int> poison_pos;
extern std::chrono::steady_clock::time_point poison_time;

class Game {
public:
    Game(int map_index = 0, int tick_ms = 150);
    void run();

private:
    int tick_ms = 150;
    void init_screen();         // 화면 초기화
    void update_screen();       // 화면 갱신
    void process_input();       // 사용자 입력 처리
    void update_state();        // 게임 상태 갱신
    bool check_collision();     // 충돌 검사
    //void spawn_item();          // 아이템 생성
    void spawn_poison();
    void reset();               // 게임 리셋
    void game_over();           // 게임 오버 처리
    //void show_score();          // 점수 표시
    bool snake_alive() const; // 뱀의 생존 여부 확인
    void update_contents_val();
    Snake snake;
    Map map;
    bool is_running;
    bool Quit=false;
    int score;
    std::chrono::steady_clock::time_point start_time;
    
    // 기타 상태 변수
};




