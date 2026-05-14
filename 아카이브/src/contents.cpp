#include "contents.hpp"
#include <ncurses.h>

void Contents::set_status(int cur_len, int max_len, int growth, int poison, int gate) {
    cur_length = cur_len;
    max_length = max_len;
    growth_cnt = growth;
    poison_cnt = poison;
    gate_cnt = gate;
}

void Contents::set_mission(int m_len, int m_growth, int m_poison, int m_gate,
                           bool len_ok, bool growth_ok, bool poison_ok, bool gate_ok) {
    mission_len = m_len;
    mission_growth = m_growth;
    mission_poison = m_poison;
    mission_gate = m_gate;
    mission_len_ok = len_ok;
    mission_growth_ok = growth_ok;
    mission_poison_ok = poison_ok;
    mission_gate_ok = gate_ok;
}

void Contents::draw(int x, int y) const {
    attron(COLOR_PAIR(9)); // 검정글자+흰배경

    // Score Board
    mvprintw(y, x,   "Score Board");
    mvprintw(y+1, x, "B: %d / %d", cur_length, max_length);
    mvprintw(y+2, x, "+: %d", growth_cnt);
    mvprintw(y+3, x, "-: %d", poison_cnt);
    mvprintw(y+4, x, "G: %d", gate_cnt);

    // Mission
    mvprintw(y+6, x, "Mission");
    mvprintw(y+7, x, "B: %d (%s)", mission_len, mission_len_ok ? "v" : " ");
    mvprintw(y+8, x, "+: %d (%s)", mission_growth, mission_growth_ok ? "v" : " ");
    mvprintw(y+9, x, "-: %d (%s)", mission_poison, mission_poison_ok ? "v" : " ");
    mvprintw(y+10, x, "G: %d (%s)", mission_gate, mission_gate_ok ? "v" : " ");

    // 모든 미션 달성 시 Game clear 출력
    if (mission_len_ok && mission_growth_ok && mission_poison_ok && mission_gate_ok) {
        mvprintw(y+12, x, "Game clear!");
    }

    attroff(COLOR_PAIR(9));
}