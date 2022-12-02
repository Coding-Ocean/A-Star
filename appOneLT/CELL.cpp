#include"graphic.h"
#include"mathUtil.h"
#include "CELL.h"

void CELL::create(int x, int y)
{
    this->x = x;
    this->y = y;
}

void CELL::init(int mapData)
{
    if (mapData == 1)
        status = OBSTACLE;
    else
        status = NO_CHECK;
    cost = 0;
    heuristic = 0;
    score = 0;
    parentDirIdx = 0;
}

void CELL::draw(int sideLen)
{
    colorMode(HSV, 100);
    angleMode(DEGREES);
    float satu = 100; float value = 50;
    if      (status == NO_CHECK) fill(0, 0, 50);
    else if (status == OBSTACLE) fill(0, 0, 0);
    else if (status == CLOSED) fill(350, satu, value);
    else if (status == OPENED) fill(80, satu, value);
    noStroke();
    rect(sideLen * x, sideLen * y, sideLen - 1, sideLen - 1);
    //info
    if (status == OPENED)
    {
        textSize(sideLen / 3);
        textMode(BCENTER);
        int px = sideLen * x + sideLen / 5;
        int py = sideLen * y + sideLen / 2;
        fill(300, 100, 100);
        text(cost, px, py - sideLen / 6);
        fill(180, 100, 100);
        text(heuristic, px+sideLen/2, py - sideLen / 6);
        fill(60, 100, 100);
        text(score, px+sideLen/4, py + sideLen / 5);
    }
}


void CELL::drawMinimal(int sideLen)
{
    colorMode(HSV, 100);
    angleMode(DEGREES);
    if (status == OBSTACLE) fill(0, 0, 0);
    else fill(0, 0, 50);
    noStroke();
    rect(sideLen * x, sideLen * y, sideLen - 1, sideLen - 1);
}
