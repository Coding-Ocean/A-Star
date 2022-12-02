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

//�S�[���܂ł̍ŒZ�����̂Q������߂�
//�i���̗\���R�X�g�̂��Ƃ��q���[���X�e�B�b�N�ƌĂԁj
int PAHT_FINDER::huristic(int fromX, int fromY, int toX, int toY) {
    int x = toX - fromX;
    int y = toY - fromY;
    return x * x + y * y;
    //return Abs(x) + Abs(y);//���ꂾ�ƌ����������Ȃ�
}

//�S�[������e�֐e�ւƖ߂�Ȃ���PathDir�����肵�Ă����i�ċA�Ăяo���j
//void PAHT_FINDER::traceRoute(int x, int y) {
//    //�X�^�[�g�܂ŗ�����return
//    if (x == Sx && y == Sy) {
//        return;
//    }
//
//    //PathDir�̃C���f�b�N�X
//    int idx = Cells[x + y * Cols].cost - 1;
//    //�u��������e�ւ̕����v���w��Dir�̃C���f�b�N�X
//    int i = Cells[x + y * Cols].parentDirIdx;
//    //�u�e���玩���ւ̕����v���u�z��PathDir�̌�납��O�ցv���ׂĂ���
//    PathDir[idx].x = -Dir[i].x;
//    PathDir[idx].y = -Dir[i].y;
//
//    //�e�ֈړ�
//    traceRoute(x + Dir[i].x, y + Dir[i].y);
//}
int j = 0;
void PAHT_FINDER::traceRoute(int x, int y) {
    //�X�^�[�g�܂ŗ�����return
    if (x == Sx && y == Sy) {
        return;
    }

    //�u��������e�ւ̕����v���w��Dir�̃C���f�b�N�X
    int i = Cells[x + y * Cols].parentDirIdx;
    //�e�ֈړ�
    traceRoute(x + Dir[i].x, y + Dir[i].y);

    //PathDir�̃C���f�b�N�X
    //int idx = Cells[x + y * Cols].cost - 1;
    //�u�e���玩���ւ̕����v���u�z��PathDir�̌�납��O�ցv���ׂĂ���
    PathDir[Idx].x = -Dir[i].x;
    PathDir[Idx].y = -Dir[i].y;
    Idx++;
}

//�S�[���܂ł̃p�X�T�����P�X�e�b�v�̂ݍs��
//���ꂪ�����G�C�X�^�[�A���S���Y���̐S����
int PAHT_FINDER::searchStep()
{
    if (DoneFlag)return 3;

    DrawFlag = 1;
    //�I�[�v���Z���̒���score���ŏ��ł���Z���̃C���f�b�N�X������
    int minScore = INT_MAX;
    int minIdx = -1; //�ŏ��X�R�A�Z��
    for (int i = 0; i < Cols*Rows; i++) {
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
        *PathDirLength = Cells[minIdx].cost;
        Idx = 0;
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
    //�Z���\��
    for (int i = 0; i < Cols*Rows; i++) {
        Cells[i].draw(SideLen);
        //Cells[i].drawMinimal(SideLen);
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

    //print(*PathDirLength);

    DrawFlag = 0;
}
