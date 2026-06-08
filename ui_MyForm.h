/********************************************************************************
** Form generated from reading UI file 'MyForm.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYFORM_H
#define UI_MYFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "MyGLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MyForm
{
public:
    QHBoxLayout *horizontalLayout;
    MyGLWidget *widget;
    QWidget *sidePanel;
    QVBoxLayout *verticalLayout;
    QLabel *coinsLabel;
    QLabel *psiLabel;
    QSlider *psiSlider;
    QLabel *thetaLabel;
    QSlider *thetaSlider;
    QLabel *zoomLabel;
    QSlider *zoomSlider;
    QPushButton *cameraButton;
    QPushButton *rotateCoinsButton;
    QPushButton *lightColorButton;
    QSpacerItem *verticalSpacer;
    QLabel *legendLabel;
    QPushButton *pushButton;

    void setupUi(QWidget *MyForm)
    {
        if (MyForm->objectName().isEmpty())
            MyForm->setObjectName(QString::fromUtf8("MyForm"));
        MyForm->resize(819, 726);
        horizontalLayout = new QHBoxLayout(MyForm);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget = new MyGLWidget(MyForm);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(50, 50));

        horizontalLayout->addWidget(widget);

        sidePanel = new QWidget(MyForm);
        sidePanel->setObjectName(QString::fromUtf8("sidePanel"));
        sidePanel->setMaximumWidth(200);
        verticalLayout = new QVBoxLayout(sidePanel);
        verticalLayout->setSpacing(8);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        coinsLabel = new QLabel(sidePanel);
        coinsLabel->setObjectName(QString::fromUtf8("coinsLabel"));

        verticalLayout->addWidget(coinsLabel);

        psiLabel = new QLabel(sidePanel);
        psiLabel->setObjectName(QString::fromUtf8("psiLabel"));

        verticalLayout->addWidget(psiLabel);

        psiSlider = new QSlider(sidePanel);
        psiSlider->setObjectName(QString::fromUtf8("psiSlider"));
        psiSlider->setOrientation(Qt::Horizontal);
        psiSlider->setMinimum(-180);
        psiSlider->setMaximum(180);

        verticalLayout->addWidget(psiSlider);

        thetaLabel = new QLabel(sidePanel);
        thetaLabel->setObjectName(QString::fromUtf8("thetaLabel"));

        verticalLayout->addWidget(thetaLabel);

        thetaSlider = new QSlider(sidePanel);
        thetaSlider->setObjectName(QString::fromUtf8("thetaSlider"));
        thetaSlider->setOrientation(Qt::Horizontal);
        thetaSlider->setMinimum(-90);
        thetaSlider->setMaximum(90);

        verticalLayout->addWidget(thetaSlider);

        zoomLabel = new QLabel(sidePanel);
        zoomLabel->setObjectName(QString::fromUtf8("zoomLabel"));

        verticalLayout->addWidget(zoomLabel);

        zoomSlider = new QSlider(sidePanel);
        zoomSlider->setObjectName(QString::fromUtf8("zoomSlider"));
        zoomSlider->setOrientation(Qt::Horizontal);
        zoomSlider->setMinimum(0);
        zoomSlider->setMaximum(100);

        verticalLayout->addWidget(zoomSlider);

        cameraButton = new QPushButton(sidePanel);
        cameraButton->setObjectName(QString::fromUtf8("cameraButton"));

        verticalLayout->addWidget(cameraButton);

        rotateCoinsButton = new QPushButton(sidePanel);
        rotateCoinsButton->setObjectName(QString::fromUtf8("rotateCoinsButton"));

        verticalLayout->addWidget(rotateCoinsButton);

        lightColorButton = new QPushButton(sidePanel);
        lightColorButton->setObjectName(QString::fromUtf8("lightColorButton"));

        verticalLayout->addWidget(lightColorButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        legendLabel = new QLabel(sidePanel);
        legendLabel->setObjectName(QString::fromUtf8("legendLabel"));
        legendLabel->setWordWrap(true);

        verticalLayout->addWidget(legendLabel);

        pushButton = new QPushButton(sidePanel);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(pushButton);


        horizontalLayout->addWidget(sidePanel);


        retranslateUi(MyForm);
        QObject::connect(pushButton, SIGNAL(clicked()), MyForm, SLOT(close()));

        QMetaObject::connectSlotsByName(MyForm);
    } // setupUi

    void retranslateUi(QWidget *MyForm)
    {
        MyForm->setWindowTitle(QCoreApplication::translate("MyForm", "IDI-Lab", nullptr));
        coinsLabel->setText(QCoreApplication::translate("MyForm", "0/0 coins", nullptr));
        psiLabel->setText(QCoreApplication::translate("MyForm", "Rotaci\303\263 horitzontal (Psi)", nullptr));
        thetaLabel->setText(QCoreApplication::translate("MyForm", "Rotaci\303\263 vertical (Theta)", nullptr));
        zoomLabel->setText(QCoreApplication::translate("MyForm", "Zoom", nullptr));
        cameraButton->setText(QCoreApplication::translate("MyForm", "C\303\240mera FPS", nullptr));
        rotateCoinsButton->setText(QCoreApplication::translate("MyForm", "Rotar monedes: ON", nullptr));
        lightColorButton->setText(QCoreApplication::translate("MyForm", "Color llum solar", nullptr));
        legendLabel->setText(QCoreApplication::translate("MyForm", "\342\200\224 MOVIMENT \342\200\224\n"
"W/\342\206\221 Endavant\n"
"S/\342\206\223 Enrere\n"
"A/\342\206\220 Girar esquerra\n"
"D/\342\206\222 Girar dreta\n"
"\n"
"\342\200\224 C\303\200MERA \342\200\224\n"
"C Canviar c\303\240mera\n"
"+/- Zoom\n"
"\n"
"\342\200\224 LLUM \342\200\224\n"
"O/P Rotar sol\n"
"N Mode nocturn", nullptr));
        pushButton->setText(QCoreApplication::translate("MyForm", "&Sortir", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyForm: public Ui_MyForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYFORM_H
