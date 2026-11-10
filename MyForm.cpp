#include "MyForm.h"
#include <QMessageBox>
#include <QColorDialog>
#include <QResizeEvent>

MyForm::MyForm (QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);

    // Estilo general del panel lateral
    ui.sidePanel->setStyleSheet(
        "QWidget#sidePanel {"
        "  background-color: #C8A878;"
        "  border-radius: 8px;"
        "  padding: 5px;"
        "}"
    );

    // Contador de monedas
    ui.coinsLabel->setStyleSheet(
        "QLabel {"
        "  color: #FFD700;"
        "  font-size: 18px;"
        "  font-weight: bold;"
        "  background-color: rgba(0,0,0,80);"
        "  border-radius: 6px;"
        "  padding: 6px;"
        "}"
    );

    // Botón cámara
    ui.cameraButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #5D4037;"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 6px;"
        "  padding: 6px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #795548;"
        "}"
    );

    // Botón color llum solar
    ui.lightColorButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #5D4037;"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 6px;"
        "  padding: 6px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #795548;"
        "}"
    );

    // Labels de sliders
    ui.psiLabel->setStyleSheet("QLabel { color: #3E2723; font-weight: bold; }");
    ui.thetaLabel->setStyleSheet("QLabel { color: #3E2723; font-weight: bold; }");
    ui.zoomLabel->setStyleSheet("QLabel { color: #3E2723; font-weight: bold; }");

    // Sliders
    QString sliderStyle =
        "QSlider::groove:horizontal { height: 6px; background: #8D6E63; border-radius: 3px; }"
        "QSlider::handle:horizontal { background: #3E2723; width: 14px; height: 14px; border-radius: 7px; margin: -4px 0; }";
    ui.psiSlider->setStyleSheet(sliderStyle);
    ui.thetaSlider->setStyleSheet(sliderStyle);
    ui.zoomSlider->setStyleSheet(sliderStyle);

    // Sortir
    ui.pushButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #8B0000;"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 6px;"
        "  padding: 6px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #B71C1C;" "}"
      );

    ui.rotateCoinsButton->setStyleSheet(
        "QPushButton { background-color: #2E7D32; color: white; font-weight: bold; border-radius: 6px; padding: 6px; }"
        "QPushButton:hover { background-color: #388E3C; }"
    );

    ui.legendLabel->setStyleSheet(
      "QLabel {"
      "  color: #3E2723;"
      "  font-size: 13px;"
      "  background-color: rgba(0,0,0,40);"
      "  border-radius: 6px;"
      "  padding: 6px;"
      "}"
  );

    connect(ui.widget, SIGNAL(monedaRecollida(int, int)), this, SLOT(onMonedaRecollida(int, int)));
    connect(ui.widget, SIGNAL(victoriaAconseguida()), this, SLOT(onVictoria()));
    connect(ui.widget, SIGNAL(gameOver()), this, SLOT(onGameOver()));

    connect(ui.psiSlider, SIGNAL(valueChanged(int)), this, SLOT(onPsiChanged(int)));
    connect(ui.thetaSlider, SIGNAL(valueChanged(int)), this, SLOT(onThetaChanged(int)));
    connect(ui.zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(onZoomChanged(int)));
    connect(ui.cameraButton, SIGNAL(clicked()), this, SLOT(onCameraToggle()));
    connect(ui.rotateCoinsButton, SIGNAL(clicked()), this, SLOT(onRotateCoinsToggle()));
    connect(ui.widget, SIGNAL(cameraChanged(int, int, int)), this, SLOT(onCameraChanged(int, int, int)));
    connect(ui.lightColorButton, SIGNAL(clicked()), this, SLOT(onLightColor()));

    startOverlay = new QPushButton("Start Game", ui.widget);
    startOverlay->setFixedSize(200, 60);
    startOverlay->setStyleSheet("background-color: #2E7D32; color: white; font-size: 18px; font-weight: bold; border-radius: 10px;");
    startOverlay->show();
    connect(startOverlay, SIGNAL(clicked()), this, SLOT(onStartGame()));

    messageOverlay = new QLabel("", ui.widget);
    messageOverlay->setAlignment(Qt::AlignCenter);
    messageOverlay->setFixedSize(300, 80);
    messageOverlay->setStyleSheet( "font-size: 32px; font-weight: bold; border-radius: 10px; padding: 10px;" );
    messageOverlay->hide();
}

void MyForm::onStartGame() {
    startOverlay->hide();
    messageOverlay->hide();
    if (firstTime) {
        firstTime = false;
        ui.widget->setGameStarted(true);
    } else {
        ui.widget->resetGame();
        ui.widget->setGameStarted(true);
    }
}

void MyForm::updateCoins(int collected, int total) {
    ui.coinsLabel->setText(QString("%1/%2 coins").arg(collected).arg(total));
}

void MyForm::onMonedaRecollida(int collected, int total) {
    ui.coinsLabel->setText(QString("%1/%2 coins").arg(collected).arg(total));
}

void MyForm::onVictoria() {
    ui.widget->setGameStarted(false);
    messageOverlay->setText("Victoria!");
    messageOverlay->setStyleSheet("font-size: 32px; font-weight: bold; border-radius: 10px; padding: 10px;" "color: #FFD700; background-color: rgba(0, 0, 0, 180);");
    messageOverlay->move((ui.widget->width() - messageOverlay->width()) / 2, (ui.widget->height() - messageOverlay->height()) / 2 - 120);    
    messageOverlay->show();
    startOverlay->show();
}

void MyForm::onGameOver() {
    ui.widget->setGameStarted(false);
    messageOverlay->setText("Game Over!");
    messageOverlay->setStyleSheet("font-size: 32px; font-weight: bold; border-radius: 10px; padding: 10px;" "color: #8B0000; background-color: rgba(0, 0, 0, 180);");
    messageOverlay->move((ui.widget->width() - messageOverlay->width()) / 2, (ui.widget->height() - messageOverlay->height()) / 2 - 120);
    messageOverlay->show();
    startOverlay->show();
}

void MyForm::onPsiChanged(int value) {
    ui.widget->setPsi(value);
}

void MyForm::onThetaChanged(int value) {
    ui.widget->setTheta(value);
}

void MyForm::onZoomChanged(int value) {
    ui.widget->setZoom(value);
}

void MyForm::onCameraToggle() {
    ui.widget->toggleCamera();
    if (ui.widget->isCameraFPS()) ui.cameraButton->setText("Càmera General");
    else ui.cameraButton->setText("Càmera FPS");
}

void MyForm::onCameraChanged(int psi, int theta, int zoom) {
    ui.psiSlider->blockSignals(true);
    ui.thetaSlider->blockSignals(true);
    ui.zoomSlider->blockSignals(true);

    ui.psiSlider->setValue(psi);
    ui.thetaSlider->setValue(theta);
    ui.zoomSlider->setValue(zoom);

    ui.psiSlider->blockSignals(false);
    ui.thetaSlider->blockSignals(false);
    ui.zoomSlider->blockSignals(false);
}

void MyForm::onLightColor() {
    QColor color = QColorDialog::getColor(Qt::white, this, "Color de la llum solar");
    if (color.isValid()) {
        ui.widget->setLightColor(
            color.redF(),
            color.greenF(),
            color.blueF()
        );
    }
}

void MyForm::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    startOverlay->move(
        (ui.widget->width() - startOverlay->width()) / 2,
        (ui.widget->height() - startOverlay->height()) / 2
    );
}

void MyForm::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    if (!overlayInitialized) {
        overlayInitialized = true;
        startOverlay->move((ui.widget->width() - startOverlay->width()) / 2, (ui.widget->height() - startOverlay->height()) / 2);
    }
}

void MyForm::onRotateCoinsToggle() {
    coinsRotating = !coinsRotating;
    ui.widget->setRotateCoins(coinsRotating);
    if (coinsRotating) {
        ui.rotateCoinsButton->setText("Rotar monedes: ON");
        ui.rotateCoinsButton->setStyleSheet(
            "QPushButton { background-color: #2E7D32; color: white; font-weight: bold; border-radius: 6px; padding: 6px; }"
            "QPushButton:hover { background-color: #388E3C; }"
        );
    } else {
        ui.rotateCoinsButton->setText("Rotar monedes: OFF");
        ui.rotateCoinsButton->setStyleSheet(
            "QPushButton { background-color: #8B0000; color: white; font-weight: bold; border-radius: 6px; padding: 6px; }"
            "QPushButton:hover { background-color: #B71C1C; }"
        );
    }
}
