#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include "model.h"
#include "BL2GLWidget.h"
#include <QTimer>
#include <QMouseEvent>

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();
  
  public slots:
    void rotateCoins();

  protected:
    // Inicialización del contexto OpenGL
    void initializeGL() override;

    // Pintado de la escena
    void paintGL() override;

    // Transformación por celda del laberinto
    void modelTransformCell(int fila, int col);
    void modelTransformCellT(int fila, int col);

    // Inicialización de cámara y matrices
    void initCamera();

    void projectTransform();

    void viewTransform();

    void keyPressEvent(QKeyEvent *event);

    void carregaShaders() override;

    //Mouse events
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

    //Poryeccion ortogonal
    void projectTransformOrtho();
    void viewTransformOrtho();

    // Uniform locations
    GLuint PMLoc, VMLoc, TG_Loc;

    // Matrices
    glm::mat4 PM;
    glm::mat4 VM;
    glm::mat4 TG;
    //Matriz de la vision ortogonal
    glm::mat4 PMortho;

    // Dimensiones del laberinto
    static const int N = 10; //filas
    static const int M = 15; //columnas

    // Matriz del laberinto
    int laberint[N][M] = {
      {1,1,1,1,1,1,1,1,1,1,4,1,1,1,1},
      {1,0,5,0,1,5,0,1,0,0,0,1,0,2,1},
      {1,0,1,0,0,1,0,1,0,1,5,0,3,0,1},
      {1,0,1,1,0,5,0,1,0,1,0,1,1,0,4},
      {1,0,1,0,1,1,0,0,0,1,0,0,0,0,1},
      {1,0,5,0,1,0,0,0,1,1,1,1,5,1,1},
      {1,0,1,0,0,0,1,0,5,0,0,0,0,0,1},
      {4,0,1,1,1,0,1,0,1,1,0,1,1,0,1},
      {1,0,0,0,1,5,1,0,5,1,0,1,5,0,1},
      {1,1,1,1,1,1,1,1,1,1,4,1,1,1,1}
    };

    Model morty, torre, moneda, fantasma;
    GLuint VAO_Morty, VAO_Torre, VAO_Moneda, VAO_Fantasma;

    void creaBuffersMorty ();
    void creaBuffersTorre ();
    void creaBuffersMoneda ();
    void creaBuffersFantasma ();

    void modelTransformMorty (int fila, int col);
    void modelTransformTorre (int fila, int col);
    void modelTransformMoneda (int fila, int col);
    void modelTransformFantasma (int fila, int col);

    void resizeGL(int w, int h) override;

    void moveMorty(int df, int dc);

    //Escalas models
    void calculaCapsaModel(
      Model &m,
      float &escala,
      glm::vec3 &centreBaseModel,
      float desiredSize
    );

    void calculaCapsaEscena();

    float escalaMorty;
    float escalaTorre;
    float escalaMoneda;
    float escalaFantasma;

    glm::vec3 centreBaseMorty;
    glm::vec3 centreBaseTorre;
    glm::vec3 centreBaseMoneda;
    glm::vec3 centreBaseFantasma;

    //Moviment Moneda

    QTimer timer;
    float angleCoin = 0.0f;

    //Moviment Morty

    void mouMortyEndavant();
    void mouMortyEnrere();
    void giraMortyEsquerra();
    void giraMortyDreta();
    float angleFromDirMorty() const;

    glm::vec3 minEscena;
    glm::vec3 maxEscena;
    glm::vec3 centroEscena;

    float radioEscena;
    float distCamera;
    
    // ---------- CAMARAS ----------
        //Euler angles

    float angleY = 0.0f;
    float angleX = 0.0f;

    int xClick;
    int yClick;

    bool rotating = false;
    bool zooming = false;

        //Otros
    bool cameraFPS = false;

    glm::vec3 obsPerspectiva;
    glm::vec3 vrpPerspectiva;

    glm::vec3 obsFPS;
    glm::vec3 vrpFPS;

    float fovPerspectiva;
    float fovFPS;

    // Posicion de Morty
    void findMorty();
    int mortyFila;
    int mortyCol;

    // Direccion de Morty
    float angleMorty = -90.0f;

    int dirMorty = 1;

    glm::vec3 direccioMiradaMorty() const;

    //LUZ
    void updateLightPosition();

    GLuint lightPosLoc;
    GLuint lightColorLoc;
    GLuint viewPosLoc;

    glm::vec3 lightPos;
    glm::vec3 lightColor;

    float angleSol = 0.0f;
    
};

#endif
