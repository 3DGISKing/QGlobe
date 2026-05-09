/********************************************************************************
** Form generated from reading UI file 'smpbuildingprodlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMPBUILDINGPRODLG_H
#define UI_SMPBUILDINGPRODLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SmpBuildingProDlg
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEditInputPath;
    QPushButton *pushButtonBrowserInput;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEditSavePath;
    QPushButton *pushButtonBrowserSave;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *lineEditSqliteDbPath;
    QPushButton *pushButtonBrowserSqlite;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *checkBoxUpdateBuildingTb;
    QCheckBox *checkBoxCreateSql;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *lineEditLocID;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEditTolerancePixel;
    QLabel *labelInfo;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonGenerate;

    void setupUi(QDialog *SmpBuildingProDlg)
    {
        if (SmpBuildingProDlg->objectName().isEmpty())
            SmpBuildingProDlg->setObjectName(QString::fromUtf8("SmpBuildingProDlg"));
        SmpBuildingProDlg->resize(393, 301);
        verticalLayout = new QVBoxLayout(SmpBuildingProDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(SmpBuildingProDlg);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEditInputPath = new QLineEdit(SmpBuildingProDlg);
        lineEditInputPath->setObjectName(QString::fromUtf8("lineEditInputPath"));
        lineEditInputPath->setReadOnly(true);

        horizontalLayout->addWidget(lineEditInputPath);

        pushButtonBrowserInput = new QPushButton(SmpBuildingProDlg);
        pushButtonBrowserInput->setObjectName(QString::fromUtf8("pushButtonBrowserInput"));

        horizontalLayout->addWidget(pushButtonBrowserInput);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(SmpBuildingProDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEditSavePath = new QLineEdit(SmpBuildingProDlg);
        lineEditSavePath->setObjectName(QString::fromUtf8("lineEditSavePath"));
        lineEditSavePath->setReadOnly(true);

        horizontalLayout_2->addWidget(lineEditSavePath);

        pushButtonBrowserSave = new QPushButton(SmpBuildingProDlg);
        pushButtonBrowserSave->setObjectName(QString::fromUtf8("pushButtonBrowserSave"));

        horizontalLayout_2->addWidget(pushButtonBrowserSave);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(SmpBuildingProDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        lineEditSqliteDbPath = new QLineEdit(SmpBuildingProDlg);
        lineEditSqliteDbPath->setObjectName(QString::fromUtf8("lineEditSqliteDbPath"));
        lineEditSqliteDbPath->setReadOnly(true);

        horizontalLayout_3->addWidget(lineEditSqliteDbPath);

        pushButtonBrowserSqlite = new QPushButton(SmpBuildingProDlg);
        pushButtonBrowserSqlite->setObjectName(QString::fromUtf8("pushButtonBrowserSqlite"));

        horizontalLayout_3->addWidget(pushButtonBrowserSqlite);


        verticalLayout->addLayout(horizontalLayout_3);

        groupBox = new QGroupBox(SmpBuildingProDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        checkBoxUpdateBuildingTb = new QCheckBox(groupBox);
        checkBoxUpdateBuildingTb->setObjectName(QString::fromUtf8("checkBoxUpdateBuildingTb"));

        verticalLayout_2->addWidget(checkBoxUpdateBuildingTb);

        checkBoxCreateSql = new QCheckBox(groupBox);
        checkBoxCreateSql->setObjectName(QString::fromUtf8("checkBoxCreateSql"));

        verticalLayout_2->addWidget(checkBoxCreateSql);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_6->addWidget(label_6);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        lineEditLocID = new QLineEdit(groupBox);
        lineEditLocID->setObjectName(QString::fromUtf8("lineEditLocID"));

        horizontalLayout_6->addWidget(lineEditLocID);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_6->addWidget(label_5);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);

        lineEditTolerancePixel = new QLineEdit(groupBox);
        lineEditTolerancePixel->setObjectName(QString::fromUtf8("lineEditTolerancePixel"));

        horizontalLayout_6->addWidget(lineEditTolerancePixel);


        verticalLayout_2->addLayout(horizontalLayout_6);


        verticalLayout->addWidget(groupBox);

        labelInfo = new QLabel(SmpBuildingProDlg);
        labelInfo->setObjectName(QString::fromUtf8("labelInfo"));

        verticalLayout->addWidget(labelInfo);

        progressBar = new QProgressBar(SmpBuildingProDlg);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        pushButtonCancel = new QPushButton(SmpBuildingProDlg);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));

        horizontalLayout_4->addWidget(pushButtonCancel);

        pushButtonGenerate = new QPushButton(SmpBuildingProDlg);
        pushButtonGenerate->setObjectName(QString::fromUtf8("pushButtonGenerate"));

        horizontalLayout_4->addWidget(pushButtonGenerate);


        verticalLayout->addLayout(horizontalLayout_4);


        retranslateUi(SmpBuildingProDlg);

        QMetaObject::connectSlotsByName(SmpBuildingProDlg);
    } // setupUi

    void retranslateUi(QDialog *SmpBuildingProDlg)
    {
        SmpBuildingProDlg->setWindowTitle(QCoreApplication::translate("SmpBuildingProDlg", "SmpBuildingProDlg", nullptr));
        label->setText(QCoreApplication::translate("SmpBuildingProDlg", "Esri Shape File", nullptr));
        pushButtonBrowserInput->setText(QCoreApplication::translate("SmpBuildingProDlg", "Browser...", nullptr));
        label_2->setText(QCoreApplication::translate("SmpBuildingProDlg", "Save Path      ", nullptr));
        pushButtonBrowserSave->setText(QCoreApplication::translate("SmpBuildingProDlg", "Browser...", nullptr));
        label_3->setText(QCoreApplication::translate("SmpBuildingProDlg", "SQLite DB Path", nullptr));
        pushButtonBrowserSqlite->setText(QCoreApplication::translate("SmpBuildingProDlg", "Browser...", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SmpBuildingProDlg", "Parameters", nullptr));
        checkBoxUpdateBuildingTb->setText(QCoreApplication::translate("SmpBuildingProDlg", "Update Building Table", nullptr));
        checkBoxCreateSql->setText(QCoreApplication::translate("SmpBuildingProDlg", "Create SQL statement(Save Path+\"\"+\"sql.txt\")", nullptr));
        label_6->setText(QCoreApplication::translate("SmpBuildingProDlg", "LocID", nullptr));
        label_5->setText(QCoreApplication::translate("SmpBuildingProDlg", "Pixel Tolerance", nullptr));
        labelInfo->setText(QCoreApplication::translate("SmpBuildingProDlg", "TextLabel", nullptr));
        pushButtonCancel->setText(QCoreApplication::translate("SmpBuildingProDlg", "Cancel", nullptr));
        pushButtonGenerate->setText(QCoreApplication::translate("SmpBuildingProDlg", "Generate Simple Building", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SmpBuildingProDlg: public Ui_SmpBuildingProDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMPBUILDINGPRODLG_H
