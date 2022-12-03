#include<vector>
#include"libOne.h"
#include"CELL.h"
#include"PATH_FINDER.h"

int MapData[100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void triangle(int sx, int sy, int gx, int gy) {
    strokeWeight(3);
    stroke(180, 100, 100);
    line(100 * sx, 100 * sy, 100 * gx, 100 * gy);
}

void gmain()
{
	window(1000, 1000);

    PATH_FINDER path_finder;
    path_finder.create(10,10);

    int sx=2, sy=1, gx=4, gy=8;
    std::vector<int> pathDirIdxs;
    path_finder.init(MapData, sx, sy, gx, gy, &pathDirIdxs);
    
    while (notQuit)
	{
        if (isTrigger(KEY_Q)) {
            path_finder.setRandomPos(sx, sy);
            path_finder.setRandomPos(gx, gy);
            path_finder.init(MapData, sx, sy, gx, gy, &pathDirIdxs);
        }

        if (isTrigger(KEY_E)) {
            sx = 2; sy = 1; gx = 4; gy = 8;
            path_finder.init(MapData, sx, sy, gx, gy, &pathDirIdxs);
        }

        if (isTrigger(KEY_A))path_finder.searchLoop();
        if (isTrigger(KEY_D))path_finder.searchStep();
        path_finder.drawCells();

        static int sw = 0;
        if (isTrigger(KEY_L))++sw %= 5;
        if (sw == 1)triangle(sx - 1, sy, gx, gy);
        if (sw == 2)triangle(sx, sy - 1, gx, gy);
        if (sw == 3)triangle(sx + 1, sy, gx, gy);
        if (sw == 4)triangle(sx, sy + 1, gx, gy);
	}
}
