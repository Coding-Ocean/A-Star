#include"libOne.h"
#include"CELL.h"

int Cols = 10;
int Rows = 10;
int W; //セルの横幅
int H; //セルの高さ
int Sx=0; //スタートセル
int Sy=0;
int Gx=0; //ゴールセル
int Gy=0;
CELL* Cells = 0;

DIR Dir[4] = {
    {0,-1},{1,0},{0,1},{-1,0}//上右下左
};

int DoneFlag;
int DrawFlag;

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
DIR Path[100];//要素数テキトー！

//functions
void setRandomPos(int& x, int& y) {
    do {
        x = random() % Cols;
        y = random() % Rows;
    } while (Cells[x + y * Cols].status == OBSTACLE);
}
//セルデータ初期設定（リセット時にも呼び出される）
void setCells() 
{
    DrawFlag = 1;
    DoneFlag = false;

    for (int x = 0; x < Cols; x++) {
        for (int y = 0; y < Rows; y++) {
            int i = x + y * Cols;
            Cells[i].status = NO_CHECK;
            Cells[i].cost = 0;
            Cells[i].score = 0;
            Cells[i].parentDirIdx = 0;
            if (MapData[i] == 1) {
                Cells[i].status = OBSTACLE;
            }
        }
    }

    setRandomPos(Sx, Sy);
    Cells[Sx + Sy * Cols].status = OPENED;
    setRandomPos(Gx, Gy);

    for (int i = 0; i < 100; i++) {
        Path[i].x = 5;
        Path[i].y = 5;
    }
}
void create()
{
    //セルマトリックスをつくる
    W = width / Cols;
    H = height / Rows;
    Cells = new CELL[W * H];
    for (int x = 0; x < Cols; x++) {
        for (int y = 0; y < Rows; y++) {
            Cells[x + y * Cols].set(x, y, W, H);
        }
    }

    setCells();
}

//ゴールまでの最短距離の２乗を求める
//（この予測コストのことをヒューリスティックと呼ぶ）
int huristic(int fromX, int fromY, int toX, int toY) {
    int x = toX - fromX;
    int y = toY - fromY;
    return x * x + y * y;
}

//ゴールから遡って、PATHを設定していく（再帰呼び出し）
void traceRoute(int x, int y) {
    if (x == Sx && y == Sy) {
        return;
    }

    int idx = Cells[x + y * Cols].cost-1;
    int i = Cells[x + y * Cols].parentDirIdx;

    Path[idx].x = -Dir[i].x;
    Path[idx].y = -Dir[i].y;

    traceRoute(x + Dir[i].x, y + Dir[i].y);
}

//ゴールまでのパス探索を１ステップのみ行う
//これが所謂エイスターアルゴリズムの心臓部
int searchStep() 
{
    DrawFlag = 1;
    //オープンセルの中でscoreが最小のセルを選ぶ
    int minScore = 9999;
    int minIdx = -1; //最小スコアセル
    for (int i = 0; i < W*H; i++) {
        if (Cells[i].status == OPENED) {
            if (Cells[i].score < minScore) {
                minScore = Cells[i].score;
                minIdx = i;
            }
        }
    }
    //オープンセルが無ければ通り道はないので終了
    if (minIdx == -1) {
        return 2;
    }
    //最小セルがゴールなら終了
    if (Cells[minIdx].x == Gx && Cells[minIdx].y == Gy) {
        traceRoute(Gx, Gy);
        DoneFlag = true;
        return 1;
    }
    //まず最小スコアのセルをクローズ状態にしてしまう
    Cells[minIdx].status = CLOSED;
    //最小スコアのセルの周囲のセルのスコアを計算してオープンにする
    int ncost = Cells[minIdx].cost + 1;
    for (int i = 0; i < 4; i++) {
        //隣接セルのインデックスneighborX,neighborY
        int nx = Cells[minIdx].x + Dir[i].x;
        int ny = Cells[minIdx].y + Dir[i].y;
        //以下の場合はなにもしないで次の隣接セルの処理へ
        if (nx < 0) continue;
        if (ny < 0) continue;
        if (nx >= Cols) continue;
        if (ny >= Rows) continue;
        CELL& ncell = Cells[nx + ny * Cols];
        if (ncell.status == OBSTACLE) continue;
        if (ncell.status == OPENED) continue;
        //隣接セルのscoreを計算後、セルをオープン状態にする
        if (ncell.status == NO_CHECK) {
            ncell.cost = ncost;
            ncell.score = ncost + huristic(nx, ny, Gx, Gy);
            ncell.parentDirIdx = (i + 2)%4;//Dir[i]の逆ベクトルのインデックス
            ncell.status = OPENED;
        }
    }
    return 0;
}

//ゴールまでのパスを探す
void searchLoop() {
    while (searchStep() == 0);
}

void drawPathLine()
{
    strokeWeight(5);
    stroke(330, 80, 100);

    int sx = Sx*W + W / 2;
    int sy = Sy*H + H / 2;
    for(int i=0; Path[i].x!=5; i++)
    {
        int ex = sx + Path[i].x * W;
        int ey = sy + Path[i].y * H;
        arrow(sx, sy, ex, ey);
        sx = ex;
        sy = ey;
    }
}

void drawCells() {
    clear();
    //セル表示
    for (int i = 0; i < W * H; i++) {
        Cells[i].draw();
    }
    //検索が終了したらパスラインを表示
    if (DoneFlag) {
        drawPathLine();
        //int s = Sx + Sy * Cols;
        //for (int i = 0; i < W * H; i++) {
        //    if (i != s) Cells[i].drawPathLine(Dir);
        //}
    }
    //スタート、ゴール文字表示
    fill(60, 100, 100);
    textSize(W/2);
    text("S", Sx * W + W / 2, Sy * H + H / 2);
    text("G", Gx * W + W / 2, Gy * H + H / 2);

    //for (int i = 0; i < 10; i++) {
    //    text((let)""+Path[i].x+" "+Path[i].y,W/2,i*W/2+W/2);
    //}
}
void gmain()
{
	window(1000, 1000);
    create();
    DrawFlag = 1;
	while (notQuit)
	{
        if (isTrigger(KEY_W))setCells();
        if (!DoneFlag && isTrigger(KEY_D))searchStep();
        if (!DoneFlag && isTrigger(KEY_A))searchLoop();
        if(DrawFlag)drawCells();
        DrawFlag = 0;
	}
}
