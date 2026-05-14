#pragma once
#include <vector>

class Map {
public:
    Map(int width = 23, int height = 23, int map_index = 0);
    const std::vector<std::vector<int>>& getMapData() const;
    void generate(); // 맵 생성 함수
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    // map.hpp
    std::vector<std::vector<int>>& getMapData() { return mapData; }
    //const std::vector<std::vector<int>>& getMapData() const { return mapData; }

private:
    int width, height;
    int map_index;
    std::vector<std::vector<int>> mapData;
};
