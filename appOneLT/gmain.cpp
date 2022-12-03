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

    PATH_FINDER path_finder;
    path_finder.create(MapData, 10, 10);

    std::vector<int> pathDirIdxs;

    int sx=2, sy=1, gx=4, gy=8;
    path_finder.init(sx, sy, gx, gy, &pathDirIdxs);
    
    while (notQuit)
    {
        if (isTrigger(KEY_Q)) {
            path_finder.setRandomPos(sx, sy);
            path_finder.setRandomPos(gx, gy);
            path_finder.init(sx, sy, gx, gy, &pathDirIdxs);
        }

        if (isTrigger(KEY_E)) {
            sx = 2; sy = 1; gx = 4; gy = 8;
            path_finder.init(sx, sy, gx, gy, &pathDirIdxs);
        }

        if (isTrigger(KEY_A))path_finder.search();

        if (isTrigger(KEY_D))path_finder.searchStep();

        path_finder.drawCells();
    }
}
