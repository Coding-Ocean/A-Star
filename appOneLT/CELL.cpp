#include"graphic.h"
#include"mathUtil.h"
#include "CELL.h"

CELL::CELL()
    :x(0)
    ,y(0)
{
}

void CELL::set(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->cost = 0;
    this->score = 0;
    this->status = NO_CHECK;
    this->parentDirIdx = 0;
}

void CELL::draw()
{
    colorMode(HSV,100);
    angleMode(DEGREES);
    float satu = 100; float value = 50;
    if      (status == NO_CHECK) fill(0, 0, 50);
    else if (status == CLOSED) fill(-10, satu, 50);
    else if (status == OPENED) fill(80, satu, value);
    else if (status == OBSTACLE) fill(0,satu,0);
    else if (status == PATH) fill(-10, satu, 50); //fill(50, satu, value);
    noStroke();
    rect(w * x, h * y, w - 1, h - 1);
    //info
    if (status == OPENED) 
    {
        fill(255);
        textSize(w/3);
        textMode(BCENTER);
        int px = w * x + w / 5;
        int py = h * y + h / 2;
        text(cost, px, py - w/6);
        text(score, px, py + w/6);
    }
}

void CELL::drawPathLine(DIR* dir) 
{
    if (status == PATH) {
        strokeWeight(5);
        stroke(330, 80, 100);
        int ex = x * w + w / 2;
        int ey = y * h + h / 2;
        int sx = ex + dir[parentDirIdx].x * w;
        int sy = ey + dir[parentDirIdx].y * h;
        arrow(sx, sy, ex, ey);
    }
}
