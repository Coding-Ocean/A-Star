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
    //�Z���f�[�^�����ݒ�i���Z�b�g���ɂ��Ăяo�����j
    void init(int sx, int sy, int gx, int gy, 
        std::vector<int>* pathDirIdxs);
    //�S�[���܂ł̃p�X��T��
    void search();
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
    //�f���p�B�X�^�[�g�ƃS�[�������߂�
    void setRandomPos(int& x, int& y);
private:
    int* MapData = nullptr;
    struct CELL* Cells = nullptr;
    int NumCells = 0;
    int Cols = 0;
    int Rows = 0;
    int SideLen = 0;

    int Sx = 0; //�X�^�[�g�Z��
    int Sy = 0;
    int Gx = 0; //�S�[���Z��
    int Gy = 0;

    std::vector<int>* PathDirIdxs=nullptr;

    int DoneFlag = 0;
    int DrawFlag = 1;
    int DrawSw = 1;

};

