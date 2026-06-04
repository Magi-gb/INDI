
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include<utility>

#define CHECK() printOglError(__FILE__, __LINE__, __FUNCTION__)
#define DEBUG(text) std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << ":" << text << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent)
    : BL2GLWidget(parent)
{
    connect(&timer, SIGNAL(timeout()),
            this, SLOT(rotateCoins()));

    timer.start(16);
}

MyGLWidget::~MyGLWidget() {}

void MyGLWidget::initializeGL() {
    BL2GLWidget::initializeGL();

    morty.load("../Models3D/Morty.obj"); 
    moneda.load("../Models3D/Coin.obj");
    fantasma.load("../Models3D/Fantasma.obj");

    findMorty();
    findGhost();
    calculaCapsaEscena();
    initCamera();
    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    updateLightPosition();
    creaBuffersMorty();
    creaBuffersAssimp();
    creaBuffersMoneda();
    creaBuffersFantasma();

    //TEXTURAS Y RESIZE DEL CUB
    glm::vec3 bbMinWall  = wallMesh->GetBBMin();
    glm::vec3 bbSizeWall = wallMesh->GetBBSize();
    float maxSizeWall = std::max(bbSizeWall.x, std::max(bbSizeWall.y, bbSizeWall.z));
    escalaWall = 1.0f / maxSizeWall;
    centreBaseWall = glm::vec3(bbMinWall.x + bbSizeWall.x / 2.0f, bbMinWall.y, bbMinWall.z + bbSizeWall.z / 2.0f);

    //TEXTURAS Y RESIZE DE LA TORRE
    glm::vec3 bbMin  = towerMesh->GetBBMin();
    glm::vec3 bbSize = towerMesh->GetBBSize();
    float maxSize = std::max(bbSize.x, std::max(bbSize.y, bbSize.z));
    escalaTorre = 6.0f / maxSize;
    centreBaseTorre = glm::vec3(bbMin.x + bbSize.x / 2.0f, bbMin.y, bbMin.z + bbSize.z / 2.0f);

    radioBaseTorre = (bbSize.z / 2.0f) * escalaTorre;

    calculaCapsaModel(
        morty,
        escalaMorty,
        centreBaseMorty,
        1.5f
    );

    /*calculaCapsaModel(
        torre,
        escalaTorre,
        centreBaseTorre,
        6.0f
    );*/

    calculaCapsaModel(
        moneda,
        escalaMoneda,
        centreBaseMoneda,
        0.5f
    );

    calculaCapsaModel(
        fantasma,
        escalaFantasma,
        centreBaseMoneda,
        0.65f
    );

    glEnable(GL_DEPTH_TEST);
}


void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, width(), height());

    projectTransform();

    viewTransform();

    sendLightUniforms();

    renderScene();

    glViewport(width() - width()/4, 0, width()/4, height()/4);

    setupMiniMapCamera();

    renderScene();

    glBindVertexArray(0);
}

void MyGLWidget::renderScene() {
    for (int i = 0; i < N; i++) {

        for (int j = 0; j < M; j++) {

            if (laberint[i][j] == 1){

                modelTransformCell(i, j);

                wallMesh->Render();
            } else {

                if (laberint[i][j] == 2) {

                    glBindVertexArray(VAO_Morty);

                    modelTransformMorty(i, j);

                    glDrawArrays(GL_TRIANGLES, 0, morty.faces().size() * 3);
                } else if (laberint[i][j] == 3) {

                    glBindVertexArray(VAO_Fantasma);

                    modelTransformFantasma(i, j);

                    glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size() *3);
                } else if (laberint[i][j] == 4) {

                    modelTransformTorre(i, j);

                    towerMesh->Render();
                } else if (laberint[i][j] == 5) {

                    glBindVertexArray(VAO_Moneda);

                    modelTransformMoneda(i, j);

                    glDrawArrays(GL_TRIANGLES, 0, moneda.faces().size() * 3);
                }
            }
            modelTransformCellT(i, j);

            wallMesh->Render();
        }
    }
}

void MyGLWidget::setupMiniMapCamera() {
    OBSOrto = centroEscena + glm::vec3(0.0f, 3.0f * radioEscena, 0.0f);

    glUniform3fv(viewPosLoc, 1, &OBSOrto[0]);

    glUniform1i(nightModeLoc, false);

    projectTransformOrtho();

    viewTransformOrtho();
}

void MyGLWidget::sendLightUniforms() {

    glUniform3fv(lightPosLoc, 1, &lightPos[0]);

    glUniform3fv(lightColorLoc, 1, &lightColor[0]);

    glm::vec3 cameraPos;

    if (cameraFPS)
        cameraPos = obsFPS;
    else
        cameraPos = obsPerspectiva;

    glUniform3fv(viewPosLoc, 1, &cameraPos[0]);

    glUniform1i(nightModeLoc, nightMode);

    glm::vec3 flashlightPos(
        mortyCol + 0.5f,
        0.5f,
        mortyFila + 0.5f
    );

    glm::vec3 flashlightDir;

    switch (dirMorty) {

        case 0:
            flashlightDir = glm::vec3(0,0,-1);
            break;

        case 1:
            flashlightDir = glm::vec3(1,0,0);
            break;

        case 2:
            flashlightDir = glm::vec3(0,0,1);
            break;

        case 3:
            flashlightDir = glm::vec3(-1,0,0);
            break;

        default:
            flashlightDir = glm::vec3(1,0,0);
    }

    glUniform3fv(flashlightPosLoc, 1, &flashlightPos[0]);

    glUniform3fv(flashlightDirLoc, 1, &flashlightDir[0]);

    glm::vec3 ghostLightPos(fantasmaCol + 0.5f, 1.0f, fantasmaFila + 0.5f);

    glm::vec3 ghostLightColor(1.0f, 1.0f, 1.0f);

    glUniform3fv(ghostLightPosLoc, 1, &ghostLightPos[0]);

    glUniform3fv(ghostLightColorLoc, 1, &ghostLightColor[0]);

    glm::vec3 coinPositions[10];
    glm::vec3 coinDirections[10];

    int k = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {

            if (laberint[i][j] == 5) {
                coinPositions[k] = glm::vec3(j + 0.5f, 0.5f, i + 0.5f);

                float a = glm::radians(angleCoin);

                coinDirections[k] = glm::vec3(-sin(a), 0.0f, -cos(a));

                k++;
            }
        }
    }

    glUniform3fv(coinLightPosLoc, 10, &coinPositions[0].x);

    glUniform3fv(coinLightDirLoc, 10, &coinDirections[0].x);
}

void MyGLWidget::resizeGL(int w, int h) {

    glViewport(0, 0, w, h);

    projectTransform();
}

void MyGLWidget::carregaShaders() {

    BL2GLWidget::carregaShaders();

    PMLoc = glGetUniformLocation(program->programId(), "PM");

    VMLoc = glGetUniformLocation(program->programId(), "VM");

    TG_Loc = glGetUniformLocation(program->programId(), "TG");

    lightPosLoc = glGetUniformLocation(program->programId(), "lightPos");

    lightColorLoc = glGetUniformLocation(program->programId(), "lightColor");

    viewPosLoc = glGetUniformLocation(program->programId(), "viewPos");

    nightModeLoc = glGetUniformLocation(program->programId(), "nightMode");

    flashlightPosLoc = glGetUniformLocation(program->programId(), "flashlightPos");

    flashlightDirLoc = glGetUniformLocation(program->programId(), "flashlightDir");

    ghostLightPosLoc = glGetUniformLocation(program->programId(), "ghostLightPos");

    ghostLightColorLoc = glGetUniformLocation(program->programId(), "ghostLightColor");

    coinLightPosLoc = glGetUniformLocation(program->programId(), "coinLightPos");

    coinLightDirLoc = glGetUniformLocation(program->programId(), "coinLightDir");

    texUVLoc = glGetAttribLocation(program->programId(), "texCoord");
    
    colorMapLoc = glGetUniformLocation(program->programId(), "colorMap");
}

void MyGLWidget::initCamera() {

    fovPerspectiva = glm::radians(60.0f);

    distCamera = radioEscena / sin(fovPerspectiva / 2.0f);

    glm::vec3 dirCamera(0.0f, 1.0f, 1.0f);

    dirCamera = glm::normalize(dirCamera);

    obsPerspectiva = centroEscena + dirCamera * distCamera;

    vrpPerspectiva = centroEscena;

    fovFPS = glm::radians(90.0f);

    projectTransform();
    viewTransform();
}

void MyGLWidget::projectTransform() {

    float ra = float(width()) / float(height());

    float FOV = fovPerspectiva;

    // Evitar recorte vertical
    if (ra < 1.0f) {

        FOV = 2.0f * atan( tan(fovPerspectiva / 2.0f) / ra);
    }

    if (cameraFPS) {

        PM = glm::perspective( fovFPS, ra, 0.2f, 200.0f);
    }

    else {

        float near = distCamera - radioEscena;
        float far  = distCamera + radioEscena;

        // Seguridad
        if (near < 0.1f)
            near = 0.1f;

        PM = glm::perspective( FOV, ra, near, far);
    }

    glUniformMatrix4fv(PMLoc, 1, GL_FALSE, &PM[0][0]);
}

void MyGLWidget::viewTransform() {

    if (!cameraFPS) {

    VM = glm::lookAt(obsPerspectiva, vrpPerspectiva, glm::vec3(0,1,0));

    } else {

        // Posicion de la cabeza de Morty
        obsFPS = glm::vec3(mortyCol + 0.5f, 0.5f, mortyFila + 0.5f);

        // Direccion hacia donde mira Morty segun dirMorty
        glm::vec3 dir;

        switch (dirMorty) {
            case 0:
                dir = glm::vec3(0.0f, 0.0f, -1.0f); // Norte
                break;

            case 1:
                dir = glm::vec3(1.0f, 0.0f, 0.0f);  // Este
                break;

            case 2:
                dir = glm::vec3(0.0f, 0.0f, 1.0f);  // Sur
                break;

            case 3:
                dir = glm::vec3(-1.0f, 0.0f, 0.0f); // Oeste
                break;

            default:
                dir = glm::vec3(1.0f, 0.0f, 0.0f);
                break;
        }
        vrpFPS = obsFPS + dir;

        VM = glm::lookAt(obsFPS, vrpFPS, glm::vec3(0, 1, 0));
    }

    glUniformMatrix4fv(VMLoc, 1, GL_FALSE, &VM[0][0]);
}

//Las dos funciones para proyectar la vista ortogonal
void MyGLWidget::projectTransformOrtho() {

    float ample = maxEscena.x - minEscena.x;
    float profunditat = maxEscena.z - minEscena.z;

    float size = std::max(ample, profunditat) / 2.0f;

    PM = glm::ortho(-size, size, -size, size, 0.1f, 4.0f * radioEscena);

    glUniformMatrix4fv(PMLoc, 1, GL_FALSE, &PM[0][0]);
}

void MyGLWidget::viewTransformOrtho() {

    glm::vec3 OBS = centroEscena + glm::vec3(0.0f, 3.0f * radioEscena, 0.0f);

    glm::vec3 VRP = centroEscena;

    VM = glm::lookAt(OBS, VRP, glm::vec3(0,0,-1));

    glUniformMatrix4fv(VMLoc, 1, GL_FALSE, &VM[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent *event) {

    makeCurrent();

    bool calActualitzar = true;

    switch(event->key()) {

    // ===================================
    // CAMARA
    // ===================================

        case Qt::Key_C:
            cameraFPS = !cameraFPS;
            projectTransform();
            viewTransform();
            break;

    // ===================================
    // MOVIMENT
    // ===================================

        case Qt::Key_W:
        case Qt::Key_Up:
            mouMortyEndavant();
            break;

        case Qt::Key_S:
        case Qt::Key_Down:
            mouMortyEnrere();
            break;

        case Qt::Key_A:
        case Qt::Key_Left:
            giraMortyEsquerra();
            break;
        
        case Qt::Key_D:
        case Qt::Key_Right:
            giraMortyDreta();
            break;

    // ===================================
    // SOL
    // ===================================

        case Qt::Key_O:
            angleSol += glm::radians(5.0f);
            updateLightPosition();
            break;

        case Qt::Key_P:
            angleSol -= glm::radians(5.0f);
            updateLightPosition();
            break;
        case Qt::Key_N:
            nightMode = !nightMode;
            update();
            break;

    default:
        calActualitzar = false;
        BL2GLWidget::keyPressEvent(event);
        break;
}

    if (calActualitzar) {
        if (cameraFPS) {
            viewTransform();
        }
        update();
    }
}

void MyGLWidget::mousePressEvent(QMouseEvent *e) {

    // Rotacion Euler
    if (e->button() == Qt::LeftButton) {

        rotating = true;

        xClick = e->x();
        yClick = e->y();
    }

    // Zoom
    if (e->button() == Qt::RightButton) {

        zooming = true;

        yClick = e->y();
    }
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *e) {

    if (e->button() == Qt::LeftButton) {

        rotating = false;
    }

    if (e->button() == Qt::RightButton) {

        zooming = false;
    }
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {

    if (rotating && !cameraFPS) {

    angleY += (e->x() - xClick) * 0.01f;

    angleX += (e->y() - yClick) * 0.01f;

    xClick = e->x();
    yClick = e->y();

    // Direccion inicial de la camara
    glm::vec3 dir(0.0f, 1.0f, 1.0f);

    // Matriz de rotacion Euler
    glm::mat4 R(1.0f);

    R = glm::rotate(R, angleY, glm::vec3(0,1,0));

    R = glm::rotate(R, angleX, glm::vec3(1,0,0));

    // Rotar direccion
    dir = glm::vec3(R * glm::vec4(dir, 0.0f));

    dir = glm::normalize(dir);

    // Orbitar alrededor del centro del mapa
    obsPerspectiva = centroEscena + dir * distCamera;

    vrpPerspectiva = centroEscena;

    viewTransform();

    update();
    }

    if (zooming && !cameraFPS) {

    float dy = e->y() - yClick;

    distCamera += dy * 0.05f;

    if (distCamera < radioEscena)
        distCamera = radioEscena;

    glm::vec3 dirCamera(0.0f, 1.0f, 1.0f);

    dirCamera = glm::normalize(dirCamera);

    obsPerspectiva = centroEscena +
                     dirCamera * distCamera;

    yClick = e->y();

    projectTransform();
    viewTransform();

    update();
    }
}

void MyGLWidget::findMorty() {

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < M; j++) {

            if (laberint[i][j] == 2) {

                mortyFila = i;
                mortyCol = j;

                return;
            }
        }
    }
}

void MyGLWidget::findGhost() {
    for (int i=0; i<N; i++) {
        for (int j=0; j<M; j++) {

            if (laberint[i][j] == 3) {

                fantasmaFila = i;
                fantasmaCol = j;

                return;
            }
        }
    }
}

bool MyGLWidget::casellaLliure(int fila, int col) const {
    if (fila < 0 || fila >= N) return false;
    if (col < 0 || col >= M) return false;

    return (laberint[fila][col] != 1 &&
            laberint[fila][col] != 4);
}

std::vector<int> MyGLWidget::direccionsPossiblesFantasma() const {

    std::vector<int> dirs;

    const int df[4] = {-1,0,1,0};
    const int dc[4] = {0,1,0,-1};

    for (int d = 0; d < 4; d++) {

        int f = fantasmaFila + df[d];
        int c = fantasmaCol + dc[d];

        if (casellaLliure(f, c))
            dirs.push_back(d);
    }

    return dirs;
}

void MyGLWidget::moveMorty(int df, int dc) {
    int novaFila = mortyFila + df;
    int novaCol = mortyCol + dc;

    // Evitar salirnos de la matriz
    if (novaFila < 0 || novaFila >= N || novaCol < 0 || novaCol >= M) {
        return;
    }

    // No puede atravesar paredes
    if (laberint[novaFila][novaCol] == 1) {
        return;
    }

    // Si quieres impedir que entre en torres, deja esto
    if (laberint[novaFila][novaCol] == 4) {
        return;
    }

    // Si pisa una moneda, aquí podrías sumar puntos
    if (laberint[novaFila][novaCol] == 5) {
        std::cout << "Moneda recogida!" << std::endl;
    }

    // Actualizar matriz
    laberint[mortyFila][mortyCol] = 0;
    laberint[novaFila][novaCol] = 2;

    // Actualizar posición lógica
    mortyFila = novaFila;
    mortyCol = novaCol;
}

void MyGLWidget::moveFantasma(int df, int dc) {

    int novaFila = fantasmaFila + df;
    int novaCol = fantasmaCol + dc;

    if (novaFila < 0 || novaFila >= N ||
        novaCol < 0 || novaCol >= M)
        return;

    if (laberint[novaFila][novaCol] == 1)
        return;

    if (laberint[novaFila][novaCol] == 4)
        return;

    if (laberint[novaFila][novaCol] == 5)
        return; // opcional

    laberint[fantasmaFila][fantasmaCol] = 0;
    laberint[novaFila][novaCol] = 3;

    fantasmaFila = novaFila;
    fantasmaCol = novaCol;
}

void MyGLWidget::mouMortyEndavant() {

    switch (dirMorty) {
        case 0:
            moveMorty(-1, 0);
            break;

        case 1:
            moveMorty(0, 1);
            break;

        case 2:
            moveMorty(1, 0);
            break;

        case 3:
            moveMorty(0, -1);
            break;
    }
    mouFantasma();
}

void MyGLWidget::mouMortyEnrere() {

    switch (dirMorty) {
        case 0:
            moveMorty(1, 0);
            break;

        case 1:
            moveMorty(0, -1);
            break;

        case 2:
            moveMorty(-1, 0);
            break;

        case 3:
            moveMorty(0, 1);
            break;
    }
    mouFantasma();
}

void MyGLWidget::mouFantasma() {

    auto dirs = direccionsPossiblesFantasma();

    glm::ivec2 dirActual = direccioFantasma();

    bool bloquejat = !casellaLliure(fantasmaFila + dirActual.x, fantasmaCol  + dirActual.y);

    bool interseccio = dirs.size() > 2;

    if (bloquejat || interseccio) {

        int idx = rand() % dirs.size();

        dirFantasma = dirs[idx];
    }

    glm::ivec2 dir = direccioFantasma();

    moveFantasma(dir.x, dir.y);
}

void MyGLWidget::giraMortyEsquerra() {
    dirMorty = (dirMorty + 3) % 4;
}

void MyGLWidget::giraMortyDreta() {
    dirMorty = (dirMorty + 1) % 4;
}

float MyGLWidget::angleFromDirMorty() const {

    switch (dirMorty) {
        case 0:
            return 180.0f;      // Norte

        case 1:
            return 90.0f;       // Este

        case 2:
            return 0.0f;        // Sur

        case 3:
            return -90.0f;      // Oeste;
    }

    return -90.0f;
}

glm::vec3 MyGLWidget::direccioMiradaMorty() const {

    switch (dirMorty) {
        case 0:
            return glm::vec3(0.0f, 0.0f, -1.0f); // Norte, fila -1

        case 1:
            return glm::vec3(1.0f, 0.0f, 0.0f);  // Este, col +1

        case 2:
            return glm::vec3(0.0f, 0.0f, 1.0f);  // Sur, fila +1

        case 3:
            return glm::vec3(-1.0f, 0.0f, 0.0f); // Oeste, col -1
    }

    return glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::ivec2 MyGLWidget::direccioFantasma() const {

    switch(dirFantasma) {

        case 0:
            return {-1,0}; // Norte

        case 1:
            return {0,1}; // Este

        case 2:
            return {1,0}; // Sur

        case 3:
            return {0,-1}; // Oeste
    }

    return {0,1};
}

//Operació per l'escala dels models
void MyGLWidget::calculaCapsaModel(Model &m, float &escala, glm::vec3 &centreBaseModel, float desiredSize) {

    float minx, miny, minz;
    float maxx, maxy, maxz;

    minx = maxx = m.vertices()[0];
    miny = maxy = m.vertices()[1];
    minz = maxz = m.vertices()[2];

    for (unsigned int i = 3; i < m.vertices().size(); i += 3) {

        float x = m.vertices()[i];
        float y = m.vertices()[i+1];
        float z = m.vertices()[i+2];

        minx = std::min(minx, x);
        maxx = std::max(maxx, x);

        miny = std::min(miny, y);
        maxy = std::max(maxy, y);

        minz = std::min(minz, z);
        maxz = std::max(maxz, z);
    }

    // Tamaño máximo del modelo
    float sizeX = maxx - minx;
    float sizeY = maxy - miny;
    float sizeZ = maxz - minz;

    float maxSize = std::max(sizeX, std::max(sizeY, sizeZ));

    escala = desiredSize / maxSize;

    // Centro de la base
    centreBaseModel = glm::vec3((minx + maxx) / 2.0f, miny, (minz + maxz) / 2.0f);
}

void MyGLWidget::calculaCapsaEscena() {

    float marge = 3.0f;

    minEscena = glm::vec3( -marge, 0.0f, -marge);

    maxEscena = glm::vec3( M + marge, 6.0f, N + marge);

    centroEscena = (minEscena + maxEscena) / 2.0f;

    radioEscena = glm::distance(maxEscena, centroEscena);
}

void MyGLWidget::rotateCoins() {

    angleCoin += 2.0f;

    if (angleCoin >= 360.0f)
        angleCoin -= 360.0f;

    update();
}

void MyGLWidget::modelTransformCell(int fila, int col) {
    
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col + 0.5f, 0.0f, fila + 0.5f));
    
    TG = glm::scale(TG, glm::vec3(escalaWall));
    
    TG = glm::translate(TG, -centreBaseWall);
    
    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCellT(int fila, int col) {
    
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col + 0.5f, -0.1f, fila + 0.5f));
    
    TG = glm::scale(TG, glm::vec3(escalaWall, escalaWall * 0.1f, escalaWall));
    
    TG = glm::translate(TG, -centreBaseWall);
    
    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::creaBuffersMorty() {

    glGenVertexArrays(1, &VAO_Morty);
    glBindVertexArray(VAO_Morty);

    GLuint VBO[3];
    glGenBuffers(3, VBO);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * morty.faces().size() * 3 * 3, morty.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // Material diffuse
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * morty.faces().size() * 3 * 3, morty.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer( matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matdiffLoc);

    // Normales
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * morty.faces().size() * 3 * 3, morty.VBO_normals(), GL_STATIC_DRAW);
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalLoc);

    glBindVertexArray(0);
}

void MyGLWidget::creaBuffersMoneda() {
    glGenVertexArrays(1, &VAO_Moneda);
    glBindVertexArray(VAO_Moneda);

    GLuint VBO[3];
    glGenBuffers(3, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * moneda.faces().size() * 3*3, moneda.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *  moneda.faces().size() * 3*3, moneda.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matdiffLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * moneda.faces().size() * 3 * 3, moneda.VBO_normals(), GL_STATIC_DRAW);
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalLoc);

    glBindVertexArray(0);
}

void MyGLWidget::creaBuffersFantasma() {
    glGenVertexArrays(1, &VAO_Fantasma);
    glBindVertexArray(VAO_Fantasma);

    GLuint VBO[3];
    glGenBuffers(3, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fantasma.faces().size() * 3*3, fantasma.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *  fantasma.faces().size() * 3*3, fantasma.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matdiffLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fantasma.faces().size() * 3 * 3, fantasma.VBO_normals(), GL_STATIC_DRAW);
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalLoc);

    glBindVertexArray(0);
}

void MyGLWidget::creaBuffersAssimp() {
    QOpenGLFunctions_3_3_Core* f = this;  // cast explícito

    wallMesh = new Mesh(f, vertexLoc, normalLoc, texUVLoc, matdiffLoc, matspecLoc, matambLoc, matshinLoc);
    wallMesh->LoadMesh("../Models3D/block.obj");

    towerMesh = new Mesh(f, vertexLoc, normalLoc, texUVLoc, matdiffLoc, matspecLoc, matambLoc, matshinLoc);
    towerMesh->LoadMesh("../Models3D/tower.obj");
}

void MyGLWidget::modelTransformMorty(int fila, int col) {
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(col, 0.0f, fila));

    TG = glm::translate(TG, glm::vec3(0.5, 0, 0.5));

    TG = glm::rotate(TG, glm::radians(angleFromDirMorty()), glm::vec3(0, 1, 0));

    TG = glm::scale(TG, glm::vec3(escalaMorty));

    TG = glm::translate(TG, -centreBaseMorty);

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformMoneda(int fila, int col) {

    glm::mat4 TG(1.0f);

    TG = glm::translate( TG, glm::vec3(col + 0.5f, 0.5f, fila + 0.5f));

    TG = glm::rotate( TG, glm::radians(angleCoin), glm::vec3(0,1,0));

    TG = glm::scale(TG, glm::vec3(escalaMoneda));

    TG = glm::translate(TG, -centreBaseMoneda);

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTorre(int fila, int col) {
    glm::mat4 TG(1.0f);

    // Trasladar al centro de la celda
    TG = glm::translate(TG, glm::vec3(col + 0.5f, 0.0f, fila + 0.5f));

    // Rotar según en qué borde del laberinto está
    if (col == 0){
        TG = glm::rotate(TG, glm::radians(90.0f), glm::vec3(0, 1, 0));
    } else if (col == (M-1)){
        TG = glm::rotate(TG, glm::radians(-90.0f), glm::vec3(0, 1, 0));
    } else if (fila == (N-1)) {
        TG = glm::rotate(TG, glm::radians(180.0f), glm::vec3(0, 1, 0));
    }
    // fila == 0 no necesita rotación, ya mira hacia dentro

    // Empujar la torre hacia atrás para que la puerta quede en el borde de la celda
    TG = glm::translate(TG, glm::vec3(0.0f, 0.0f, -(radioBaseTorre - 0.5f)));

    TG = glm::scale(TG, glm::vec3(escalaTorre));

    TG = glm::translate(TG, -centreBaseTorre);

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma(int fila, int col) {

    glm::mat4 TG(1.0f);

    TG = glm::translate( TG, glm::vec3(col + 0.5f, 0.0f, fila + 0.5f));

    TG = glm::scale(TG, glm::vec3(escalaFantasma));

    glUniformMatrix4fv(TG_Loc, 1, GL_FALSE, &TG[0][0]);
}

//Funcions llum
void MyGLWidget::updateLightPosition() {

    lightPos.x = centroEscena.x + radioEscena * cos(angleSol);
    lightPos.y = centroEscena.y + radioEscena * sin(angleSol);
    lightPos.z = centroEscena.z;
}
