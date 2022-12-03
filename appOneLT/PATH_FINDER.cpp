#include<vector>
#include"libOne.h"
#include"CELL.h"
#include"PATH_FINDER.h"

extern DIR Dir[4] = {
    {0,-1},{1,0},{0,1},{-1,0}//上右下左
};

PATH_FINDER::PATH_FINDER()
{
}

PATH_FINDER::~PATH_FINDER()
{
    SAFE_DELETE_ARRAY(Cells); 
}

void PATH_FINDER::create(int* mapData, int cols, int rows)
{
    MapData = mapData;
    //セル配列をつくる
    Cols = cols;
    Rows = rows;
    NumCells = Cols * Rows;
    Cells = new CELL[NumCells];
    for (int x = 0; x < Cols; x++) {
        for (int y = 0; y < Rows; y++) {
            Cells[x + y * Cols].x = x;
            Cells[x + y * Cols].y = y;
        }
    }
    SideLen = height / Rows;
}

void PATH_FINDER::destroy()
{
    SAFE_DELETE_ARRAY(Cells);
}

//セルデータ初期設定（リセット時にも呼び出される）
void PATH_FINDER::init(int sx, int sy, int gx, int gy, 
    std::vector<int>* pathDirIdxs)
{
    DoneFlag = 0;

    for (int x = 0; x < Cols; x++) {
        for (int y = 0; y < Rows; y++) {
            int i = x + y * Cols;
            if (MapData[i] == 1)
                Cells[i].status = OBSTACLE;
            else
                Cells[i].status = NO_CHECK;
            Cells[i].cost = 0;
            Cells[i].heuristic = 0;
            Cells[i].score = 0;
            Cells[i].parentDirIdx = 0;
        }
    }

    Gx = gx;
    Gy = gy;
    Sx = sx;
    Sy = sy;
    Cells[Sx + Sy * Cols].status = OPENED;

    PathDirIdxs = pathDirIdxs;

    DrawFlag = 1;
}

void PATH_FINDER::setRandomPos(int& x, int& y) {
    do {
        x = random() % Cols;
        y = random() % Rows;
    } while (Cells[x + y * Cols].status == OBSTACLE);
}

//ゴールまでの最短距離の２乗を求める
//（この予測コストのことをヒューリスティックと呼ぶ）
int PATH_FINDER::huristic(int fromX, int fromY, int toX, int toY) {
    int x = toX - fromX;
    int y = toY - fromY;
    return x * x + y * y;
}

void PATH_FINDER::traceRoute(int x, int y) {
    //スタートまで来たらreturn
    if (x == Sx && y == Sy) {
        return;
    }

    //「自分から親への方向」を指すDirのインデックス
    int i = Cells[x + y * Cols].parentDirIdx;
    //親へ移動
    traceRoute(x + Dir[i].x, y + Dir[i].y);

    //「自分から親への"逆方向インデックス"」を「配列PathDirIdxsへ」並べていく
    (*PathDirIdxs).push_back( (i + 2) % 4 );
}

//ゴールまでのパス探索を１ステップのみ行う
//これが所謂エイスターアルゴリズムの心臓部
int PATH_FINDER::searchStep()
{
    if (DoneFlag)return 3;

    DrawFlag = 1;
    //オープンセルの中でscoreが最小であるセルのインデックスを決定
    int minScore = INT_MAX;
    int minIdx = -1; //最小スコアセル
    for (int i = 0; i < NumCells; i++) {
        if (Cells[i].status == OPENED) {
            if (Cells[i].score < minScore) {
                minScore = Cells[i].score;
                minIdx = i;
            }
        }
    }
    //オープンセルが見つからなければ、通り道はないので終了
    if (minIdx == -1) {
        return 2;
    }
    //最小セルがゴールなら終了
    if (Cells[minIdx].x == Gx && Cells[minIdx].y == Gy) {
        (*PathDirIdxs).clear();
        traceRoute(Gx, Gy);
        DoneFlag = true;
        return 1;
    }
    //まず最小スコアのセルをクローズ状態にしてしまう
    Cells[minIdx].status = CLOSED;
    //これからオープンするセルのコスト
    int cost = Cells[minIdx].cost + 1;
    for (int i = 0; i < 4; i++) {
        //隣接セルのインデックスnx,ny(nはneighborの略）
        int nx = Cells[minIdx].x + Dir[i].x;
        int ny = Cells[minIdx].y + Dir[i].y;
        //マップの外を指していたら、次の隣接セルの処理へ
        if (nx < 0) continue;
        if (ny < 0) continue;
        if (nx >= Cols) continue;
        if (ny >= Rows) continue;
        //隣接セル名をncellとする
        CELL& ncell = Cells[nx + ny * Cols];
        if (ncell.status == NO_CHECK) {
            //隣接セルのscoreを計算
            ncell.cost = cost;
            ncell.heuristic = huristic(nx, ny, Gx, Gy);
            ncell.score = cost + ncell.heuristic;
            //オープンする
            ncell.status = OPENED;
            //親セル（このセルを開いたセル）への方向インデックス
            ncell.parentDirIdx = (i + 2) % 4;//Dir[i]の逆ベクトルのインデックス
        }
    }
    return 0;
}

//ゴールまでのパスを探す
void PATH_FINDER::search() {
    if (DoneFlag)return;
    while (searchStep() == 0);
}

void PATH_FINDER::drawPathLine()
{
    strokeWeight(5);
    stroke(330, 80, 100);

    int sx = Sx * SideLen + SideLen / 2;
    int sy = Sy * SideLen + SideLen / 2;
    for (int i : *PathDirIdxs)
    {
        int ex = sx + Dir[i].x * SideLen;
        int ey = sy + Dir[i].y * SideLen;
        arrow(sx, sy, ex, ey);
        sx = ex;
        sy = ey;
    }
}

void PATH_FINDER::drawCells() {
    if (isTrigger(KEY_SPACE)) {
        DrawSw = 1 - DrawSw;
        DrawFlag = 1;
    }
    if (!DrawFlag)
        return;
    
    clear();
    //セル表示
    if (DrawSw) {
        for (int i = 0; i < NumCells; i++) {
            Cells[i].draw(SideLen);
        }
    }
    else {
        for (int i = 0; i < NumCells; i++) {
            Cells[i].drawMinimal(SideLen);
        }
    }
    //検索が終了したらパスラインを表示
    if (DoneFlag) {
        drawPathLine();
    }
    //スタート、ゴール文字表示
    fill(60, 100, 100);
    textSize(SideLen / 2);
    textMode(BCENTER);
    text("S", Sx * SideLen + SideLen / 2, Sy * SideLen + SideLen / 2);
    text("G", Gx * SideLen + SideLen / 2, Gy * SideLen + SideLen / 2);

    printSize(20);
    print("Wでリセット");
    print("Dでステップ探索");
    print("");
    print("Aですべて探索");
    print("Qでランダムリセット");

    //print((*PathDirIdxs).size());

    DrawFlag = 0;
}
