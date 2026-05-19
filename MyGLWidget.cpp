#include "MyGLWidget.h"

#include <iostream>
#include <cmath>
#include <cstdlib>

#define CHECK() printOglError(__FILE__, __LINE__, __FUNCTION__)
#define DEBUG(text) std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << ":" << text << std::endl;

MyGLWidget::~MyGLWidget() {}

// =====================================================
// INITIALIZE
// =====================================================

void MyGLWidget::initializeGL()
{
    BL2GLWidget::initializeGL();

    morty.load("../Models3D/Morty.obj");
    fantasma.load("../Models3D/Fantasma.obj");
    moneda.load("../Models3D/Coin.obj");
    torre.load("../Models3D/tower.obj");

    calculaCapsaModel(morty, infoMorty, 1.5f);
    calculaCapsaModel(fantasma, infoFantasma, 0.65f);
    calculaCapsaModel(moneda, infoMoneda, 0.5f);
    calculaCapsaModel(torre, infoTorre, 6.0f);

    creaBuffersMorty();
    creaBuffersFantasma();
    creaBuffersMoneda();
    creaBuffersTorre();

    generaMonedes();

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {

            if (laberint[i][j] == 2) {
                playerRow = i;
                playerCol = j;
            }

            if (laberint[i][j] == 3) {
                ghostRow = i;
                ghostCol = j;
            }
        }
    }

    initCamera();

    glEnable(GL_DEPTH_TEST);

    connect(&timer,
            SIGNAL(timeout()),
            this,
            SLOT(rotateCoins()));

    timer.start(16);
}

// =====================================================
// PAINT
// =====================================================

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ==================== MAZE ====================

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {

            if (laberint[i][j] == 1) {

                glBindVertexArray(VAO_Cub);
                modelTransformCell(i, j);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            glBindVertexArray(VAO_Cub);
            modelTransformCellT(i, j);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            if (laberint[i][j] == 4) {

                glBindVertexArray(VAO_Torre);
                modelTransformTorre(i, j);

                glDrawArrays(GL_TRIANGLES, 0, torre.faces().size()*3);
            }
        }
    }


    // ==================== MORTY ====================

    glBindVertexArray(VAO_Morty);

    modelTransformMorty(playerRow, playerCol);

    glDrawArrays(GL_TRIANGLES,
                 0,
                 morty.faces().size()*3);

    // ==================== GHOST ====================

    glBindVertexArray(VAO_Fantasma);

    modelTransformFantasma(ghostRow, ghostCol);

    glDrawArrays(GL_TRIANGLES,
                 0,
                 fantasma.faces().size()*3);

    // ==================== COINS ====================

    glBindVertexArray(VAO_Moneda);

    for (auto &coin : coins) {

        if (coin.activa) {

            modelTransformMoneda(coin.fila, coin.col);

            glDrawArrays(GL_TRIANGLES, 0, moneda.faces().size()*3);
        }
    }

    glBindVertexArray(0);
}

// =====================================================
// CAMERA
// =====================================================

void MyGLWidget::calculaEscena()
{
    float ample = M;
    float profunditat = N;

    centreEscena = glm::vec3(
        ample/2.0f,
        0.0f,
        profunditat/2.0f
    );

    radiEscena = sqrt(ample*ample + profunditat*profunditat);
}

void MyGLWidget::initCamera()
{
    calculaEscena();

    FOV = float(M_PI/3.0f);

    ra = float(width()) / float(height());

    float d = 2.0f * radiEscena;

    glm::vec3 OBS = centreEscena + glm::vec3(d,d,d);

    VM = glm::lookAt(
        OBS,
        centreEscena,
        glm::vec3(0,1,0)
    );

    glUniformMatrix4fv(VMLoc, 1, GL_FALSE, &VM[0][0]);

    float near = d - radiEscena;
    float far = d + radiEscena;

    PM = glm::perspective(FOV, ra, near, far);

    glUniformMatrix4fv(PMLoc, 1, GL_FALSE, &PM[0][0]);
}

void MyGLWidget::resizeGL(int w, int h)
{
    ra = float(w)/float(h);

    PM = glm::perspective(FOV, ra, 0.1f, 200.0f);

    glUniformMatrix4fv(PMLoc, 1, GL_FALSE, &PM[0][0]);

    glViewport(0,0,w,h);
}

// =====================================================
// MODEL INFO
// =====================================================

void MyGLWidget::calculaCapsaModel(Model &m, InfoModel &info, float alcadaDesitjada)
{
    double minx, miny, minz;
    double maxx, maxy, maxz;

    minx = maxx = m.vertices()[0];
    miny = maxy = m.vertices()[1];
    minz = maxz = m.vertices()[2];

    for (unsigned int i = 3;
         i < m.vertices().size();
         i += 3)
    {
        minx = std::min(minx, m.vertices()[i]);
        maxx = std::max(maxx, m.vertices()[i]);

        miny = std::min(miny, m.vertices()[i+1]);
        maxy = std::max(maxy, m.vertices()[i+1]);

        minz = std::min(minz, m.vertices()[i+2]);
        maxz = std::max(maxz, m.vertices()[i+2]);
    }

    info.alcada = float(maxy - miny);

    info.escala = alcadaDesitjada / info.alcada;

    info.centreBase = glm::vec3(
        float((minx + maxx)/2.0),
        float(miny),
        float((minz + maxz)/2.0)
    );
}

// =====================================================
// TRANSFORMS
// =====================================================

void MyGLWidget::modelTransformCell(int fila, int col)
{
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col,0,fila));

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCellT(int fila, int col)
{
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col,0,fila));

    TG = glm::scale(TG, glm::vec3(1,0.1,1));

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformMorty(int fila, int col)
{
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col+0.5f, 0, fila+0.5f));

    TG = glm::rotate(TG, glm::radians(float(dir*90)), glm::vec3(0,1,0));

    TG = glm::scale(TG, glm::vec3(infoMorty.escala));

    TG = glm::translate(TG, -infoMorty.centreBase);

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma(int fila, int col)
{
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col+0.5f, 0, fila+0.5f));

    TG = glm::scale(TG, glm::vec3(infoFantasma.escala));

    TG = glm::translate(TG, -infoFantasma.centreBase);

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTorre(int fila, int col)
{
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col + 0.5f, 0, fila + 0.5f));

    TG = glm::scale(TG, glm::vec3(infoTorre.escala));

    TG = glm::translate(TG, -infoTorre.centreBase);

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformMoneda(int fila, int col)
{
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col + 0.5f, 0.15f, fila + 0.5f));

    TG = glm::rotate(TG, angleCoins, glm::vec3(0,1,0));

    TG = glm::scale(TG, glm::vec3(infoMoneda.escala));

    TG = glm::translate(TG, -infoMoneda.centreBase);

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::generaMonedes()
{
    coins.clear();

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {

            if (laberint[i][j] == 0) {
                Coin c;
                c.fila = i;
                c.col = j;
                c.activa = true;
                coins.push_back(c);
            }
        }
    }
}

void MyGLWidget::rotateCoins()
{
    angleCoins += 0.05f;
    update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {

    case Qt::Key_W:
        dir = 0;
        if (playerRow > 0 && laberint[playerRow-1][playerCol] != 1)
            playerRow--;
        break;

    case Qt::Key_S:
        dir = 2;
        if (playerRow < N-1 && laberint[playerRow+1][playerCol] != 1)
            playerRow++;
        break;

    case Qt::Key_A:
        dir = 3;
        if (playerCol > 0 && laberint[playerRow][playerCol-1] != 1)
            playerCol--;
        break;

    case Qt::Key_D:
        dir = 1;
        if (playerCol < M-1 && laberint[playerRow][playerCol+1] != 1)
            playerCol++;
        break;

    default:
        BL2GLWidget::keyPressEvent(event);
    }

    update();
}
 
// =====================================================
// CREA BUFFERS MORTY
// =====================================================

void MyGLWidget::creaBuffersMorty()
{
    GLuint VBO[2];

    glGenVertexArrays(1, &VAO_Morty);
    glBindVertexArray(VAO_Morty);

    glGenBuffers(2, VBO);

    // VERTICES

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*morty.faces().size()*3*3, morty.VBO_vertices(), GL_STATIC_DRAW);

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(vertexLoc);

    // NORMALS

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*morty.faces().size()*3*3, morty.VBO_normals(), GL_STATIC_DRAW);

    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(normalLoc);

    glBindVertexArray(0);
}

// =====================================================
// CREA BUFFERS FANTASMA
// =====================================================

void MyGLWidget::creaBuffersFantasma()
{
    GLuint VBO[2];

    glGenVertexArrays(1, &VAO_Fantasma);
    glBindVertexArray(VAO_Fantasma);

    glGenBuffers(2, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*fantasma.faces().size()*3*3, fantasma.VBO_vertices(), GL_STATIC_DRAW);

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*fantasma.faces().size()*3*3, fantasma.VBO_matdiff(), GL_STATIC_DRAW);

    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(normalLoc);

    glBindVertexArray(0);
}

// =====================================================
// CREA BUFFERS MONEDA
// =====================================================

void MyGLWidget::creaBuffersMoneda()
{
    GLuint VBO[2];

    glGenVertexArrays(1, &VAO_Moneda);
    glBindVertexArray(VAO_Moneda);

    glGenBuffers(2, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*moneda.faces().size()*3*3, moneda.VBO_vertices(), GL_STATIC_DRAW);

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*moneda.faces().size()*3*3, moneda.VBO_matdiff(), GL_STATIC_DRAW);

    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(normalLoc);

    glBindVertexArray(0);
}

// =====================================================
// CREA BUFFERS TORRE
// =====================================================

void MyGLWidget::creaBuffersTorre()
{
    GLuint VBO[2];

    glGenVertexArrays(1, &VAO_Torre);
    glBindVertexArray(VAO_Torre);

    glGenBuffers(2, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*torre.faces().size()*3*3, torre.VBO_vertices(), GL_STATIC_DRAW);

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*torre.faces().size()*3*3, torre.VBO_matdiff(), GL_STATIC_DRAW);

    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(normalLoc);

    glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
    BL2GLWidget::carregaShaders();

    TG_Loc = glGetUniformLocation(program->programId(), "TG");

    PMLoc = glGetUniformLocation(program->programId(), "proj");

    VMLoc = glGetUniformLocation(program->programId(), "view");
}