// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include<utility>

#define CHECK() printOglError(__FILE__, __LINE__, __FUNCTION__)
#define DEBUG(text) std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << ":" << text << std::endl;

MyGLWidget::~MyGLWidget() {}

void MyGLWidget::initializeGL() {
    BL2GLWidget::initializeGL();

    morty.load("../Models3D/Morty.obj"); 
    torre.load("../Models3D/tower.obj");
    moneda.load("../Models3D/Coin.obj");

    initCamera();
    creaBuffersMorty();
    creaBuffersTorre();
    creaBuffersMoneda();

    glEnable(GL_DEPTH_TEST);
}


void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {

            if (laberint[i][j] == 1){
                glBindVertexArray(VAO_Cub);
                modelTransformCell(i, j);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            } else {
                if (laberint[i][j] == 2) {
                    glBindVertexArray(VAO_Morty);
                    modelTransformMorty(i, j);
                    glDrawArrays(GL_TRIANGLES, 0, morty.faces().size() * 3);
                } else if (laberint[i][j] == 3) {      // Cambiar el numero de asignacion de la moneda a 5 o aleatorizar el spawn
                    glBindVertexArray(VAO_Moneda);
                    modelTransformMoneda(i, j);
                    glDrawArrays(GL_TRIANGLES, 0, moneda.faces().size() * 3);  
                } else if (laberint[i][j] == 4) {
                    glBindVertexArray(VAO_Torre);
                    modelTransformTorre(i, j);
                    glDrawArrays(GL_TRIANGLES, 0, torre.faces().size() * 3);
                }
                glBindVertexArray(VAO_Cub);
                modelTransformCellT(i, j);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    }

    glBindVertexArray(0);
}

void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders();

    PMLoc = glGetUniformLocation(program->programId(), "PM");
    VMLoc = glGetUniformLocation(program->programId(), "VM");
    TG_Loc = glGetUniformLocation(program->programId(), "TG");
}

void MyGLWidget::initCamera() {
    float FOV = float(M_PI / 3.0);
    float ra = 1.0f;
    float near = 0.1f;
    float far = 200.0f;

    glm::mat4 PM = glm::perspective(FOV, ra, near, far);
    glUniformMatrix4fv(PMLoc, 1, GL_FALSE, &PM[0][0]);

    glm::vec3 OBS(30, 30, 30);
    glm::vec3 VRP(M/2, 0, N/2);
    glm::vec3 UP(0, 1, 0);

    glm::mat4 VM = glm::lookAt(OBS, VRP, UP);
    glUniformMatrix4fv(VMLoc, 1, GL_FALSE, &VM[0][0]);
}

void MyGLWidget::modelTransformCell(int fila, int col) {
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col, 0.0f, fila));

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCellT(int fila, int col) {
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col, 0.0f, fila));
    TG = glm::scale(TG, glm::vec3(1.0f, 0.1f, 1.0f));

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::creaBuffersMorty() {
    glGenVertexArrays(1, &VAO_Morty);
    glBindVertexArray(VAO_Morty);

    GLuint VBO[2];
    glGenBuffers(2, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * morty.faces().size() * 3*3,
                 morty.VBO_vertices(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) *  morty.faces().size() * 3*3,
                 morty.VBO_matdiff(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matdiffLoc);

    glBindVertexArray(0);
}

void MyGLWidget::creaBuffersTorre() {
    glGenVertexArrays(1, &VAO_Torre);
    glBindVertexArray(VAO_Torre);

    GLuint VBO[2];
    glGenBuffers(2, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * torre.faces().size() * 3*3,
                 torre.VBO_vertices(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) *  torre.faces().size() * 3*3,
                 torre.VBO_matdiff(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matdiffLoc);

    glBindVertexArray(0);
}

void MyGLWidget::creaBuffersMoneda() {
    glGenVertexArrays(1, &VAO_Moneda);
    glBindVertexArray(VAO_Moneda);

    GLuint VBO[2];
    glGenBuffers(2, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * moneda.faces().size() * 3*3,
                 moneda.VBO_vertices(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) *  moneda.faces().size() * 3*3,
                 moneda.VBO_matdiff(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matdiffLoc);

    glBindVertexArray(0);
}

void MyGLWidget::modelTransformMorty(int fila, int col) {
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col, 0.0f, fila));

    float escala = 1.5f / 312.3f;
    TG = glm::scale(TG, glm::vec3(escala));

    TG = glm::translate(TG, glm::vec3(-100.0f, 213.0f, 6.0f));

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformMoneda(int fila, int col) {
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col, 0.0f, fila));

    float escala = 0.5f / 11.0f;
    TG = glm::scale(TG, glm::vec3(escala));

    TG = glm::translate(TG, glm::vec3(0.0f, -5.5f, -0.25f));

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTorre(int fila, int col) {
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col, 0, fila));

    //Estos if's sirven para que la torre aparezca en las salidas, mire correctamente y no se coma el laberinto
    if (col == 0){
        TG = glm::rotate(TG, glm::radians(90.0f), glm::vec3(0, 1, 0));
    } else if (col == (M-1)){
        TG = glm::rotate(TG, glm::radians(-90.0f), glm::vec3(0, 1, 0));
    } else if (fila == (N-1)) {
        TG = glm::rotate(TG, glm::radians(180.0f), glm::vec3(0, 1, 0));
    }
    TG = glm::translate(TG, glm::vec3(0.0f, 0.0f, -2.5f));

    float escala = 6.0f / 172.0f;
    TG = glm::scale(TG, glm::vec3(escala));

    TG = glm::translate(TG, glm::vec3(-2, 0, -2));

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

