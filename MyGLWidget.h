#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include "model.h"
#include "BL2GLWidget.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

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

    void carregaShaders() override;

    // Uniform locations
    GLuint PMLoc, VMLoc, TG_Loc;

    // Matrices
    glm::mat4 PM;
    glm::mat4 VM;
    glm::mat4 TG;

    // Dimensiones del laberinto
    static const int N = 10; //filas
    static const int M = 15; //columnas

    // Matriz del laberinto
    int laberint[N][M] = {
      {1,1,1,1,1,1,1,1,1,1,4,1,1,1,1},
      {1,0,0,0,3,0,0,0,0,0,0,0,0,0,1},
      {1,1,1,1,0,1,1,1,0,1,1,1,1,0,1},
      {1,0,0,3,0,0,0,1,0,0,0,1,0,0,4},
      {1,1,1,0,1,1,0,1,1,1,0,1,1,0,1},
      {1,0,0,3,0,1,0,0,0,1,0,0,0,3,1},
      {1,1,1,1,0,1,1,1,0,1,1,1,1,0,1},
      {4,0,1,0,0,0,1,0,0,0,0,1,0,0,1},
      {1,0,0,0,1,0,1,0,0,1,0,0,0,2,1},
      {1,1,1,1,1,1,1,1,1,1,4,1,1,1,1}
    };

    Model morty, torre, moneda;
    GLuint VAO_Morty, VAO_Torre, VAO_Moneda;

    void creaBuffersMorty ();

    void creaBuffersTorre ();

    void creaBuffersMoneda ();

    void modelTransformMorty (int fila, int col);

    void modelTransformTorre (int fila, int col);

    void modelTransformMoneda (int fila, int col);

    
};

#endif
