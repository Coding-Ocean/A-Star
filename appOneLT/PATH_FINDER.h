#pragma once
struct DIR {
    int x, y;
};
class PAHT_FINDER
{
private:
    DIR Dir[4] = {
        {0,-1},{1,0},{0,1},{-1,0}//上右下左
    };
    DIR PathDir[100];//要素数テキトー！
    int PathDirLength=0;

    class CELL* Cells = 0;
    int Cols = 10;
    int Rows = 10;
    int SideLen = 50;

    int Sx = 0; //スタートセル
    int Sy = 0;
    int Gx = 0; //ゴールセル
    int Gy = 0;

    int DoneFlag=0;
    int DrawFlag=1;

public:
    void create();
    //セルデータ初期設定（リセット時にも呼び出される）
    void setCells(int* mapData, int sx, int sy, int gx, int gy);
    void setRandomPos(int& x, int& y);
    //ゴールまでのパスを探す
    void searchLoop();
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
};

