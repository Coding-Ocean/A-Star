#include"graphic.h"
#include"mathUtil.h"
#include "CELL.h"

void CELL::create(int col, int row)
{
    Col = col;
    Row = row;
}

void CELL::init(int mapData)
{
    if (mapData == 1)
        Status = OBSTACLE;
    else
        Status = NO_CHECK;
    Cost = 0;
    Score = 0;
    ParentDirIdx = 0;
}

void CELL::draw(int sideLen)
{
    colorMode(HSV, 100);
    angleMode(DEGREES);
    float satu = 100; float value = 50;
    if      (Status == NO_CHECK) fill(0, 0, 50);
    else if (Status == OBSTACLE) fill(0, 0, 0);
    else if (Status == CLOSED) fill(350, satu, value);
    else if (Status == OPENED) fill(80, satu, value);
    noStroke();
    rect(sideLen * Col, sideLen * Row, sideLen - 1, sideLen - 1);
    //info
    if (Status == OPENED)
    {
        fill(0, 0, 100);
        textSize(sideLen / 3);
        textMode(BCENTER);
        int px = sideLen * Col + sideLen / 5;
        int py = sideLen * Row + sideLen / 2;
        text(Cost, px, py - sideLen / 6);
        text(Score, px, py + sideLen / 6);
    }
}


void CELL::drawMinimal(int sideLen)
{
    colorMode(HSV, 100);
    angleMode(DEGREES);
    if (Status == OBSTACLE) fill(0, 0, 0);
    else fill(0, 0, 50);
    noStroke();
    rect(sideLen * Col, sideLen * Row, sideLen - 1, sideLen - 1);
}
