#ifndef BL2GLWIDGET_H
#define BL2GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class BL2GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
  Q_OBJECT

  public:
    BL2GLWidget(QWidget *parent = 0);
    ~BL2GLWidget();

  protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    virtual void keyPressEvent(QKeyEvent *event);

    virtual void carregaShaders();
    void creaBuffersCub(); 

    QOpenGLShaderProgram *program;

    // VAO del cub
    GLuint VAO_Cub;

    // Atributs del shader
    GLuint vertexLoc;
    GLuint normalLoc;
    GLuint matambLoc;
    GLuint matdiffLoc;
    GLuint matspecLoc;
    GLuint matshinLoc;

    // Uniform TG
    GLuint transLoc;

    // Variables auxiliars
    float escala;
    int ample, alt;

    int printOglError(const char file[], const int line, const char func[]);
};

#endif
