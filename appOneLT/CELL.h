#pragma once
const int NO_CHECK = 0;
const int OPENED = 1;
const int CLOSED = 2;
const int OBSTACLE = 3;
struct CELL {
public:
    int x;
    int y;
    int status;
    int cost;
    int heuristic;
    int score;
    int parentDirIdx;

    void draw(int sideLen);
    void drawMinimal(int sideLen);
};
