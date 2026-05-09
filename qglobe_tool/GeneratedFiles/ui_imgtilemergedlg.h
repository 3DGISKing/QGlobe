/********************************************************************************
** Form generated from reading UI file 'imgtilemergedlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMGTILEMERGEDLG_H
#define UI_IMGTILEMERGEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ImgTileMergeDlg
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QLineEdit *lineEditInput;
    QPushButton *pushButtonInput;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEditSave;
    QPushButton *pushButtonSave;
    QProgressBar *progressBarJpgFile;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButtonMerge;

    void setupUi(QDialog *ImgTileMergeDlg)
    {
        if (ImgTileMergeDlg->objectName().isEmpty())
            ImgTileMergeDlg->setObjectName(QString::fromUtf8("ImgTileMergeDlg"));
        ImgTileMergeDlg->resize(524, 138);
        verticalLayout = new QVBoxLayout(ImgTileMergeDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(ImgTileMergeDlg);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        lineEditInput = new QLineEdit(ImgTileMergeDlg);
        lineEditInput->setObjectName(QString::fromUtf8("lineEditInput"));

        horizontalLayout->addWidget(lineEditInput);

        pushButtonInput = new QPushButton(ImgTileMergeDlg);
        pushButtonInput->setObjectName(QString::fromUtf8("pushButtonInput"));

        horizontalLayout->addWidget(pushButtonInput);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(ImgTileMergeDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        lineEditSave = new QLineEdit(ImgTileMergeDlg);
        lineEditSave->setObjectName(QString::fromUtf8("lineEditSave"));

        horizontalLayout_2->addWidget(lineEditSave);

        pushButtonSave = new QPushButton(ImgTileMergeDlg);
        pushButtonSave->setObjectName(QString::fromUtf8("pushButtonSave"));

        horizontalLayout_2->addWidget(pushButtonSave);


        verticalLayout->addLayout(horizontalLayout_2);

        progressBarJpgFile = new QProgressBar(ImgTileMergeDlg);
        progressBarJpgFile->setObjectName(QString::fromUtf8("progressBarJpgFile"));
        progressBarJpgFile->setValue(24);

        verticalLayout->addWidget(progressBarJpgFile);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        pushButtonMerge = new QPushButton(ImgTileMergeDlg);
        pushButtonMerge->setObjectName(QString::fromUtf8("pushButtonMerge"));

        horizontalLayout_3->addWidget(pushButtonMerge);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(ImgTileMergeDlg);

        QMetaObject::connectSlotsByName(ImgTileMergeDlg);
    } // setupUi

    void retranslateUi(QDialog *ImgTileMergeDlg)
    {
        ImgTileMergeDlg->setWindowTitle(QCoreApplication::translate("ImgTileMergeDlg", "ImgTileMergeDlg", nullptr));
        label->setText(QCoreApplication::translate("ImgTileMergeDlg", "Input Folder", nullptr));
        pushButtonInput->setText(QCoreApplication::translate("ImgTileMergeDlg", "Browser...", nullptr));
        label_2->setText(QCoreApplication::translate("ImgTileMergeDlg", "Save Folder", nullptr));
        pushButtonSave->setText(QCoreApplication::translate("ImgTileMergeDlg", "Browser...", nullptr));
        pushButtonMerge->setText(QCoreApplication::translate("ImgTileMergeDlg", "Merge", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImgTileMergeDlg: public Ui_ImgTileMergeDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMGTILEMERGEDLG_H
