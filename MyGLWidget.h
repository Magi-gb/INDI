#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include "model.h"
#include "BL2GLWidget.h"

#include <QTimer>
#include <vector>

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

protected:

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent *event) override;

    void carregaShaders() override;

    // ==================== CAMERA ====================

    void initCamera();
    void calculaEscena();

    GLuint PMLoc, VMLoc, TG_Loc;

    glm::mat4 PM;
    glm::mat4 VM;
    glm::mat4 TG;

    float FOV;
    float ra;

    glm::vec3 centreEscena;
    float radiEscena;

    // ==================== MAZE ====================

    static const int N = 10;
    static const int M = 15;

    int laberint[N][M] = {
      {1,1,1,1,1,1,1,1,1,1,4,1,1,1,1},
      {1,0,0,0,3,0,0,0,0,0,0,0,0,0,1},
      {1,1,1,1,0,1,1,1,0,1,1,1,1,0,1},
      {1,0,0,0,0,0,0,1,0,0,0,1,0,0,4},
      {1,1,1,0,1,1,0,1,1,1,0,1,1,0,1},
      {1,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
      {1,1,1,1,0,1,1,1,0,1,1,1,1,0,1},
      {4,0,1,0,0,0,1,0,0,0,0,1,0,0,1},
      {1,0,0,0,1,0,1,0,0,1,0,0,0,2,1},
      {1,1,1,1,1,1,1,1,1,1,4,1,1,1,1}
    };

    // ==================== MODELS ====================

    struct InfoModel {
        float escala;
        glm::vec3 centreBase;
        float alcada;
    };

    Model morty;
    Model fantasma;
    Model moneda;
    Model torre;

    InfoModel infoMorty;
    InfoModel infoFantasma;
    InfoModel infoMoneda;
    InfoModel infoTorre;

    GLuint VAO_Morty;
    GLuint VAO_Fantasma;
    GLuint VAO_Moneda;
    GLuint VAO_Torre;

    void creaBuffersMorty();
    void creaBuffersFantasma();
    void creaBuffersMoneda();
    void creaBuffersTorre();

    void calculaCapsaModel(Model &m, InfoModel &info, float alcadaDesitjada);

    // ==================== TRANSFORMS ====================

    void modelTransformCell(int fila, int col);
    void modelTransformCellT(int fila, int col);
    void modelTransformMorty(int fila, int col);
    void modelTransformFantasma(int fila, int col);
    void modelTransformMoneda(int fila, int col);
    void modelTransformTorre(int fila, int col);

    // ==================== PLAYER ====================

    int playerRow;
    int playerCol;

    int ghostRow;
    int ghostCol;

    int dir = 0;

    // ==================== COINS ====================

    struct Coin {
        int fila;
        int col;
        bool activa;
    };

    std::vector<Coin> coins;

    void generaMonedes();

    float angleCoins = 0.0f;

    QTimer timer;

public slots:

    void rotateCoins();
};

#endif