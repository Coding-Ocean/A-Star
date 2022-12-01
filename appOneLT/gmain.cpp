#include<vector>
#include"libOne.h"
#include"CELL.h"
#include"PATH_FINDER.h"

int MapData[100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void gmain()
{
	window(1000, 1000);
    
    PAHT_FINDER path_finder;
    path_finder.create();

    int sx=3, sy=1, gx=4, gy=8;
    path_finder.setCells(MapData, sx, sy, gx, gy);
    
    while (notQuit)
	{
        if (isTrigger(KEY_W)) {
            path_finder.setRandomPos(sx, sy);
            path_finder.setRandomPos(gx, gy);
            path_finder.setCells(MapData, sx, sy, gx, gy);
        }
        if (isTrigger(KEY_A))path_finder.searchLoop();
        if (isTrigger(KEY_D))path_finder.searchStep();
        path_finder.drawCells();
	}
}
