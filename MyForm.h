#include "ui_MyForm.h"
#include <QPushButton>
#include <QLabel>

class MyForm : public QWidget
{
  Q_OBJECT

  public:
    MyForm (QWidget *parent=0);
    void updateCoins(int collected, int total);

private slots:
    void onStartGame();
    void onMonedaRecollida(int collected, int total);
    void onVictoria();
    void onGameOver();
    void onPsiChanged(int value);
    void onThetaChanged(int value);
    void onZoomChanged(int value);
    void onCameraToggle();
    void onRotateCoinsToggle();
    void onCameraChanged(int psi, int theta, int zoom);
    void onLightColor();
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

  private:
    Ui::MyForm ui;
    QPushButton *startOverlay;
    QLabel *messageOverlay;
    bool overlayInitialized = false;
    bool firstTime = true;
    bool coinsRotating = true;

};