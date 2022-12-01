#pragma once
struct DIR {
    int x, y;
};
class PAHT_FINDER
{
private:
    DIR Dir[4] = {
        {0,-1},{1,0},{0,1},{-1,0}//��E����
    };
    DIR PathDir[100];//�v�f���e�L�g�[�I
    int PathDirLength=0;

    class CELL* Cells = 0;
    int Cols = 10;
    int Rows = 10;
    int SideLen = 50;

    int Sx = 0; //�X�^�[�g�Z��
    int Sy = 0;
    int Gx = 0; //�S�[���Z��
    int Gy = 0;

    int DoneFlag=0;
    int DrawFlag=1;

public:
    void create();
    //�Z���f�[�^�����ݒ�i���Z�b�g���ɂ��Ăяo�����j
    void setCells(int* mapData, int sx, int sy, int gx, int gy);
    void setRandomPos(int& x, int& y);
    //�S�[���܂ł̃p�X��T��
    void searchLoop();
    //�S�[���܂ł̃p�X�T�����P�X�e�b�v�̂ݍs��
    //���ꂪ�����G�C�X�^�[�A���S���Y���̐S����
    int searchStep();
    //�S�[���܂ł̍ŒZ�����̂Q������߂�
    //�i���̗\���R�X�g�̂��Ƃ��q���[���X�e�B�b�N�ƌĂԁj
    int huristic(int fromX, int fromY, int toX, int toY);
    //�S�[������k���āAPATH��ݒ肵�Ă����i�ċA�Ăяo���j
    void traceRoute(int x, int y);
    //�p�X��\������
    void drawPathLine();
    //�Z����\������
    void drawCells();
};

