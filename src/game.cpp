#include "game.hpp"
#include <iostream>
#include <ncurses.h>
#include <chrono>
#define COLOR_ORANGE 8
#define COLOR_GRAY 9


Map map; // Declare a global map object
Snake snake; // Declare a global snake object
Contents contents;
Gate gate;
enum Direction { UP = 0, DOWN, LEFT, RIGHT };

std::pair<int, int> poison_pos;
std::chrono::steady_clock::time_point poison_time;
int cur_len = 0, max_len = 0;
int growth_cnt = 0, poison_cnt = 0, gate_cnt = 0;
bool len_ok = false, growth_ok = false, poison_ok = false, gate_ok = false;

// 상태/미션 값 갱신 함수
void Game::update_contents_val() {
    cur_len = snake.get_body().size();
    if (cur_len > max_len) max_len = cur_len;

    // 미션 달성 조건 예시 (원하는 값으로 수정)
    len_ok = (cur_len >= 10);
    growth_ok = (growth_cnt >= 5);
    poison_ok = (poison_cnt >= 2);
    gate_ok = (gate_cnt >= 1);

    contents.set_status(cur_len, max_len, growth_cnt, poison_cnt, gate_cnt);
    contents.set_mission(10, 5, 2, 1, len_ok, growth_ok, poison_ok, gate_ok);
}



Game::Game(int map_index, int tick_ms)
    : snake(), map(23,23,map_index), is_running(false), score(0), tick_ms(tick_ms)
{
    // 필요시 추가 초기화 코드 작성
}

void Game::init_screen() {
    initscr();               // ncurses 화면 초기화
    start_color();           // 컬러 기능 활성화
    noecho();                // 입력 문자 화면에 표시 안 함
    cbreak();                // 라인 버퍼링 해제

    // 터미널이 컬러 지원하는지 확인
    if (!has_colors()) {
        endwin();
        std::cerr << "Your terminal does not support colors." << std::endl;
        exit(1);
    }

    // 터미널이 컬러 변경 지원 시, 회색 정의
    if (can_change_color()) {
        init_color(COLOR_GRAY, 500, 500, 500); // 회색 (R,G,B 50%)
        init_color(COLOR_ORANGE, 1000, 647, 0);
        init_color(COLOR_BLACK, 0, 0, 0); // 검정색
    }


    // 컬러 페어 초기화
    init_pair(1, COLOR_WHITE, COLOR_WHITE); // 빈 칸
    init_pair(2, COLOR_BLACK , COLOR_GRAY);  // 벽
    init_pair(3, COLOR_BLACK , COLOR_BLACK);  // 고정 벽
    init_pair(4, COLOR_BLACK , COLOR_YELLOW); // 뱀 머리
    init_pair(5, COLOR_BLACK , COLOR_ORANGE);  // 뱀 몸통
    init_pair(6, COLOR_BLACK , COLOR_GREEN);  // 사과
    init_pair(7, COLOR_BLACK, COLOR_RED);  // 독사과
    init_pair(8, COLOR_BLACK, COLOR_MAGENTA); // gate
    init_pair(9, COLOR_BLACK, COLOR_WHITE); // 검정글자, 흰배경 (글자 잘 보임)
    // 전체 배경색 지정
    bkgd(COLOR_PAIR(1));
}

void Game::update_screen() {
    clear(); // 화면 초기화

    // 맵 정보 가져오기
    auto mapData = map.getMapData();
    int rows = map.getHeight();
    int cols = map.getWidth();

    // 맵 출력
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int cell = mapData[i][j];
            char ch = ' ';
            int color = 1;

          
                switch (cell) {
                    case 0: ch = ' '; color = 1; break;
                    case 1: ch = ' '; color = 2; break;
                    case 2: ch = ' '; color = 3; break;
                    case 3: ch = ' '; color = 4; break;
                    case 4: ch = ' '; color = 5; break;
                    case 5: ch = ' '; color = 6; break;
                    case 6: ch = ' '; color = 7; break;
                    case 7: ch = ' '; color = 8; break; // 게이트
                    default: ch = ' '; color = 1; break;
                }
            

            attron(COLOR_PAIR(color));
            mvaddch(i, j * 2, ch);
            mvaddch(i, j * 2 + 1, ' ');
            attroff(COLOR_PAIR(color));
        }
    }
    // 상태 갱신 (현재 길이, 최대 길이, 성장, 독, 게이트 횟수)
    update_contents_val();

    // 스코어보드+미션 출력
    contents.draw(map.getWidth() * 2 + 2, 2);

    refresh(); // 화면 갱신
}

void Game::process_input() {
    int ch = getch();
    switch (ch) {
        case 'w':
        case 'W':
        case KEY_UP:
            snake.set_direction(UP);
            break;
        case 'a':
        case 'A':
        case KEY_LEFT:
            snake.set_direction(LEFT);
            break;
        case 's':
        case 'S':
        case KEY_DOWN:
            snake.set_direction(DOWN);
            break;
        case 'd':
        case 'D':
        case KEY_RIGHT:
            snake.set_direction(RIGHT);
            break;
        case 'q':
        case 'Q':
            is_running = false; // 게임 종료
            Quit = true;
            break;
        default:
            break;
    }
}

void Game::update_state() {
    // 현재 방향에 따라 dx, dy 결정
    int dx = 0, dy = 0;
    switch (snake.get_direction()) {
        case UP:    dx = -1; break;
        case DOWN:  dx = 1;  break;
        case LEFT:  dy = -1; break;
        case RIGHT: dy = 1;  break;
    }

    // 이전 뱀 위치를 맵에서 지움 (몸통, 머리 모두)
    for (const auto& part : snake.get_body()) {
        int x = part.first;
        int y = part.second;
        if (x >= 0 && x < map.getHeight() && y >= 0 && y < map.getWidth()) {
            if (map.getMapData()[x][y] == 3 || map.getMapData()[x][y] == 4)
                map.getMapData()[x][y] = 0; // 빈 칸으로
        }
    }

    // 뱀 이동
    snake.move(dx, dy);

    // 벽 충돌 검사
    auto head = snake.get_head();

    // 게이트 진입 체크
     if (gate.is_gate(head.first, head.second)) {
        // 현재 방향을 전달하여, 출구 앞 좌표와 새로운 방향을 받음
        auto result = gate.teleport_with_dir(
            head.first, head.second, snake.get_direction(), map.getMapData());

        // 뱀 머리 좌표를 출구 앞 좌표로 이동
        snake.set_head(result.first.first, result.first.second);

        // 뱀의 방향도 출구 방향으로 변경
        snake.set_direction(result.second);

        // 게이트 통과 횟수 증가 등 추가 처리
        gate_cnt++;
    }   
    if (head.first < 0 || head.first >= map.getHeight() ||
        head.second < 0 || head.second >= map.getWidth() ||
        map.getMapData()[head.first][head.second] == 1) // 1: 벽
    {
        is_running = false;
        snake.set_alive(false);
        return;
    }

    // 자기 몸과의 충돌 검사
    if (snake.check_collision()) {
        is_running = false;
        return;
    }

    // 아이템 먹었는지 검사
    bool ate_apple = false;
    bool ate_poison = false;
    if (map.getMapData()[head.first][head.second] == 5) { // 사과
        growth_cnt++;
        snake.grow();
        ate_apple = true;
        score++;
        mvprintw(0, 0, "사과 먹음! 위치: (%d, %d)   ", head.first, head.second);
    } else if (map.getMapData()[head.first][head.second] == 6) { // 독사과
        poison_cnt++;
        // 뱀의 길이가 1보다 크면 줄임
        if (snake.get_body().size() > 1) {
            snake.shrink(); // shrink()는 뱀의 길이를 1 줄이는 함수로 아래 참고
        }
        else{
            is_running = false;
            return;
        }
        ate_poison = true;
        mvprintw(0, 0, "독사과 먹음! 위치: (%d, %d)   ", head.first, head.second);
    }

    // 뱀의 새로운 위치를 맵에 표시 (머리: 3, 몸통: 4)
    const auto& body = snake.get_body();
    for (size_t i = 0; i < body.size(); ++i) {
        int x = body[i].first;
        int y = body[i].second;
        if (x >= 0 && x < map.getHeight() && y >= 0 && y < map.getWidth()) {
            if (i == 0)
                map.getMapData()[x][y] = 3; // 머리
            else
                map.getMapData()[x][y] = 4; // 몸통
        }
    }

    // 사과를 먹었다면, 다음 프레임에 새 사과 생성
    if (ate_apple) {
        int apple_x = -1, apple_y = -1;
        while (true) {
            int x = rand() % map.getWidth();
            int y = rand() % map.getHeight();
            if (map.getMapData()[x][y] == 0) {
                map.getMapData()[x][y] = 5; // 새 사과 생성
                apple_x = x;
                apple_y = y;
                break;
            }
        }
        mvprintw(1, 0, "새 사과 생성 위치: (%d, %d)   ", apple_x, apple_y);
    }

    // 독사과를 먹었다면, 다음 프레임에 새 독사과 생성
    if (ate_poison) {
       spawn_poison();
    }
}

bool Game::check_collision() {
    auto head = snake.get_head();

    // 벽 충돌 검사
    if (head.first < 0 || head.first >= map.getHeight() ||
        head.second < 0 || head.second >= map.getWidth() ||
        map.getMapData()[head.first][head.second] == 1) // 1: 벽
    {
        snake.set_alive(false);
        return true;
    }

    // 자기 몸과의 충돌 검사
    if (snake.check_collision()) {
        snake.set_alive(false);
        return true;
    }

    return false;
}

void Game::reset() {
    map.generate();
    snake = Snake(map);
    is_running = true;
    score = 0;

    // 사과(아이템) 생성: 1프레임 이후 맵의 빈 공간(0) 중 랜덤 위치에 5로 저장
    srand(static_cast<unsigned int>(time(nullptr)));
    while (true) {
        int x = rand() % (map.getWidth()-2);
        int y = rand() % (map.getHeight()-2);
        if (map.getMapData()[x+1][y+1] == 0) {
            map.getMapData()[x+1][y+1] = 5; // 사과 생성
            break;
        }
    }
    spawn_poison();
    gate.spawn(map.getMapData());

}

void Game::spawn_poison() {
    while (true) {
        int x = rand() % map.getWidth();
        int y = rand() % map.getHeight();
        if (map.getMapData()[x][y] == 0) {
            map.getMapData()[x][y] = 6;
            poison_pos = {x, y};
            poison_time = std::chrono::steady_clock::now();
            break;
        }
    }
}

void Game::game_over() {
    clear();
    mvprintw(map.getHeight() / 2, (map.getWidth() - 9), "GAME OVER");
    mvprintw(map.getHeight() / 2 + 1, (map.getWidth() - 15), "Press any key to exit...");

    // 최종 점수 정보 출력
    int info_y = map.getHeight() / 2 + 3;
    mvprintw(info_y++, (map.getWidth() - 20), "최대 길이: %d", max_len);
    mvprintw(info_y++, (map.getWidth() - 20), "B: %d / %d", cur_len, max_len);
    mvprintw(info_y++, (map.getWidth() - 20), "Growth Item: %d", growth_cnt);
    mvprintw(info_y++, (map.getWidth() - 20), "Poison Item: %d", poison_cnt);
    mvprintw(info_y++, (map.getWidth() - 20), "Gate 사용: %d", gate_cnt);

    // 게임 시간 출력
    //static auto start_time = std::chrono::steady_clock::now();
    auto end_time = std::chrono::steady_clock::now();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    mvprintw(info_y++, (map.getWidth() - 20), "게임 시간: %lld초", seconds);

    refresh();
    getch(); // 아무 키나 누를 때까지 대기
    endwin(); // ncurses 종료
    if(Quit) {
        std::cout << "강제종료\n";
    }
    std::cout << "최대 길이: " << max_len << std::endl;
    std::cout << "B: " << cur_len << " / " << max_len << std::endl;
    std::cout << "Growth Item: " << growth_cnt << std::endl;
    std::cout << "Poison Item: " << poison_cnt << std::endl;
    std::cout << "Gate 사용: " << gate_cnt << std::endl;
    std::cout << "게임 시간: " << seconds << "초" << std::endl;
}


bool Game::snake_alive() const {
    return snake.get_alive();
}
void Game::run() {
    reset();
    init_screen();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    start_time = std::chrono::steady_clock::now();
    is_running = true;
    while (is_running) {
        process_input();
        update_state();
        update_screen();

        // 디버깅: snake_alive 상태와 뱀 머리 좌표, alive 상태 출력
        //auto head = snake.get_head();
        //mvprintw(0, 0, "snake_alive: %d, head: (%d, %d), alive: %d   ",
        //         snake_alive(), head.first, head.second, snake.get_alive());
        refresh();

        if (!snake_alive()) {
            is_running = false;
            break;
        }
        if (len_ok && growth_ok && poison_ok && gate_ok) {
            update_screen(); // "Game clear!" 출력
            refresh();
            std::this_thread::sleep_for(std::chrono::seconds(5)); // 5초 대기
            is_running = false;
            break;
        }

        napms(tick_ms);
    }
    game_over();
}
// snake_alive()는 snake가 살아있는지 반환하는 함수로, 아래처럼 구현할 수 있습니다.
// bool Game::snake_alive() const { return snake.is_alive(); }