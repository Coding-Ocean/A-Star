#include"libOne.h"
#include"CELL.h"
#include"PATH_FINDER.h"

void PAHT_FINDER::create()
{
    //�Z���}�g���b�N�X������
    Cells = new CELL[Cols*Rows];
    for (int c = 0; c < Cols; c++) {
        for (int r = 0; r < Rows; r++) {
            Cells[c + r * Cols].create(c, r);
        }
    }
    SideLen = height / Rows;
}

//�Z���f�[�^�����ݒ�i���Z�b�g���ɂ��Ăяo�����j
void PAHT_FINDER::setCells(int* mapData, int sx, int sy, int gx, int gy)
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
    Cells[Sx + Sy * Cols].Status = OPENED;

    for (int i = 0; i < 100; i++) {
        PathDir[i].x = 5;
        PathDir[i].y = 5;
    }

    DrawFlag = 1;
}

void PAHT_FINDER::setRandomPos(int& x, int& y) {
    do {
        x = random() % Cols;
        y = random() % Rows;
    } while (Cells[x + y * Cols].Status == OBSTACLE);
}

//�S�[���܂ł̍ŒZ�����̂Q������߂�
//�i���̗\���R�X�g�̂��Ƃ��q���[���X�e�B�b�N�ƌĂԁj
int PAHT_FINDER::huristic(int fromX, int fromY, int toX, int toY) {
    int x = toX - fromX;
    int y = toY - fromY;
    return x * x + y * y;
}

//�S�[������k���āAPAHT��ݒ肵�Ă����i�ċA�Ăяo���j
void PAHT_FINDER::traceRoute(int x, int y) {
    if (x == Sx && y == Sy) {
        return;
    }

    int idx = Cells[x + y * Cols].Cost - 1;
    int i = Cells[x + y * Cols].ParentDirIdx;

    PathDir[idx].x = -Dir[i].x;
    PathDir[idx].y = -Dir[i].y;

    traceRoute(x + Dir[i].x, y + Dir[i].y);
}

//�S�[���܂ł̃p�X�T�����P�X�e�b�v�̂ݍs��
//���ꂪ�����G�C�X�^�[�A���S���Y���̐S����
int PAHT_FINDER::searchStep()
{
    if (DoneFlag)return 3;

    DrawFlag = 1;
    //�I�[�v���Z���̒���score���ŏ��̃Z����I��
    int minScore = 9999;
    int minIdx = -1; //�ŏ��X�R�A�Z��
    for (int i = 0; i < Cols*Rows; i++) {
        if (Cells[i].Status == OPENED) {
            if (Cells[i].Score < minScore) {
                minScore = Cells[i].Score;
                minIdx = i;
            }
        }
    }
    //�I�[�v���Z����������Βʂ蓹�͂Ȃ��̂ŏI��
    if (minIdx == -1) {
        return 2;
    }
    //�ŏ��Z�����S�[���Ȃ�I��
    if (Cells[minIdx].Col == Gx && Cells[minIdx].Row == Gy) {
        PathDirLength = Cells[minIdx].Cost;
        traceRoute(Gx, Gy);
        DoneFlag = true;
        return 1;
    }
    //�܂��ŏ��X�R�A�̃Z�����N���[�Y��Ԃɂ��Ă��܂�
    Cells[minIdx].Status = CLOSED;
    //�ŏ��X�R�A�̃Z���̎��͂̃Z���̃X�R�A���v�Z���ăI�[�v���ɂ���
    int ncost = Cells[minIdx].Cost + 1;
    for (int i = 0; i < 4; i++) {
        //�אڃZ���̃C���f�b�N�XneighborX,neighborY
        int nx = Cells[minIdx].Col + Dir[i].x;
        int ny = Cells[minIdx].Row + Dir[i].y;
        //�ȉ��̏ꍇ�͂Ȃɂ����Ȃ��Ŏ��̗אڃZ���̏�����
        if (nx < 0) continue;
        if (ny < 0) continue;
        if (nx >= Cols) continue;
        if (ny >= Rows) continue;
        CELL& ncell = Cells[nx + ny * Cols];
        if (ncell.Status == OBSTACLE) continue;
        if (ncell.Status == OPENED) continue;
        //�אڃZ����score���v�Z��A�Z�����I�[�v����Ԃɂ���
        if (ncell.Status == NO_CHECK) {
            ncell.Cost = ncost;
            ncell.Score = ncost + huristic(nx, ny, Gx, Gy);
            ncell.Status = OPENED;
            ncell.ParentDirIdx = (i + 2) % 4;//Dir[i]�̋t�x�N�g���̃C���f�b�N�X
        }
    }
    return 0;
}

//�S�[���܂ł̃p�X��T��
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
    for (int i = 0; i<PathDirLength; i++)
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
    //�Z���\��
    for (int i = 0; i < Cols*Rows; i++) {
        //Cells[i].draw(SideLen);
        Cells[i].drawMinimal(SideLen);
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

    DrawFlag = 0;
}
