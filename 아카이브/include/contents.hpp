#pragma once
#include <string>

class Contents {
public:
    void set_status(int cur_len, int max_len, int growth, int poison, int gate);
    void set_mission(int m_len, int m_growth, int m_poison, int m_gate,
                     bool len_ok, bool growth_ok, bool poison_ok, bool gate_ok);
    void draw(int x, int y) const;
private:
    // Score Board
    int cur_length = 0, max_length = 0;
    int growth_cnt = 0, poison_cnt = 0, gate_cnt = 0;
    // Mission
    int mission_len = 0, mission_growth = 0, mission_poison = 0, mission_gate = 0;
    bool mission_len_ok = false, mission_growth_ok = false, mission_poison_ok = false, mission_gate_ok = false;
};