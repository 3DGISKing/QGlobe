/********************************************************************************
** Form generated from reading UI file 'qglobedbextractdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GDEMDBEXTRACTDLG_H
#define UI_GDEMDBEXTRACTDLG_H

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
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GDemDbExtractDlg
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEditInputPath;
    QPushButton *pushButtonBrowserInput;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEditSavePath;
    QPushButton *pushButtonBrowserSave;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_6;
    QRadioButton *radioButtonImage;
    QCheckBox *actionJGWmaker;
    QRadioButton *radioButtonDem;
    QRadioButton *radioButtonShape;
    QGroupBox *groupBoxBounding;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton;
    QLineEdit *lineEditLeft;
    QPushButton *pushButton_3;
    QLineEdit *lineEditTop;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_2;
    QLineEdit *lineEditRight;
    QPushButton *pushButton_4;
    QLineEdit *lineEditBottom;
    QLabel *labelTileFileName;
    QProgressBar *progressBarTileCount;
    QLabel *labelGDemFileName;
    QProgressBar *progressBarGDemCount;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonExtract;

    void setupUi(QDialog *GDemDbExtractDlg)
    {
        if (GDemDbExtractDlg->objectName().isEmpty())
            GDemDbExtractDlg->setObjectName(QString::fromUtf8("GDemDbExtractDlg"));
        GDemDbExtractDlg->resize(514, 413);
        verticalLayout_4 = new QVBoxLayout(GDemDbExtractDlg);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        label = new QLabel(GDemDbExtractDlg);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        lineEditInputPath = new QLineEdit(GDemDbExtractDlg);
        lineEditInputPath->setObjectName(QString::fromUtf8("lineEditInputPath"));
        lineEditInputPath->setReadOnly(false);

        horizontalLayout->addWidget(lineEditInputPath);

        pushButtonBrowserInput = new QPushButton(GDemDbExtractDlg);
        pushButtonBrowserInput->setObjectName(QString::fromUtf8("pushButtonBrowserInput"));

        horizontalLayout->addWidget(pushButtonBrowserInput);


        verticalLayout_4->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(GDemDbExtractDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEditSavePath = new QLineEdit(GDemDbExtractDlg);
        lineEditSavePath->setObjectName(QString::fromUtf8("lineEditSavePath"));

        horizontalLayout_2->addWidget(lineEditSavePath);

        pushButtonBrowserSave = new QPushButton(GDemDbExtractDlg);
        pushButtonBrowserSave->setObjectName(QString::fromUtf8("pushButtonBrowserSave"));

        horizontalLayout_2->addWidget(pushButtonBrowserSave);


        verticalLayout_4->addLayout(horizontalLayout_2);

        groupBox_2 = new QGroupBox(GDemDbExtractDlg);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        radioButtonImage = new QRadioButton(groupBox_2);
        radioButtonImage->setObjectName(QString::fromUtf8("radioButtonImage"));
        radioButtonImage->setChecked(true);

        horizontalLayout_6->addWidget(radioButtonImage);

        actionJGWmaker = new QCheckBox(groupBox_2);
        actionJGWmaker->setObjectName(QString::fromUtf8("actionJGWmaker"));
        actionJGWmaker->setChecked(false);

        horizontalLayout_6->addWidget(actionJGWmaker);


        verticalLayout_2->addLayout(horizontalLayout_6);

        radioButtonDem = new QRadioButton(groupBox_2);
        radioButtonDem->setObjectName(QString::fromUtf8("radioButtonDem"));

        verticalLayout_2->addWidget(radioButtonDem);

        radioButtonShape = new QRadioButton(groupBox_2);
        radioButtonShape->setObjectName(QString::fromUtf8("radioButtonShape"));

        verticalLayout_2->addWidget(radioButtonShape);


        verticalLayout_4->addWidget(groupBox_2);

        groupBoxBounding = new QGroupBox(GDemDbExtractDlg);
        groupBoxBounding->setObjectName(QString::fromUtf8("groupBoxBounding"));
        groupBoxBounding->setLayoutDirection(Qt::LeftToRight);
        groupBoxBounding->setAlignment(Qt::AlignCenter);
        groupBoxBounding->setCheckable(true);
        groupBoxBounding->setChecked(false);
        verticalLayout = new QVBoxLayout(groupBoxBounding);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushButton = new QPushButton(groupBoxBounding);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_3->addWidget(pushButton);

        lineEditLeft = new QLineEdit(groupBoxBounding);
        lineEditLeft->setObjectName(QString::fromUtf8("lineEditLeft"));

        horizontalLayout_3->addWidget(lineEditLeft);

        pushButton_3 = new QPushButton(groupBoxBounding);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setAutoFillBackground(false);

        horizontalLayout_3->addWidget(pushButton_3);

        lineEditTop = new QLineEdit(groupBoxBounding);
        lineEditTop->setObjectName(QString::fromUtf8("lineEditTop"));

        horizontalLayout_3->addWidget(lineEditTop);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        pushButton_2 = new QPushButton(groupBoxBounding);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout_4->addWidget(pushButton_2);

        lineEditRight = new QLineEdit(groupBoxBounding);
        lineEditRight->setObjectName(QString::fromUtf8("lineEditRight"));

        horizontalLayout_4->addWidget(lineEditRight);

        pushButton_4 = new QPushButton(groupBoxBounding);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        horizontalLayout_4->addWidget(pushButton_4);

        lineEditBottom = new QLineEdit(groupBoxBounding);
        lineEditBottom->setObjectName(QString::fromUtf8("lineEditBottom"));

        horizontalLayout_4->addWidget(lineEditBottom);


        verticalLayout->addLayout(horizontalLayout_4);


        verticalLayout_4->addWidget(groupBoxBounding);

        labelTileFileName = new QLabel(GDemDbExtractDlg);
        labelTileFileName->setObjectName(QString::fromUtf8("labelTileFileName"));

        verticalLayout_4->addWidget(labelTileFileName);

        progressBarTileCount = new QProgressBar(GDemDbExtractDlg);
        progressBarTileCount->setObjectName(QString::fromUtf8("progressBarTileCount"));
        progressBarTileCount->setValue(0);

        verticalLayout_4->addWidget(progressBarTileCount);

        labelGDemFileName = new QLabel(GDemDbExtractDlg);
        labelGDemFileName->setObjectName(QString::fromUtf8("labelGDemFileName"));

        verticalLayout_4->addWidget(labelGDemFileName);

        progressBarGDemCount = new QProgressBar(GDemDbExtractDlg);
        progressBarGDemCount->setObjectName(QString::fromUtf8("progressBarGDemCount"));
        progressBarGDemCount->setValue(0);

        verticalLayout_4->addWidget(progressBarGDemCount);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        pushButtonCancel = new QPushButton(GDemDbExtractDlg);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));

        horizontalLayout_5->addWidget(pushButtonCancel);

        pushButtonExtract = new QPushButton(GDemDbExtractDlg);
        pushButtonExtract->setObjectName(QString::fromUtf8("pushButtonExtract"));

        horizontalLayout_5->addWidget(pushButtonExtract);


        verticalLayout_4->addLayout(horizontalLayout_5);


        retranslateUi(GDemDbExtractDlg);

        QMetaObject::connectSlotsByName(GDemDbExtractDlg);
    } // setupUi

    void retranslateUi(QDialog *GDemDbExtractDlg)
    {
        GDemDbExtractDlg->setWindowTitle(QCoreApplication::translate("GDemDbExtractDlg", "GDemDbExtractDlg", nullptr));
        label->setText(QCoreApplication::translate("GDemDbExtractDlg", "Db Path    ", nullptr));
        pushButtonBrowserInput->setText(QCoreApplication::translate("GDemDbExtractDlg", "Browser...", nullptr));
        label_2->setText(QCoreApplication::translate("GDemDbExtractDlg", "Save Path", nullptr));
        pushButtonBrowserSave->setText(QCoreApplication::translate("GDemDbExtractDlg", "Browser...", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("GDemDbExtractDlg", "Data Type", nullptr));
        radioButtonImage->setText(QCoreApplication::translate("GDemDbExtractDlg", " Jpeg Image", nullptr));
        actionJGWmaker->setText(QCoreApplication::translate("GDemDbExtractDlg", "Creating jgw", nullptr));
        radioButtonDem->setText(QCoreApplication::translate("GDemDbExtractDlg", "Dem", nullptr));
        radioButtonShape->setText(QCoreApplication::translate("GDemDbExtractDlg", "Shape", nullptr));
        groupBoxBounding->setTitle(QCoreApplication::translate("GDemDbExtractDlg", "Bounding Rectangle", nullptr));
        pushButton->setText(QCoreApplication::translate("GDemDbExtractDlg", "Left", nullptr));
        lineEditLeft->setText(QCoreApplication::translate("GDemDbExtractDlg", "-180", nullptr));
        pushButton_3->setText(QCoreApplication::translate("GDemDbExtractDlg", "Top", nullptr));
        lineEditTop->setText(QCoreApplication::translate("GDemDbExtractDlg", "90", nullptr));
        pushButton_2->setText(QCoreApplication::translate("GDemDbExtractDlg", "Right", nullptr));
        lineEditRight->setText(QCoreApplication::translate("GDemDbExtractDlg", "180", nullptr));
        pushButton_4->setText(QCoreApplication::translate("GDemDbExtractDlg", "Bottom", nullptr));
        lineEditBottom->setText(QCoreApplication::translate("GDemDbExtractDlg", "-90", nullptr));
        labelTileFileName->setText(QCoreApplication::translate("GDemDbExtractDlg", "None", nullptr));
        labelGDemFileName->setText(QString());
        pushButtonCancel->setText(QCoreApplication::translate("GDemDbExtractDlg", "Cancel", nullptr));
        pushButtonExtract->setText(QCoreApplication::translate("GDemDbExtractDlg", "Extract", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GDemDbExtractDlg: public Ui_GDemDbExtractDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GDEMDBEXTRACTDLG_H
