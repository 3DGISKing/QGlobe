/********************************************************************************
** Form generated from reading UI file 'camerakeyframedlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERAKEYFRAMEDLG_H
#define UI_CAMERAKEYFRAMEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CameraKeyFrameDlg
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QLineEdit *lineEditName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEditLongitude;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *lineEditLatitude;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_9;
    QSpacerItem *horizontalSpacer_10;
    QLineEdit *lineEditAltitude;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_4;
    QLineEdit *lineEditHeight;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_7;
    QLineEdit *lineEditHeading;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QSpacerItem *horizontalSpacer_8;
    QLineEdit *lineEditTilt;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_5;
    QLineEdit *lineEditGoToTime;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_6;
    QLineEdit *lineEditStopTime;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *pushButtonOk;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonCapture;

    void setupUi(QDialog *CameraKeyFrameDlg)
    {
        if (CameraKeyFrameDlg->objectName().isEmpty())
            CameraKeyFrameDlg->setObjectName(QString::fromUtf8("CameraKeyFrameDlg"));
        CameraKeyFrameDlg->setWindowModality(Qt::ApplicationModal);
        CameraKeyFrameDlg->resize(269, 370);
        CameraKeyFrameDlg->setMinimumSize(QSize(250, 350));
        CameraKeyFrameDlg->setMaximumSize(QSize(269, 370));
        verticalLayout = new QVBoxLayout(CameraKeyFrameDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CameraKeyFrameDlg);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        lineEditName = new QLineEdit(CameraKeyFrameDlg);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));
        lineEditName->setMinimumSize(QSize(150, 0));

        horizontalLayout->addWidget(lineEditName);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CameraKeyFrameDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        lineEditLongitude = new QLineEdit(CameraKeyFrameDlg);
        lineEditLongitude->setObjectName(QString::fromUtf8("lineEditLongitude"));
        lineEditLongitude->setMinimumSize(QSize(150, 0));

        horizontalLayout_2->addWidget(lineEditLongitude);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(CameraKeyFrameDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        lineEditLatitude = new QLineEdit(CameraKeyFrameDlg);
        lineEditLatitude->setObjectName(QString::fromUtf8("lineEditLatitude"));
        lineEditLatitude->setMinimumSize(QSize(150, 0));

        horizontalLayout_3->addWidget(lineEditLatitude);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_9 = new QLabel(CameraKeyFrameDlg);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_10->addWidget(label_9);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_10);

        lineEditAltitude = new QLineEdit(CameraKeyFrameDlg);
        lineEditAltitude->setObjectName(QString::fromUtf8("lineEditAltitude"));
        lineEditAltitude->setMinimumSize(QSize(150, 0));

        horizontalLayout_10->addWidget(lineEditAltitude);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(CameraKeyFrameDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        lineEditHeight = new QLineEdit(CameraKeyFrameDlg);
        lineEditHeight->setObjectName(QString::fromUtf8("lineEditHeight"));
        lineEditHeight->setMinimumSize(QSize(150, 0));

        horizontalLayout_4->addWidget(lineEditHeight);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_7 = new QLabel(CameraKeyFrameDlg);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_7->addWidget(label_7);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_7);

        lineEditHeading = new QLineEdit(CameraKeyFrameDlg);
        lineEditHeading->setObjectName(QString::fromUtf8("lineEditHeading"));
        lineEditHeading->setMinimumSize(QSize(150, 0));

        horizontalLayout_7->addWidget(lineEditHeading);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_8 = new QLabel(CameraKeyFrameDlg);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_8->addWidget(label_8);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_8);

        lineEditTilt = new QLineEdit(CameraKeyFrameDlg);
        lineEditTilt->setObjectName(QString::fromUtf8("lineEditTilt"));
        lineEditTilt->setMinimumSize(QSize(150, 0));

        horizontalLayout_8->addWidget(lineEditTilt);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(CameraKeyFrameDlg);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_5->addWidget(label_5);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        lineEditGoToTime = new QLineEdit(CameraKeyFrameDlg);
        lineEditGoToTime->setObjectName(QString::fromUtf8("lineEditGoToTime"));

        horizontalLayout_5->addWidget(lineEditGoToTime);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_6 = new QLabel(CameraKeyFrameDlg);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_6->addWidget(label_6);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);

        lineEditStopTime = new QLineEdit(CameraKeyFrameDlg);
        lineEditStopTime->setObjectName(QString::fromUtf8("lineEditStopTime"));

        horizontalLayout_6->addWidget(lineEditStopTime);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_9);

        pushButtonOk = new QPushButton(CameraKeyFrameDlg);
        pushButtonOk->setObjectName(QString::fromUtf8("pushButtonOk"));

        horizontalLayout_9->addWidget(pushButtonOk);

        pushButtonCancel = new QPushButton(CameraKeyFrameDlg);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));

        horizontalLayout_9->addWidget(pushButtonCancel);

        pushButtonCapture = new QPushButton(CameraKeyFrameDlg);
        pushButtonCapture->setObjectName(QString::fromUtf8("pushButtonCapture"));

        horizontalLayout_9->addWidget(pushButtonCapture);


        verticalLayout->addLayout(horizontalLayout_9);


        retranslateUi(CameraKeyFrameDlg);
        QObject::connect(pushButtonOk, SIGNAL(clicked()), CameraKeyFrameDlg, SLOT(onOk()));
        QObject::connect(pushButtonCancel, SIGNAL(clicked()), CameraKeyFrameDlg, SLOT(onCancel()));
        QObject::connect(pushButtonCapture, SIGNAL(clicked()), CameraKeyFrameDlg, SLOT(onCapture()));

        QMetaObject::connectSlotsByName(CameraKeyFrameDlg);
    } // setupUi

    void retranslateUi(QDialog *CameraKeyFrameDlg)
    {
        CameraKeyFrameDlg->setWindowTitle(QCoreApplication::translate("CameraKeyFrameDlg", "CameraKeyFrameDlg", nullptr));
        label->setText(QCoreApplication::translate("CameraKeyFrameDlg", "Name", nullptr));
        label_2->setText(QCoreApplication::translate("CameraKeyFrameDlg", "Longitude", nullptr));
        label_3->setText(QCoreApplication::translate("CameraKeyFrameDlg", "Latitude", nullptr));
        label_9->setText(QCoreApplication::translate("CameraKeyFrameDlg", "Altitude", nullptr));
        label_4->setText(QCoreApplication::translate("CameraKeyFrameDlg", "Height", nullptr));
        label_7->setText(QCoreApplication::translate("CameraKeyFrameDlg", "Heading", nullptr));
        label_8->setText(QCoreApplication::translate("CameraKeyFrameDlg", "Tilt", nullptr));
        label_5->setText(QCoreApplication::translate("CameraKeyFrameDlg", "GoToTime", nullptr));
        label_6->setText(QCoreApplication::translate("CameraKeyFrameDlg", "Stop Time", nullptr));
        pushButtonOk->setText(QCoreApplication::translate("CameraKeyFrameDlg", "OK", nullptr));
        pushButtonCancel->setText(QCoreApplication::translate("CameraKeyFrameDlg", "Cancel", nullptr));
        pushButtonCapture->setText(QCoreApplication::translate("CameraKeyFrameDlg", "Capture", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CameraKeyFrameDlg: public Ui_CameraKeyFrameDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERAKEYFRAMEDLG_H
