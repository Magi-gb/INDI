#include "BL2GLWidget.h"
#include <iostream>

BL2GLWidget::BL2GLWidget(QWidget* parent)
  : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);
}

BL2GLWidget::~BL2GLWidget()
{
  if (program != NULL)
    delete program;
}

void BL2GLWidget::initializeGL()
{
  initializeOpenGLFunctions();

  glClearColor(0.5, 0.7, 1.0, 1.0);

  carregaShaders();
  creaBuffersCub();

  escala = 1.0f;
  glEnable(GL_DEPTH_TEST);
}

void BL2GLWidget::paintGL()
{
  // MyGLWidget fa la feina pel laberint.
}

void BL2GLWidget::resizeGL(int w, int h)
{
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void BL2GLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S:
      escala += 0.05;
      break;
    case Qt::Key_D:
      escala -= 0.05;
      break;
    default:
      event->ignore();
      break;
  }
  update();
}

void BL2GLWidget::carregaShaders()
{
  QOpenGLShader fs(QOpenGLShader::Fragment, this);
  QOpenGLShader vs(QOpenGLShader::Vertex, this);

  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");

  program = new QOpenGLShaderProgram(this);
  program->addShader(&fs);
  program->addShader(&vs);
  program->link();
  program->bind();

  // Atributs que realment existeixen al vertex shader
  vertexLoc   = glGetAttribLocation(program->programId(), "vertex");
  normalLoc   = glGetAttribLocation(program->programId(), "normal");
  matambLoc   = glGetAttribLocation(program->programId(), "matamb");
  matdiffLoc  = glGetAttribLocation(program->programId(), "matdiff");
  matspecLoc  = glGetAttribLocation(program->programId(), "matspec");
  matshinLoc  = glGetAttribLocation(program->programId(), "matshin");

  // Uniform de transformació de model
  transLoc = glGetUniformLocation(program->programId(), "TG");
}


int BL2GLWidget::printOglError(const char file[], int line, const char func[])
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502:
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        std::cout << std::endl;
        retCode = 1;
    }
    return retCode;
}

///
/// Funció d'utilitat per crear un VAO d'un cub amb l'aresta mínima a l'origen
/// i d'aresta unitària.
///
void BL2GLWidget::creaBuffersCub ()
{
  // Dades del cub
  // Vèrtexs del cub
  glm::vec3 vertexs[8] = {
       /* 0*/ glm::vec3( 0.0, 0.0, 0.0),  /* 1*/ glm::vec3( 1.0, 0.0, 0.0),
       /* 2*/ glm::vec3( 0.0, 1.0, 0.0),  /* 3*/ glm::vec3( 1.0, 1.0, 0.0),
       /* 4*/ glm::vec3( 0.0, 0.0, 1.0),  /* 5*/ glm::vec3( 1.0, 0.0, 1.0),
       /* 6*/ glm::vec3( 0.0, 1.0, 1.0),  /* 7*/ glm::vec3( 1.0, 1.0, 1.0)
  };

  // VBO amb la posició dels vèrtexs
  glm::vec3 poscub[36] = {  // 12 triangles
       vertexs[0], vertexs[2], vertexs[1],
       vertexs[1], vertexs[2], vertexs[3],
       vertexs[5], vertexs[1], vertexs[7],
       vertexs[1], vertexs[3], vertexs[7],
       vertexs[2], vertexs[6], vertexs[3],
       vertexs[3], vertexs[6], vertexs[7],
       vertexs[0], vertexs[4], vertexs[6],
       vertexs[0], vertexs[6], vertexs[2],
       vertexs[0], vertexs[1], vertexs[4],
       vertexs[1], vertexs[5], vertexs[4],
       vertexs[4], vertexs[5], vertexs[6],
       vertexs[5], vertexs[7], vertexs[6]
  };

  // VBO amb la normal de cada vèrtex
  glm::vec3 normals[6] = {
       /* 0*/ glm::vec3( 1.0, 0.0,  0.0),  /* 1*/ glm::vec3( -1.0, 0.0, 0.0),
       /* 2*/ glm::vec3( 0.0, 1.0,  0.0),  /* 3*/ glm::vec3( 0.0, -1.0, 0.0),
       /* 4*/ glm::vec3( 0.0, 0.0,  1.0),  /* 5*/ glm::vec3( 0.0, 0.0, -1.0)
  };
  glm::vec3 normcub[36] = {
       normals[5], normals[5], normals[5],
       normals[5], normals[5], normals[5],
       normals[0], normals[0], normals[0],
       normals[0], normals[0], normals[0],
       normals[2], normals[2], normals[2],
       normals[2], normals[2], normals[2],
       normals[1], normals[1], normals[1],
       normals[1], normals[1], normals[1],
       normals[3], normals[3], normals[3],
       normals[3], normals[3], normals[3],
       normals[4], normals[4], normals[4],
       normals[4], normals[4], normals[4]
  };

  // inicialitzem el material del cub
  glm::vec3 amb, diff, spec;
  float shin;
  amb = glm::vec3(0.1,0.0,0.0);
  diff = glm::vec3(0.6,0.5,0.5);
  spec = glm::vec3(0.6,0.6,0.6);
  shin = 100;

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matambcub[36] = {
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb
  };
  float a=0.5;
  glm::vec3 diff1 = glm::vec3(0.6,0.0,0.0);
  glm::vec3 matdiffcub[36] = {
	diff, diff, diff1, diff, diff, diff,
	diff, diff, diff1, diff, diff, diff,
	diff*a, diff*a, diff1*a, diff*a, diff*a, diff1*a,
	diff, diff, diff1, diff, diff, diff,
	diff, diff, diff1, diff, diff, diff,
	diff, diff, diff1, diff, diff, diff
  };
  glm::vec3 matspeccub[36] = {
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec
  };
  float matshincub[36] = {
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin
  };

// Creació del Vertex Array Object del cub
  glGenVertexArrays(1, &VAO_Cub);
  glBindVertexArray(VAO_Cub);

  GLuint VBO_Cub[6];
  glGenBuffers(6, VBO_Cub);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(poscub), poscub, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normcub), normcub, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matambcub), matambcub, GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffcub), matdiffcub, GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspeccub), matspeccub, GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshincub), matshincub, GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}
