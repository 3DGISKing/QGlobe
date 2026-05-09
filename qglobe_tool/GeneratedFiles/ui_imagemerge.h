/********************************************************************************
** Form generated from reading UI file 'imagemerge.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEMERGE_H
#define UI_IMAGEMERGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_ImageMergeClass
{
public:
    QLabel *label_9;
    QLabel *label_3;
    QPushButton *actionSourceDirBtn;
    QLineEdit *actionDestDirEdit;
    QPushButton *actionDestDirBtn;
    QLineEdit *actionSourceDirEdit;
    QProgressBar *actionDestProgress;
    QLabel *actionlblSourcefile;
    QPushButton *actionRunBtn;
    QLabel *actionlbldestfile;
    QGroupBox *actionOptiongroup;
    QGroupBox *groupBox_2;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *actionLeftEdit;
    QLineEdit *actionRightEdit;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *actionBottomEdit;
    QLineEdit *actionTopEdit;
    QGroupBox *groupBox_3;
    QLabel *label_14;
    QLabel *label_15;
    QLineEdit *actiontargetLeftEdit;
    QLineEdit *actiontargetRightEdit;
    QLabel *label_16;
    QLabel *label_17;
    QLineEdit *actiontargetBottomEdit;
    QLineEdit *actiontargetTopEdit;
    QGroupBox *groupBox;
    QLabel *label_12;
    QComboBox *actionSizeCombo;
    QLabel *label_10;
    QLineEdit *actionImgQualityEdit;
    QLabel *label_11;
    QCheckBox *actionJGWCheckBox;
    QCheckBox *actiondatabaseUpgradeCheckBox;
    QLineEdit *actionToLevelEdit;
    QLabel *label_13;
    QLineEdit *actionFromLevelEdit;
    QLabel *label_8;
    QLabel *label_2;
    QCheckBox *actionUseLowerImageCheckBox;
    QGroupBox *groupBox_4;
    QRadioButton *actionOperationSumBtn;
    QRadioButton *actionOperationSourceBtn;
    QRadioButton *actionOperationTargetBtn;
    QRadioButton *actionOperationCrossBtn;
    QCheckBox *actionOnlyJGWCheckBox;
    QLabel *actionMultiText;
    QProgressBar *actionSourceProgress;
    QLabel *label;
    QLabel *actionlbltmpfile;
    QCheckBox *actionBlockCheckBox;

    void setupUi(QDialog *ImageMergeClass)
    {
        if (ImageMergeClass->objectName().isEmpty())
            ImageMergeClass->setObjectName(QString::fromUtf8("ImageMergeClass"));
        ImageMergeClass->resize(609, 380);
        label_9 = new QLabel(ImageMergeClass);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(170, 40, 71, 20));
        label_9->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(ImageMergeClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(160, 10, 81, 20));
        label_3->setAlignment(Qt::AlignCenter);
        actionSourceDirBtn = new QPushButton(ImageMergeClass);
        actionSourceDirBtn->setObjectName(QString::fromUtf8("actionSourceDirBtn"));
        actionSourceDirBtn->setGeometry(QRect(540, 10, 41, 21));
        actionDestDirEdit = new QLineEdit(ImageMergeClass);
        actionDestDirEdit->setObjectName(QString::fromUtf8("actionDestDirEdit"));
        actionDestDirEdit->setGeometry(QRect(260, 40, 281, 21));
        actionDestDirBtn = new QPushButton(ImageMergeClass);
        actionDestDirBtn->setObjectName(QString::fromUtf8("actionDestDirBtn"));
        actionDestDirBtn->setGeometry(QRect(540, 40, 41, 21));
        actionSourceDirEdit = new QLineEdit(ImageMergeClass);
        actionSourceDirEdit->setObjectName(QString::fromUtf8("actionSourceDirEdit"));
        actionSourceDirEdit->setGeometry(QRect(260, 10, 281, 21));
        actionSourceDirEdit->setReadOnly(true);
        actionDestProgress = new QProgressBar(ImageMergeClass);
        actionDestProgress->setObjectName(QString::fromUtf8("actionDestProgress"));
        actionDestProgress->setGeometry(QRect(360, 300, 231, 23));
        actionDestProgress->setValue(0);
        actionlblSourcefile = new QLabel(ImageMergeClass);
        actionlblSourcefile->setObjectName(QString::fromUtf8("actionlblSourcefile"));
        actionlblSourcefile->setGeometry(QRect(20, 280, 411, 20));
        actionRunBtn = new QPushButton(ImageMergeClass);
        actionRunBtn->setObjectName(QString::fromUtf8("actionRunBtn"));
        actionRunBtn->setGeometry(QRect(410, 340, 75, 24));
        actionlbldestfile = new QLabel(ImageMergeClass);
        actionlbldestfile->setObjectName(QString::fromUtf8("actionlbldestfile"));
        actionlbldestfile->setGeometry(QRect(190, 260, 401, 20));
        actionlbldestfile->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        actionOptiongroup = new QGroupBox(ImageMergeClass);
        actionOptiongroup->setObjectName(QString::fromUtf8("actionOptiongroup"));
        actionOptiongroup->setGeometry(QRect(10, 60, 291, 191));
        groupBox_2 = new QGroupBox(actionOptiongroup);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 271, 81));
        groupBox_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 20, 41, 16));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 50, 41, 16));
        actionLeftEdit = new QLineEdit(groupBox_2);
        actionLeftEdit->setObjectName(QString::fromUtf8("actionLeftEdit"));
        actionLeftEdit->setGeometry(QRect(50, 20, 81, 20));
        actionLeftEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        actionRightEdit = new QLineEdit(groupBox_2);
        actionRightEdit->setObjectName(QString::fromUtf8("actionRightEdit"));
        actionRightEdit->setGeometry(QRect(50, 50, 81, 20));
        actionRightEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(140, 20, 31, 16));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(140, 50, 41, 16));
        actionBottomEdit = new QLineEdit(groupBox_2);
        actionBottomEdit->setObjectName(QString::fromUtf8("actionBottomEdit"));
        actionBottomEdit->setGeometry(QRect(180, 50, 81, 20));
        actionBottomEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        actionTopEdit = new QLineEdit(groupBox_2);
        actionTopEdit->setObjectName(QString::fromUtf8("actionTopEdit"));
        actionTopEdit->setGeometry(QRect(180, 20, 81, 20));
        actionTopEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox_3 = new QGroupBox(actionOptiongroup);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 100, 271, 81));
        groupBox_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 20, 41, 16));
        label_15 = new QLabel(groupBox_3);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 50, 41, 16));
        actiontargetLeftEdit = new QLineEdit(groupBox_3);
        actiontargetLeftEdit->setObjectName(QString::fromUtf8("actiontargetLeftEdit"));
        actiontargetLeftEdit->setGeometry(QRect(50, 20, 81, 20));
        actiontargetLeftEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        actiontargetRightEdit = new QLineEdit(groupBox_3);
        actiontargetRightEdit->setObjectName(QString::fromUtf8("actiontargetRightEdit"));
        actiontargetRightEdit->setGeometry(QRect(50, 50, 81, 20));
        actiontargetRightEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_16 = new QLabel(groupBox_3);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(140, 20, 31, 16));
        label_17 = new QLabel(groupBox_3);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(140, 50, 41, 16));
        actiontargetBottomEdit = new QLineEdit(groupBox_3);
        actiontargetBottomEdit->setObjectName(QString::fromUtf8("actiontargetBottomEdit"));
        actiontargetBottomEdit->setGeometry(QRect(180, 50, 81, 20));
        actiontargetBottomEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        actiontargetTopEdit = new QLineEdit(groupBox_3);
        actiontargetTopEdit->setObjectName(QString::fromUtf8("actiontargetTopEdit"));
        actiontargetTopEdit->setGeometry(QRect(180, 20, 81, 20));
        actiontargetTopEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox = new QGroupBox(ImageMergeClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(310, 60, 271, 191));
        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 20, 71, 16));
        actionSizeCombo = new QComboBox(groupBox);
        actionSizeCombo->addItem(QString());
        actionSizeCombo->addItem(QString());
        actionSizeCombo->addItem(QString());
        actionSizeCombo->addItem(QString());
        actionSizeCombo->addItem(QString());
        actionSizeCombo->addItem(QString());
        actionSizeCombo->setObjectName(QString::fromUtf8("actionSizeCombo"));
        actionSizeCombo->setGeometry(QRect(90, 20, 101, 22));
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 50, 81, 16));
        actionImgQualityEdit = new QLineEdit(groupBox);
        actionImgQualityEdit->setObjectName(QString::fromUtf8("actionImgQualityEdit"));
        actionImgQualityEdit->setGeometry(QRect(90, 50, 31, 20));
        actionImgQualityEdit->setAlignment(Qt::AlignCenter);
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(130, 50, 21, 16));
        actionJGWCheckBox = new QCheckBox(groupBox);
        actionJGWCheckBox->setObjectName(QString::fromUtf8("actionJGWCheckBox"));
        actionJGWCheckBox->setGeometry(QRect(120, 70, 91, 19));
        actiondatabaseUpgradeCheckBox = new QCheckBox(groupBox);
        actiondatabaseUpgradeCheckBox->setObjectName(QString::fromUtf8("actiondatabaseUpgradeCheckBox"));
        actiondatabaseUpgradeCheckBox->setGeometry(QRect(120, 110, 141, 20));
        actionToLevelEdit = new QLineEdit(groupBox);
        actionToLevelEdit->setObjectName(QString::fromUtf8("actionToLevelEdit"));
        actionToLevelEdit->setGeometry(QRect(70, 110, 31, 20));
        actionToLevelEdit->setAlignment(Qt::AlignCenter);
        actionToLevelEdit->setReadOnly(true);
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 110, 51, 20));
        actionFromLevelEdit = new QLineEdit(groupBox);
        actionFromLevelEdit->setObjectName(QString::fromUtf8("actionFromLevelEdit"));
        actionFromLevelEdit->setGeometry(QRect(70, 80, 31, 20));
        actionFromLevelEdit->setAlignment(Qt::AlignCenter);
        actionFromLevelEdit->setReadOnly(true);
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 80, 61, 20));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(170, 130, 91, 16));
        label_2->setAlignment(Qt::AlignCenter);
        actionUseLowerImageCheckBox = new QCheckBox(groupBox);
        actionUseLowerImageCheckBox->setObjectName(QString::fromUtf8("actionUseLowerImageCheckBox"));
        actionUseLowerImageCheckBox->setGeometry(QRect(120, 90, 141, 19));
        actionUseLowerImageCheckBox->setChecked(true);
        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 150, 251, 31));
        groupBox_4->setAlignment(Qt::AlignCenter);
        actionOperationSumBtn = new QRadioButton(groupBox_4);
        actionOperationSumBtn->setObjectName(QString::fromUtf8("actionOperationSumBtn"));
        actionOperationSumBtn->setGeometry(QRect(140, 10, 51, 19));
        actionOperationSourceBtn = new QRadioButton(groupBox_4);
        actionOperationSourceBtn->setObjectName(QString::fromUtf8("actionOperationSourceBtn"));
        actionOperationSourceBtn->setGeometry(QRect(80, 10, 61, 20));
        actionOperationTargetBtn = new QRadioButton(groupBox_4);
        actionOperationTargetBtn->setObjectName(QString::fromUtf8("actionOperationTargetBtn"));
        actionOperationTargetBtn->setGeometry(QRect(10, 10, 61, 19));
        actionOperationTargetBtn->setChecked(true);
        actionOperationCrossBtn = new QRadioButton(groupBox_4);
        actionOperationCrossBtn->setObjectName(QString::fromUtf8("actionOperationCrossBtn"));
        actionOperationCrossBtn->setGeometry(QRect(200, 10, 51, 19));
        actionOnlyJGWCheckBox = new QCheckBox(groupBox);
        actionOnlyJGWCheckBox->setObjectName(QString::fromUtf8("actionOnlyJGWCheckBox"));
        actionOnlyJGWCheckBox->setGeometry(QRect(150, 50, 101, 19));
        actionMultiText = new QLabel(groupBox);
        actionMultiText->setObjectName(QString::fromUtf8("actionMultiText"));
        actionMultiText->setGeometry(QRect(190, 20, 71, 20));
        actionMultiText->setAlignment(Qt::AlignCenter);
        actionSourceProgress = new QProgressBar(ImageMergeClass);
        actionSourceProgress->setObjectName(QString::fromUtf8("actionSourceProgress"));
        actionSourceProgress->setGeometry(QRect(20, 300, 251, 23));
        actionSourceProgress->setValue(0);
        label = new QLabel(ImageMergeClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 131, 20));
        label->setAlignment(Qt::AlignCenter);
        actionlbltmpfile = new QLabel(ImageMergeClass);
        actionlbltmpfile->setObjectName(QString::fromUtf8("actionlbltmpfile"));
        actionlbltmpfile->setGeometry(QRect(20, 260, 171, 20));
        actionBlockCheckBox = new QCheckBox(ImageMergeClass);
        actionBlockCheckBox->setObjectName(QString::fromUtf8("actionBlockCheckBox"));
        actionBlockCheckBox->setGeometry(QRect(30, 40, 81, 19));

        retranslateUi(ImageMergeClass);

        QMetaObject::connectSlotsByName(ImageMergeClass);
    } // setupUi

    void retranslateUi(QDialog *ImageMergeClass)
    {
        ImageMergeClass->setWindowTitle(QCoreApplication::translate("ImageMergeClass", "ImageMerge", nullptr));
        label_9->setText(QCoreApplication::translate("ImageMergeClass", "Dest Dir", nullptr));
        label_3->setText(QCoreApplication::translate("ImageMergeClass", "Source Dir", nullptr));
        actionSourceDirBtn->setText(QCoreApplication::translate("ImageMergeClass", "...", nullptr));
        actionDestDirEdit->setText(QString());
        actionDestDirBtn->setText(QCoreApplication::translate("ImageMergeClass", "...", nullptr));
        actionlblSourcefile->setText(QString());
        actionRunBtn->setText(QCoreApplication::translate("ImageMergeClass", "Run", nullptr));
        actionlbldestfile->setText(QString());
        actionOptiongroup->setTitle(QCoreApplication::translate("ImageMergeClass", "Options", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("ImageMergeClass", "Source  Range", nullptr));
        label_6->setText(QCoreApplication::translate("ImageMergeClass", "Left", nullptr));
        label_7->setText(QCoreApplication::translate("ImageMergeClass", "Right", nullptr));
        actionLeftEdit->setText(QCoreApplication::translate("ImageMergeClass", "126.376356", nullptr));
        actionRightEdit->setText(QCoreApplication::translate("ImageMergeClass", "131.259322", nullptr));
        label_4->setText(QCoreApplication::translate("ImageMergeClass", "Top", nullptr));
        label_5->setText(QCoreApplication::translate("ImageMergeClass", "Bottom", nullptr));
        actionBottomEdit->setText(QCoreApplication::translate("ImageMergeClass", "42.286147", nullptr));
        actionTopEdit->setText(QCoreApplication::translate("ImageMergeClass", "45.641165", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("ImageMergeClass", "Target Range", nullptr));
        label_14->setText(QCoreApplication::translate("ImageMergeClass", "Left", nullptr));
        label_15->setText(QCoreApplication::translate("ImageMergeClass", "Right", nullptr));
        actiontargetLeftEdit->setText(QCoreApplication::translate("ImageMergeClass", "126.376356", nullptr));
        actiontargetRightEdit->setText(QCoreApplication::translate("ImageMergeClass", "131.259322", nullptr));
        label_16->setText(QCoreApplication::translate("ImageMergeClass", "Top", nullptr));
        label_17->setText(QCoreApplication::translate("ImageMergeClass", "Bottom", nullptr));
        actiontargetBottomEdit->setText(QCoreApplication::translate("ImageMergeClass", "42.286147", nullptr));
        actiontargetTopEdit->setText(QCoreApplication::translate("ImageMergeClass", "45.641165", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ImageMergeClass", "Output", nullptr));
        label_12->setText(QCoreApplication::translate("ImageMergeClass", "Image Size", nullptr));
        actionSizeCombo->setItemText(0, QCoreApplication::translate("ImageMergeClass", "256*256", nullptr));
        actionSizeCombo->setItemText(1, QCoreApplication::translate("ImageMergeClass", "512*512", nullptr));
        actionSizeCombo->setItemText(2, QCoreApplication::translate("ImageMergeClass", "1024*1024", nullptr));
        actionSizeCombo->setItemText(3, QCoreApplication::translate("ImageMergeClass", "2048*2048", nullptr));
        actionSizeCombo->setItemText(4, QCoreApplication::translate("ImageMergeClass", "4096*4096", nullptr));
        actionSizeCombo->setItemText(5, QCoreApplication::translate("ImageMergeClass", "8192*8192", nullptr));

        label_10->setText(QCoreApplication::translate("ImageMergeClass", "Image Quality", nullptr));
        actionImgQualityEdit->setText(QCoreApplication::translate("ImageMergeClass", "75", nullptr));
        label_11->setText(QCoreApplication::translate("ImageMergeClass", "%", nullptr));
        actionJGWCheckBox->setText(QCoreApplication::translate("ImageMergeClass", "JGW writing", nullptr));
        actiondatabaseUpgradeCheckBox->setText(QCoreApplication::translate("ImageMergeClass", "database upgrade Mode", nullptr));
        actionToLevelEdit->setText(QCoreApplication::translate("ImageMergeClass", "11", nullptr));
        label_13->setText(QCoreApplication::translate("ImageMergeClass", "To Level", nullptr));
        actionFromLevelEdit->setText(QCoreApplication::translate("ImageMergeClass", "11", nullptr));
        label_8->setText(QCoreApplication::translate("ImageMergeClass", "From Level", nullptr));
        label_2->setText(QCoreApplication::translate("ImageMergeClass", "(256*256) size", nullptr));
        actionUseLowerImageCheckBox->setText(QCoreApplication::translate("ImageMergeClass", "Use Lower Level Image", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("ImageMergeClass", "Operation", nullptr));
        actionOperationSumBtn->setText(QCoreApplication::translate("ImageMergeClass", "Sum", nullptr));
        actionOperationSourceBtn->setText(QCoreApplication::translate("ImageMergeClass", "Source", nullptr));
        actionOperationTargetBtn->setText(QCoreApplication::translate("ImageMergeClass", "Target", nullptr));
        actionOperationCrossBtn->setText(QCoreApplication::translate("ImageMergeClass", "Cross", nullptr));
        actionOnlyJGWCheckBox->setText(QCoreApplication::translate("ImageMergeClass", "Only JGW writing", nullptr));
        actionMultiText->setText(QCoreApplication::translate("ImageMergeClass", "( 1 * 1 )", nullptr));
        label->setText(QCoreApplication::translate("ImageMergeClass", "Merge  Between images ", nullptr));
        actionlbltmpfile->setText(QString());
        actionBlockCheckBox->setText(QCoreApplication::translate("ImageMergeClass", "Block Mode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImageMergeClass: public Ui_ImageMergeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEMERGE_H
