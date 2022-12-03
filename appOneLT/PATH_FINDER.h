#pragma once
struct DIR {
    int x, y;
};
extern DIR Dir[4];

class PATH_FINDER
{
public:
    PATH_FINDER();
    ~PATH_FINDER();
    void create(int* mapData, int cols, int rows);
    void destroy();
    //セルデータ初期設定（リセット時にも呼び出される）
    void init(int sx, int sy, int gx, int gy, 
        std::vector<int>* pathDirIdxs);
    //ゴールまでのパスを探す
    void search();
    //ゴールまでのパス探索を１ステップのみ行う
    //これが所謂エイスターアルゴリズムの心臓部
    int searchStep();
    //ゴールまでの最短距離の２乗を求める
    //（この予測コストのことをヒューリスティックと呼ぶ）
    int huristic(int fromX, int fromY, int toX, int toY);
    //ゴールから遡って、PATHを設定していく（再帰呼び出し）
    void traceRoute(int x, int y);
    //パスを表示する
    void drawPathLine();
    //セルを表示する
    void drawCells();
    //デモ用。スタートとゴールを求める
    void setRandomPos(int& x, int& y);
private:
    int* MapData = nullptr;
    struct CELL* Cells = nullptr;
    int NumCells = 0;
    int Cols = 0;
    int Rows = 0;
    int SideLen = 0;

    int Sx = 0; //スタートセル
    int Sy = 0;
    int Gx = 0; //ゴールセル
    int Gy = 0;

    std::vector<int>* PathDirIdxs=nullptr;

    int DoneFlag = 0;
    int DrawFlag = 1;
    int DrawSw = 1;

};

