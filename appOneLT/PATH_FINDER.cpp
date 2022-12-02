#include"libOne.h"
#include"CELL.h"
#include"PATH_FINDER.h"

void PAHT_FINDER::create()
{
    //セルマトリックスをつくる
    Cells = new CELL[Cols*Rows];
    for (int c = 0; c < Cols; c++) {
        for (int r = 0; r < Rows; r++) {
            Cells[c + r * Cols].create(c, r);
        }
    }
    SideLen = height / Rows;
}

//セルデータ初期設定（リセット時にも呼び出される）
void PAHT_FINDER::setCells(int const* mapData, int sx, int sy, int gx, int gy, DIR* pathDir, int* pathDirLength)
{
    DoneFlag = 0;

    for (int c = 0; c < Cols; c++) {
        for (int r = 0; r < Rows; r++) {
            int i = c + r * Cols;
            Cells[i].init(mapData[i]);
        }
    }

    Gx = gx;
    Gy = gy;
    Sx = sx;
    Sy = sy;
    Cells[Sx + Sy * Cols].status = OPENED;

    PathDir = pathDir;
    PathDirLength = pathDirLength;
    *PathDirLength = 0;

    DrawFlag = 1;
}

void PAHT_FINDER::setRandomPos(int& x, int& y) {
    do {
        x = random() % Cols;
        y = random() % Rows;
    } while (Cells[x + y * Cols].status == OBSTACLE);
}

//ゴールまでの最短距離の２乗を求める
//（この予測コストのことをヒューリスティックと呼ぶ）
int PAHT_FINDER::huristic(int fromX, int fromY, int toX, int toY) {
    int x = toX - fromX;
    int y = toY - fromY;
    return x * x + y * y;
    //return Abs(x) + Abs(y);//これだと効率が悪くなる
}

//ゴールから親へ親へと戻りながらPathDirを決定していく（再帰呼び出し）
//void PAHT_FINDER::traceRoute(int x, int y) {
//    //スタートまで来たらreturn
//    if (x == Sx && y == Sy) {
//        return;
//    }
//
//    //PathDirのインデックス
//    int idx = Cells[x + y * Cols].cost - 1;
//    //「自分から親への方向」を指すDirのインデックス
//    int i = Cells[x + y * Cols].parentDirIdx;
//    //「親から自分への方向」を「配列PathDirの後ろから前へ」並べていく
//    PathDir[idx].x = -Dir[i].x;
//    PathDir[idx].y = -Dir[i].y;
//
//    //親へ移動
//    traceRoute(x + Dir[i].x, y + Dir[i].y);
//}
int j = 0;
void PAHT_FINDER::traceRoute(int x, int y) {
    //スタートまで来たらreturn
    if (x == Sx && y == Sy) {
        return;
    }

    //「自分から親への方向」を指すDirのインデックス
    int i = Cells[x + y * Cols].parentDirIdx;
    //親へ移動
    traceRoute(x + Dir[i].x, y + Dir[i].y);

    //PathDirのインデックス
    //int idx = Cells[x + y * Cols].cost - 1;
    //「親から自分への方向」を「配列PathDirの後ろから前へ」並べていく
    PathDir[Idx].x = -Dir[i].x;
    PathDir[Idx].y = -Dir[i].y;
    Idx++;
}

//ゴールまでのパス探索を１ステップのみ行う
//これが所謂エイスターアルゴリズムの心臓部
int PAHT_FINDER::searchStep()
{
    if (DoneFlag)return 3;

    DrawFlag = 1;
    //オープンセルの中でscoreが最小であるセルのインデックスを決定
    int minScore = INT_MAX;
    int minIdx = -1; //最小スコアセル
    for (int i = 0; i < Cols*Rows; i++) {
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
        *PathDirLength = Cells[minIdx].cost;
        Idx = 0;
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
void PAHT_FINDER::searchLoop() {
    if (DoneFlag)return;
    while (searchStep() == 0);
}

void PAHT_FINDER::drawPathLine()
{
    strokeWeight(5);
    stroke(330, 80, 100);

    int sx = Sx * SideLen + SideLen / 2;
    int sy = Sy * SideLen + SideLen / 2;
    for (int i = 0; i<*PathDirLength; i++)
    {
        int ex = sx + PathDir[i].x * SideLen;
        int ey = sy + PathDir[i].y * SideLen;
        arrow(sx, sy, ex, ey);
        sx = ex;
        sy = ey;
    }
}

void PAHT_FINDER::drawCells() {
    if (!DrawFlag)
        return;
    
    clear();
    //セル表示
    for (int i = 0; i < Cols*Rows; i++) {
        Cells[i].draw(SideLen);
        //Cells[i].drawMinimal(SideLen);
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

    //print(*PathDirLength);

    DrawFlag = 0;
}
