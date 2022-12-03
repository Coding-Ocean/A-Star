#include<vector>
#include"libOne.h"
#include"CELL.h"
#include"PATH_FINDER.h"

extern DIR Dir[4] = {
    {0,-1},{1,0},{0,1},{-1,0}//��E����
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
    //�Z���z�������
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

//�Z���f�[�^�����ݒ�i���Z�b�g���ɂ��Ăяo�����j
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

//�S�[���܂ł̍ŒZ�����̂Q������߂�
//�i���̗\���R�X�g�̂��Ƃ��q���[���X�e�B�b�N�ƌĂԁj
int PATH_FINDER::huristic(int fromX, int fromY, int toX, int toY) {
    int x = toX - fromX;
    int y = toY - fromY;
    return x * x + y * y;
}

void PATH_FINDER::traceRoute(int x, int y) {
    //�X�^�[�g�܂ŗ�����return
    if (x == Sx && y == Sy) {
        return;
    }

    //�u��������e�ւ̕����v���w��Dir�̃C���f�b�N�X
    int i = Cells[x + y * Cols].parentDirIdx;
    //�e�ֈړ�
    traceRoute(x + Dir[i].x, y + Dir[i].y);

    //�u��������e�ւ�"�t�����C���f�b�N�X"�v���u�z��PathDirIdxs�ցv���ׂĂ���
    (*PathDirIdxs).push_back( (i + 2) % 4 );
}

//�S�[���܂ł̃p�X�T�����P�X�e�b�v�̂ݍs��
//���ꂪ�����G�C�X�^�[�A���S���Y���̐S����
int PATH_FINDER::searchStep()
{
    if (DoneFlag)return 3;

    DrawFlag = 1;
    //�I�[�v���Z���̒���score���ŏ��ł���Z���̃C���f�b�N�X������
    int minScore = INT_MAX;
    int minIdx = -1; //�ŏ��X�R�A�Z��
    for (int i = 0; i < NumCells; i++) {
        if (Cells[i].status == OPENED) {
            if (Cells[i].score < minScore) {
                minScore = Cells[i].score;
                minIdx = i;
            }
        }
    }
    //�I�[�v���Z����������Ȃ���΁A�ʂ蓹�͂Ȃ��̂ŏI��
    if (minIdx == -1) {
        return 2;
    }
    //�ŏ��Z�����S�[���Ȃ�I��
    if (Cells[minIdx].x == Gx && Cells[minIdx].y == Gy) {
        (*PathDirIdxs).clear();
        traceRoute(Gx, Gy);
        DoneFlag = true;
        return 1;
    }
    //�܂��ŏ��X�R�A�̃Z�����N���[�Y��Ԃɂ��Ă��܂�
    Cells[minIdx].status = CLOSED;
    //���ꂩ��I�[�v������Z���̃R�X�g
    int cost = Cells[minIdx].cost + 1;
    for (int i = 0; i < 4; i++) {
        //�אڃZ���̃C���f�b�N�Xnx,ny(n��neighbor�̗��j
        int nx = Cells[minIdx].x + Dir[i].x;
        int ny = Cells[minIdx].y + Dir[i].y;
        //�}�b�v�̊O���w���Ă�����A���̗אڃZ���̏�����
        if (nx < 0) continue;
        if (ny < 0) continue;
        if (nx >= Cols) continue;
        if (ny >= Rows) continue;
        //�אڃZ������ncell�Ƃ���
        CELL& ncell = Cells[nx + ny * Cols];
        if (ncell.status == NO_CHECK) {
            //�אڃZ����score���v�Z
            ncell.cost = cost;
            ncell.heuristic = huristic(nx, ny, Gx, Gy);
            ncell.score = cost + ncell.heuristic;
            //�I�[�v������
            ncell.status = OPENED;
            //�e�Z���i���̃Z�����J�����Z���j�ւ̕����C���f�b�N�X
            ncell.parentDirIdx = (i + 2) % 4;//Dir[i]�̋t�x�N�g���̃C���f�b�N�X
        }
    }
    return 0;
}

//�S�[���܂ł̃p�X��T��
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
    //�Z���\��
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
    //�������I��������p�X���C����\��
    if (DoneFlag) {
        drawPathLine();
    }
    //�X�^�[�g�A�S�[�������\��
    fill(60, 100, 100);
    textSize(SideLen / 2);
    textMode(BCENTER);
    text("S", Sx * SideLen + SideLen / 2, Sy * SideLen + SideLen / 2);
    text("G", Gx * SideLen + SideLen / 2, Gy * SideLen + SideLen / 2);

    printSize(20);
    print("W�Ń��Z�b�g");
    print("D�ŃX�e�b�v�T��");
    print("");
    print("A�ł��ׂĒT��");
    print("Q�Ń����_�����Z�b�g");

    //print((*PathDirIdxs).size());

    DrawFlag = 0;
}
