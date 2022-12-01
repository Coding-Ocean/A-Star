#pragma once
const int NO_CHECK = 0;
const int OPENED = 1;
const int CLOSED = 2;
const int OBSTACLE = 3;
class CELL {
public:
    int Col;
    int Row;
    int Status;
    int Cost;
    int Score;
    int ParentDirIdx;

    void create(int col, int row);
    void init(int mapData);
    void draw(int sideLen);
    void drawMinimal(int sideLen);
};
