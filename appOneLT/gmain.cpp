#include"libOne.h"
#include"CELL.h"

int Cols = 30;
int Rows = 30;
int W; //�Z���̉���
int H; //�Z���̍���
int Sx=-1; //�X�^�[�g�Z��
int Sy=-1;
int Gx=-1; //�S�[���Z��
int Gy=-1;
CELL* Cells = 0;

DIR Dir[8] = {
    {0,-1},{0,1},{-1,0},{1,0},//�㉺���E
    {0,1},{0,-1},{1,0},{-1,0},//���Ε���
};

int Button;
int ButtonStep;
int ButtonReset;

int DoneFlag;
int DrawFlag;

int MapData[900] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

//functions
void setRandomPos(int& x, int& y) {
    do {
        x = random() % Cols;
        y = random() % Rows;
    } while (Cells[x + y * Cols].status == OBSTACLE);
}
//�Z���f�[�^�����ݒ�i���Z�b�g���ɂ��Ăяo�����j
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

    setRandomPos(Gx, Gy);
    setRandomPos(Sx, Sy);
    Cells[Sx + Sy * Cols].status = OPENED;
}
void create()
{
    //�Z���}�g���b�N�X������
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

//�S�[���܂ł̍ŒZ�����̂Q������߂�
//�i���̗\���R�X�g�̂��Ƃ��q���[���X�e�B�b�N�ƌĂԁj
int huristic(int fromX, int fromY, int toX, int toY) {
    int x = toX - fromX;
    int y = toY - fromY;
    return x * x + y * y;
}

//�S�[������k���āA�ʂ蓹��status��PATH��ݒ肵�Ă����i�ċA�Ăяo���j
void traceRoute(int x, int y) {
    Cells[x + y * Cols].status = PATH;

    if (x == Sx && y == Sy) {
        return;
    }

    int i = Cells[x + y * Cols].parentDirIdx;
    traceRoute(x + Dir[i].x, y + Dir[i].y);

    return;
}

//�S�[���܂ł̃p�X�T�����P�X�e�b�v�̂ݍs��
//���ꂪ�����G�[�X�^�[�A���S���Y���̐S����
int searchStep() 
{
    DrawFlag = 1;
    //�I�[�v���Z���̒���score���ŏ��̃Z����I��
    int minScore = 9999;
    int minIdx = -1; //�ŏ��X�R�A�Z��
    for (int i = 0; i < W*H; i++) {
        if (Cells[i].status == OPENED) {
            if (Cells[i].score < minScore) {
                minScore = Cells[i].score;
                minIdx = i;
            }
        }
    }
    //�I�[�v���Z����������Βʂ蓹�͂Ȃ��̂ŏI��
    if (minIdx == -1) {
        print("no solution");
        return 2;
    }
    //�ŏ��Z�����S�[���Ȃ�I��
    if (Cells[minIdx].x == Gx && Cells[minIdx].y == Gy) {
        traceRoute(Gx, Gy);
        DoneFlag = true;
        return 1;
    }
    //�܂��ŏ��X�R�A�̃Z�����N���[�Y��Ԃɂ��Ă��܂�
    Cells[minIdx].status = CLOSED;
    //�ŏ��X�R�A�̃Z���̎��͂̃Z���̃X�R�A���v�Z���ăI�[�v���ɂ���
    int ncost = Cells[minIdx].cost + 1;
    for (int i = 0; i < 4; i++) {
        //�אڃZ���̃C���f�b�N�XneighborX,neighborY
        int nx = Cells[minIdx].x + Dir[i].x;
        int ny = Cells[minIdx].y + Dir[i].y;
        //�ȉ��̏ꍇ�͂Ȃɂ����Ȃ��Ŏ��̗אڃZ���̏�����
        if (nx < 0) continue;
        if (ny < 0) continue;
        if (nx >= Cols) continue;
        if (ny >= Rows) continue;
        CELL& ncell = Cells[nx + ny * Cols];
        if (ncell.status == OBSTACLE) continue;
        if (ncell.status == OPENED) continue;
        //�אڃZ����score���v�Z��A�Z�����I�[�v����Ԃɂ���
        if (ncell.status == NO_CHECK) {
            ncell.cost = ncost;
            ncell.score = ncost + huristic(nx, ny, Gx, Gy);
            ncell.parentDirIdx = (i + 4);
            ncell.status = OPENED;
        }
    }
    return 0;
}

//�S�[���܂ł̃p�X��T��
void searchLoop() {
    while (searchStep() == 0);
}

void drawCells() {
    clear();
    //�Z���\��
    for (int i = 0; i < W * H; i++) {
        Cells[i].draw();
    }
    //�������I��������p�X���C����\��
    if (DoneFlag) {
        int s = Sx + Sy * Cols;
        for (int i = 0; i < W * H; i++) {
            if (i != s) Cells[i].drawPathLine(Dir);
        }
    }
    //�X�^�[�g�A�S�[�������\��
    fill(0, 0, 100);
    textSize(W / 2);
    text("S", Sx * W + W / 2, Sy * H + H / 2);
    text("G", Gx * W + W / 2, Gy * H + H / 2);
    print(Cells[Gx + Gy * Cols].cost);
}

void gmain()
{
	window(990, 990);
    create();
    DrawFlag = 1;
	while (notQuit)
	{
        if (isTrigger(KEY_W))setCells();
        if (isTrigger(KEY_D))searchStep();
        if (!DoneFlag && isTrigger(KEY_A))searchLoop();
        if(DrawFlag)drawCells();
        DrawFlag = 0;
	}
}
