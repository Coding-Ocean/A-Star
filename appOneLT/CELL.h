#pragma once
const int NO_CHECK = 0;
const int OPENED = 1;
const int CLOSED = 2;
const int OBSTACLE = 3;
const int PATH = 4;
struct DIR {
    int x, y;
};
class CELL {
public:
    int x;
    int y;
    int w;
    int h;
    int cost;
    int score;
    int status;
    int parentDirIdx;

    CELL();
    void set(int x, int y, int w, int h);
    void draw();
    void drawPathLine(DIR* dir);
};
