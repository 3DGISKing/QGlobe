/********************************************************************************
** Form generated from reading UI file 'latlongtohopeimage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LATLONGTOHOPEIMAGE_H
#define UI_LATLONGTOHOPEIMAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_LatLongToHopeImageClass
{
public:
    QGroupBox *actionOptiongroup;
    QGroupBox *groupBox;
    QLabel *label_6;
    QLineEdit *actionBottomEdit;
    QLabel *label_5;
    QLabel *label_4;
    QLineEdit *actionTopEdit;
    QLabel *label_3;
    QLineEdit *actionLeftEdit;
    QLineEdit *actionRightEdit;
    QGroupBox *groupBox_2;
    QLineEdit *actionDestBottomEdit;
    QLabel *label_11;
    QLabel *label_13;
    QLabel *label_14;
    QLineEdit *actionDestTopEdit;
    QLabel *label_15;
    QLineEdit *actionDestLeftEdit;
    QLineEdit *actionDestRightEdit;
    QGroupBox *groupBox_3;
    QLabel *label_8;
    QLineEdit *actionImgQualityEdit;
    QLabel *label_9;
    QLabel *label_7;
    QLineEdit *actionStartLevelEdit;
    QLabel *label_10;
    QLineEdit *actionEndLevelEdit;
    QLabel *label_16;
    QLineEdit *actionIgnoreColorEdit;
    QCheckBox *jgwChkBox;
    QLabel *label_2;
    QLineEdit *actionDestinateDirEdit;
    QPushButton *actionSourceFileBtn;
    QPushButton *actionDestinateDirBtn;
    QLabel *label;
    QLineEdit *actionSourceFileEdit;
    QLabel *label_12;
    QProgressBar *actionProgressBar;
    QLabel *actionlblfilename;
    QPushButton *convertBtn;
    QCheckBox *actionDirectorySelect;
    QProgressBar *actionTotalProgressBar;
    QLabel *actionlbltotalfilename;
    QLabel *label_17;
    QLineEdit *tmpDirEdit;
    QPushButton *tmpDirBtn;
    QPushButton *marginProcessBtn;

    void setupUi(QDialog *LatLongToHopeImageClass)
    {
        if (LatLongToHopeImageClass->objectName().isEmpty())
            LatLongToHopeImageClass->setObjectName(QString::fromUtf8("LatLongToHopeImageClass"));
        LatLongToHopeImageClass->resize(573, 389);
        actionOptiongroup = new QGroupBox(LatLongToHopeImageClass);
        actionOptiongroup->setObjectName(QString::fromUtf8("actionOptiongroup"));
        actionOptiongroup->setGeometry(QRect(10, 117, 551, 181));
        groupBox = new QGroupBox(actionOptiongroup);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 351, 80));
        groupBox->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 50, 31, 16));
        actionBottomEdit = new QLineEdit(groupBox);
        actionBottomEdit->setObjectName(QString::fromUtf8("actionBottomEdit"));
        actionBottomEdit->setGeometry(QRect(220, 50, 121, 20));
        actionBottomEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        actionBottomEdit->setReadOnly(true);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 20, 31, 21));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(180, 50, 41, 20));
        actionTopEdit = new QLineEdit(groupBox);
        actionTopEdit->setObjectName(QString::fromUtf8("actionTopEdit"));
        actionTopEdit->setGeometry(QRect(220, 20, 121, 20));
        actionTopEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        actionTopEdit->setReadOnly(true);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(190, 20, 21, 16));
        actionLeftEdit = new QLineEdit(groupBox);
        actionLeftEdit->setObjectName(QString::fromUtf8("actionLeftEdit"));
        actionLeftEdit->setGeometry(QRect(40, 20, 131, 20));
        actionLeftEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        actionLeftEdit->setReadOnly(true);
        actionRightEdit = new QLineEdit(groupBox);
        actionRightEdit->setObjectName(QString::fromUtf8("actionRightEdit"));
        actionRightEdit->setGeometry(QRect(40, 50, 131, 20));
        actionRightEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        actionRightEdit->setReadOnly(true);
        groupBox_2 = new QGroupBox(actionOptiongroup);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 90, 351, 80));
        groupBox_2->setAlignment(Qt::AlignCenter);
        actionDestBottomEdit = new QLineEdit(groupBox_2);
        actionDestBottomEdit->setObjectName(QString::fromUtf8("actionDestBottomEdit"));
        actionDestBottomEdit->setGeometry(QRect(220, 50, 121, 20));
        actionDestBottomEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 50, 31, 16));
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 20, 21, 16));
        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(180, 50, 31, 21));
        actionDestTopEdit = new QLineEdit(groupBox_2);
        actionDestTopEdit->setObjectName(QString::fromUtf8("actionDestTopEdit"));
        actionDestTopEdit->setGeometry(QRect(220, 20, 121, 20));
        actionDestTopEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(190, 20, 31, 16));
        actionDestLeftEdit = new QLineEdit(groupBox_2);
        actionDestLeftEdit->setObjectName(QString::fromUtf8("actionDestLeftEdit"));
        actionDestLeftEdit->setGeometry(QRect(40, 20, 131, 20));
        actionDestLeftEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        actionDestRightEdit = new QLineEdit(groupBox_2);
        actionDestRightEdit->setObjectName(QString::fromUtf8("actionDestRightEdit"));
        actionDestRightEdit->setGeometry(QRect(40, 50, 131, 20));
        actionDestRightEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox_3 = new QGroupBox(actionOptiongroup);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(370, 10, 171, 161));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 23, 81, 16));
        actionImgQualityEdit = new QLineEdit(groupBox_3);
        actionImgQualityEdit->setObjectName(QString::fromUtf8("actionImgQualityEdit"));
        actionImgQualityEdit->setGeometry(QRect(100, 23, 31, 20));
        actionImgQualityEdit->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(140, 23, 21, 16));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 76, 71, 20));
        actionStartLevelEdit = new QLineEdit(groupBox_3);
        actionStartLevelEdit->setObjectName(QString::fromUtf8("actionStartLevelEdit"));
        actionStartLevelEdit->setGeometry(QRect(99, 76, 41, 20));
        actionStartLevelEdit->setAlignment(Qt::AlignCenter);
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(20, 106, 61, 20));
        actionEndLevelEdit = new QLineEdit(groupBox_3);
        actionEndLevelEdit->setObjectName(QString::fromUtf8("actionEndLevelEdit"));
        actionEndLevelEdit->setGeometry(QRect(99, 106, 41, 20));
        actionEndLevelEdit->setAlignment(Qt::AlignCenter);
        label_16 = new QLabel(groupBox_3);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(10, 47, 81, 20));
        actionIgnoreColorEdit = new QLineEdit(groupBox_3);
        actionIgnoreColorEdit->setObjectName(QString::fromUtf8("actionIgnoreColorEdit"));
        actionIgnoreColorEdit->setGeometry(QRect(100, 47, 61, 20));
        actionIgnoreColorEdit->setAlignment(Qt::AlignCenter);
        jgwChkBox = new QCheckBox(groupBox_3);
        jgwChkBox->setObjectName(QString::fromUtf8("jgwChkBox"));
        jgwChkBox->setGeometry(QRect(20, 130, 141, 19));
        label_2 = new QLabel(LatLongToHopeImageClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 60, 111, 20));
        actionDestinateDirEdit = new QLineEdit(LatLongToHopeImageClass);
        actionDestinateDirEdit->setObjectName(QString::fromUtf8("actionDestinateDirEdit"));
        actionDestinateDirEdit->setGeometry(QRect(130, 60, 301, 21));
        actionSourceFileBtn = new QPushButton(LatLongToHopeImageClass);
        actionSourceFileBtn->setObjectName(QString::fromUtf8("actionSourceFileBtn"));
        actionSourceFileBtn->setGeometry(QRect(430, 30, 75, 21));
        actionDestinateDirBtn = new QPushButton(LatLongToHopeImageClass);
        actionDestinateDirBtn->setObjectName(QString::fromUtf8("actionDestinateDirBtn"));
        actionDestinateDirBtn->setGeometry(QRect(430, 60, 75, 21));
        label = new QLabel(LatLongToHopeImageClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 30, 101, 16));
        actionSourceFileEdit = new QLineEdit(LatLongToHopeImageClass);
        actionSourceFileEdit->setObjectName(QString::fromUtf8("actionSourceFileEdit"));
        actionSourceFileEdit->setGeometry(QRect(130, 30, 301, 21));
        label_12 = new QLabel(LatLongToHopeImageClass);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(150, 0, 241, 16));
        QFont font;
        font.setPointSize(10);
        label_12->setFont(font);
        label_12->setAlignment(Qt::AlignCenter);
        actionProgressBar = new QProgressBar(LatLongToHopeImageClass);
        actionProgressBar->setObjectName(QString::fromUtf8("actionProgressBar"));
        actionProgressBar->setGeometry(QRect(30, 317, 331, 23));
        actionProgressBar->setAutoFillBackground(false);
        actionProgressBar->setValue(0);
        actionlblfilename = new QLabel(LatLongToHopeImageClass);
        actionlblfilename->setObjectName(QString::fromUtf8("actionlblfilename"));
        actionlblfilename->setGeometry(QRect(30, 297, 421, 16));
        convertBtn = new QPushButton(LatLongToHopeImageClass);
        convertBtn->setObjectName(QString::fromUtf8("convertBtn"));
        convertBtn->setGeometry(QRect(390, 310, 91, 24));
        actionDirectorySelect = new QCheckBox(LatLongToHopeImageClass);
        actionDirectorySelect->setObjectName(QString::fromUtf8("actionDirectorySelect"));
        actionDirectorySelect->setGeometry(QRect(20, 10, 111, 19));
        actionDirectorySelect->setChecked(true);
        actionTotalProgressBar = new QProgressBar(LatLongToHopeImageClass);
        actionTotalProgressBar->setObjectName(QString::fromUtf8("actionTotalProgressBar"));
        actionTotalProgressBar->setGeometry(QRect(30, 344, 331, 23));
        actionTotalProgressBar->setAutoFillBackground(false);
        actionTotalProgressBar->setValue(0);
        actionlbltotalfilename = new QLabel(LatLongToHopeImageClass);
        actionlbltotalfilename->setObjectName(QString::fromUtf8("actionlbltotalfilename"));
        actionlbltotalfilename->setGeometry(QRect(30, 310, 351, 16));
        label_17 = new QLabel(LatLongToHopeImageClass);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(20, 90, 111, 20));
        tmpDirEdit = new QLineEdit(LatLongToHopeImageClass);
        tmpDirEdit->setObjectName(QString::fromUtf8("tmpDirEdit"));
        tmpDirEdit->setGeometry(QRect(130, 90, 301, 21));
        tmpDirBtn = new QPushButton(LatLongToHopeImageClass);
        tmpDirBtn->setObjectName(QString::fromUtf8("tmpDirBtn"));
        tmpDirBtn->setGeometry(QRect(430, 90, 75, 21));
        marginProcessBtn = new QPushButton(LatLongToHopeImageClass);
        marginProcessBtn->setObjectName(QString::fromUtf8("marginProcessBtn"));
        marginProcessBtn->setGeometry(QRect(390, 340, 91, 24));
        QWidget::setTabOrder(actionSourceFileEdit, actionDestinateDirEdit);
        QWidget::setTabOrder(actionDestinateDirEdit, actionLeftEdit);
        QWidget::setTabOrder(actionLeftEdit, actionTopEdit);
        QWidget::setTabOrder(actionTopEdit, actionRightEdit);
        QWidget::setTabOrder(actionRightEdit, actionBottomEdit);
        QWidget::setTabOrder(actionBottomEdit, actionStartLevelEdit);
        QWidget::setTabOrder(actionStartLevelEdit, actionSourceFileBtn);
        QWidget::setTabOrder(actionSourceFileBtn, actionDestinateDirBtn);
        QWidget::setTabOrder(actionDestinateDirBtn, convertBtn);

        retranslateUi(LatLongToHopeImageClass);
        QObject::connect(actionImgQualityEdit, SIGNAL(textChanged(QString)), LatLongToHopeImageClass, SLOT(getImagQuality()));

        QMetaObject::connectSlotsByName(LatLongToHopeImageClass);
    } // setupUi

    void retranslateUi(QDialog *LatLongToHopeImageClass)
    {
        LatLongToHopeImageClass->setWindowTitle(QCoreApplication::translate("LatLongToHopeImageClass", "LatLongToHopeImage", nullptr));
        actionOptiongroup->setTitle(QCoreApplication::translate("LatLongToHopeImageClass", "Options", nullptr));
        groupBox->setTitle(QCoreApplication::translate("LatLongToHopeImageClass", "Source Range", nullptr));
        label_6->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Right", nullptr));
        actionBottomEdit->setText(QCoreApplication::translate("LatLongToHopeImageClass", "-90", nullptr));
        label_5->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Left", nullptr));
        label_4->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Bottom", nullptr));
        actionTopEdit->setText(QCoreApplication::translate("LatLongToHopeImageClass", "90", nullptr));
        label_3->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Top", nullptr));
        actionLeftEdit->setText(QCoreApplication::translate("LatLongToHopeImageClass", "-180", nullptr));
        actionRightEdit->setText(QCoreApplication::translate("LatLongToHopeImageClass", "180", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("LatLongToHopeImageClass", "Destinate Range", nullptr));
        actionDestBottomEdit->setText(QCoreApplication::translate("LatLongToHopeImageClass", "-90", nullptr));
        label_11->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Right", nullptr));
        label_13->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Left", nullptr));
        label_14->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Bottom", nullptr));
        actionDestTopEdit->setText(QCoreApplication::translate("LatLongToHopeImageClass", "90", nullptr));
        label_15->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Top", nullptr));
        actionDestLeftEdit->setText(QCoreApplication::translate("LatLongToHopeImageClass", "-180", nullptr));
        actionDestRightEdit->setText(QCoreApplication::translate("LatLongToHopeImageClass", "180", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("LatLongToHopeImageClass", "GroupBox", nullptr));
        label_8->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Image Quality", nullptr));
        actionImgQualityEdit->setText(QCoreApplication::translate("LatLongToHopeImageClass", "100", nullptr));
        label_9->setText(QCoreApplication::translate("LatLongToHopeImageClass", "%", nullptr));
        label_7->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Start  Level", nullptr));
        actionStartLevelEdit->setText(QCoreApplication::translate("LatLongToHopeImageClass", "0", nullptr));
        label_10->setText(QCoreApplication::translate("LatLongToHopeImageClass", "End Level", nullptr));
        actionEndLevelEdit->setText(QCoreApplication::translate("LatLongToHopeImageClass", "18", nullptr));
        label_16->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Ignore Color :", nullptr));
        actionIgnoreColorEdit->setText(QCoreApplication::translate("LatLongToHopeImageClass", "0x000000", nullptr));
        jgwChkBox->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Write JGW File?", nullptr));
        label_2->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Destinate Directory", nullptr));
        actionDestinateDirEdit->setText(QString());
        actionSourceFileBtn->setText(QCoreApplication::translate("LatLongToHopeImageClass", "...", nullptr));
        actionDestinateDirBtn->setText(QCoreApplication::translate("LatLongToHopeImageClass", "...", nullptr));
        label->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Source File", nullptr));
        actionSourceFileEdit->setText(QString());
        label_12->setText(QCoreApplication::translate("LatLongToHopeImageClass", "GeoImage Tiling", nullptr));
        actionlblfilename->setText(QString());
        convertBtn->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Convert", nullptr));
        actionDirectorySelect->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Directory Select", nullptr));
        actionlbltotalfilename->setText(QString());
        label_17->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Temp Directory(for Margin process)", nullptr));
        tmpDirEdit->setText(QString());
        tmpDirBtn->setText(QCoreApplication::translate("LatLongToHopeImageClass", "...", nullptr));
        marginProcessBtn->setText(QCoreApplication::translate("LatLongToHopeImageClass", "Margin Process", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LatLongToHopeImageClass: public Ui_LatLongToHopeImageClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LATLONGTOHOPEIMAGE_H
