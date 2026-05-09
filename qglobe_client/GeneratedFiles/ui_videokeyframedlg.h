/********************************************************************************
** Form generated from reading UI file 'videokeyframedlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOKEYFRAMEDLG_H
#define UI_VIDEOKEYFRAMEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_VideoKeyFrameDlg
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEditFileName;
    QPushButton *pushButtonVideoBrowser;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QSpinBox *spinBoxTime;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonOk;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *VideoKeyFrameDlg)
    {
        if (VideoKeyFrameDlg->objectName().isEmpty())
            VideoKeyFrameDlg->setObjectName(QString::fromUtf8("VideoKeyFrameDlg"));
        VideoKeyFrameDlg->resize(429, 112);
        verticalLayout = new QVBoxLayout(VideoKeyFrameDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(VideoKeyFrameDlg);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEditFileName = new QLineEdit(VideoKeyFrameDlg);
        lineEditFileName->setObjectName(QString::fromUtf8("lineEditFileName"));

        horizontalLayout->addWidget(lineEditFileName);

        pushButtonVideoBrowser = new QPushButton(VideoKeyFrameDlg);
        pushButtonVideoBrowser->setObjectName(QString::fromUtf8("pushButtonVideoBrowser"));

        horizontalLayout->addWidget(pushButtonVideoBrowser);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(VideoKeyFrameDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer_2 = new QSpacerItem(44, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        spinBoxTime = new QSpinBox(VideoKeyFrameDlg);
        spinBoxTime->setObjectName(QString::fromUtf8("spinBoxTime"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(spinBoxTime->sizePolicy().hasHeightForWidth());
        spinBoxTime->setSizePolicy(sizePolicy);
        spinBoxTime->setMaximum(999999999);

        horizontalLayout_2->addWidget(spinBoxTime);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushButtonOk = new QPushButton(VideoKeyFrameDlg);
        pushButtonOk->setObjectName(QString::fromUtf8("pushButtonOk"));

        horizontalLayout_3->addWidget(pushButtonOk);

        pushButtonCancel = new QPushButton(VideoKeyFrameDlg);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));

        horizontalLayout_3->addWidget(pushButtonCancel);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(VideoKeyFrameDlg);

        QMetaObject::connectSlotsByName(VideoKeyFrameDlg);
    } // setupUi

    void retranslateUi(QDialog *VideoKeyFrameDlg)
    {
        VideoKeyFrameDlg->setWindowTitle(QCoreApplication::translate("VideoKeyFrameDlg", "VideoKeyFrameDlg", nullptr));
        label->setText(QCoreApplication::translate("VideoKeyFrameDlg", "Video Filename", nullptr));
        pushButtonVideoBrowser->setText(QCoreApplication::translate("VideoKeyFrameDlg", "Video Browse", nullptr));
        label_2->setText(QCoreApplication::translate("VideoKeyFrameDlg", "Time", nullptr));
        pushButtonOk->setText(QCoreApplication::translate("VideoKeyFrameDlg", "OK", nullptr));
        pushButtonCancel->setText(QCoreApplication::translate("VideoKeyFrameDlg", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VideoKeyFrameDlg: public Ui_VideoKeyFrameDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOKEYFRAMEDLG_H
