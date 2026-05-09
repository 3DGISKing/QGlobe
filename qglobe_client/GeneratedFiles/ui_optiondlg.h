/********************************************************************************
** Form generated from reading UI file 'optiondlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONDLG_H
#define UI_OPTIONDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OptionDlg
{
public:
    QTabWidget *optionProperty;
    QWidget *viewTab;
    QGroupBox *labelSizeGroup;
    QRadioButton *actionSmallSize;
    QRadioButton *actionMediumSize;
    QRadioButton *actionLargeSize;
    QGroupBox *tudeShowGroup;
    QRadioButton *action10Notation;
    QRadioButton *actionDMS;
    QRadioButton *action10NotationDM;
    QGroupBox *distanceGroup;
    QRadioButton *actionFeetMile;
    QRadioButton *actionMeterKilo;
    QGroupBox *topoQualityGroup;
    QLabel *label_7;
    QSlider *actionTopoQualitySlider;
    QLabel *label_6;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLineEdit *actionHeightMagnify;
    QLabel *label_11;
    QCheckBox *actionToolTipCheck;
    QCheckBox *actionShowGISInfo;
    QGroupBox *labelSizeGroup_2;
    QSlider *cameraFOVHorizontalSlider;
    QLabel *label_24;
    QLabel *label_25;
    QWidget *cacheTab;
    QLineEdit *actionMemoryCacheSize;
    QLineEdit *actionDiskCacheSize;
    QPushButton *actionDeleteMemoryCache;
    QPushButton *actionDeleteDiskCache;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QWidget *navigationTab;
    QGroupBox *quickMoveGroup;
    QLabel *label;
    QLineEdit *actionQuickMoveSpeed;
    QSlider *actionQuickMoveSlider;
    QLabel *label_2;
    QLabel *label_3;
    QGroupBox *mouseWheelGroup;
    QLabel *label_5;
    QSlider *actionMouseWheelSlider;
    QLabel *label_4;
    QCheckBox *actionMouseWheelDirection;
    QGroupBox *touringGroup;
    QLineEdit *cameraTiltLineEdit;
    QSlider *cameraTiltSlider;
    QLabel *cameraTiltLabel;
    QLabel *degreesLabel;
    QLabel *metersLabel;
    QLineEdit *cameraRangeLineEdit;
    QLabel *cameraRangeLabel;
    QSlider *cameraRangeSlider;
    QLabel *speedLabel;
    QSlider *speedSlider;
    QLineEdit *speedLineEdit;
    QLabel *slowLabel;
    QLabel *fastLabel;
    QWidget *tab;
    QCheckBox *checkBoxEnableLighting;
    QGroupBox *groupBoxSunPosition;
    QLabel *label_18;
    QLineEdit *lineEditSunHeight;
    QLabel *label_16;
    QLineEdit *lineEditSunLongitude;
    QLabel *label_17;
    QLineEdit *lineEditSunLatitude;
    QGraphicsView *graphicsViewWorld;
    QGroupBox *groupBoxSunProperty;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_20;
    QPushButton *pushButtonAmbientColor;
    QLabel *label_19;
    QPushButton *pushButtonLightColor;
    QLabel *label_21;
    QDoubleSpinBox *doubleSpinBoxLightConstantAttenuation;
    QPushButton *actionDefaultButton;
    QPushButton *actionOkButton;
    QPushButton *actionCancelButton;
    QPushButton *actionApplyButton;

    void setupUi(QDialog *OptionDlg)
    {
        if (OptionDlg->objectName().isEmpty())
            OptionDlg->setObjectName(QString::fromUtf8("OptionDlg"));
        OptionDlg->setWindowModality(Qt::WindowModal);
        OptionDlg->resize(461, 470);
        OptionDlg->setMinimumSize(QSize(461, 470));
        OptionDlg->setMaximumSize(QSize(461, 470));
        OptionDlg->setModal(true);
        optionProperty = new QTabWidget(OptionDlg);
        optionProperty->setObjectName(QString::fromUtf8("optionProperty"));
        optionProperty->setGeometry(QRect(10, 10, 441, 420));
        viewTab = new QWidget();
        viewTab->setObjectName(QString::fromUtf8("viewTab"));
        labelSizeGroup = new QGroupBox(viewTab);
        labelSizeGroup->setObjectName(QString::fromUtf8("labelSizeGroup"));
        labelSizeGroup->setGeometry(QRect(10, 185, 411, 50));
        actionSmallSize = new QRadioButton(labelSizeGroup);
        actionSmallSize->setObjectName(QString::fromUtf8("actionSmallSize"));
        actionSmallSize->setGeometry(QRect(10, 25, 84, 18));
        actionMediumSize = new QRadioButton(labelSizeGroup);
        actionMediumSize->setObjectName(QString::fromUtf8("actionMediumSize"));
        actionMediumSize->setGeometry(QRect(120, 25, 84, 18));
        actionLargeSize = new QRadioButton(labelSizeGroup);
        actionLargeSize->setObjectName(QString::fromUtf8("actionLargeSize"));
        actionLargeSize->setGeometry(QRect(230, 25, 84, 18));
        tudeShowGroup = new QGroupBox(viewTab);
        tudeShowGroup->setObjectName(QString::fromUtf8("tudeShowGroup"));
        tudeShowGroup->setGeometry(QRect(10, 34, 211, 90));
        action10Notation = new QRadioButton(tudeShowGroup);
        action10Notation->setObjectName(QString::fromUtf8("action10Notation"));
        action10Notation->setGeometry(QRect(10, 65, 140, 18));
        actionDMS = new QRadioButton(tudeShowGroup);
        actionDMS->setObjectName(QString::fromUtf8("actionDMS"));
        actionDMS->setGeometry(QRect(10, 25, 200, 18));
        action10NotationDM = new QRadioButton(tudeShowGroup);
        action10NotationDM->setObjectName(QString::fromUtf8("action10NotationDM"));
        action10NotationDM->setGeometry(QRect(10, 45, 190, 18));
        distanceGroup = new QGroupBox(viewTab);
        distanceGroup->setObjectName(QString::fromUtf8("distanceGroup"));
        distanceGroup->setGeometry(QRect(230, 34, 191, 90));
        actionFeetMile = new QRadioButton(distanceGroup);
        actionFeetMile->setObjectName(QString::fromUtf8("actionFeetMile"));
        actionFeetMile->setGeometry(QRect(10, 60, 110, 18));
        actionMeterKilo = new QRadioButton(distanceGroup);
        actionMeterKilo->setObjectName(QString::fromUtf8("actionMeterKilo"));
        actionMeterKilo->setGeometry(QRect(10, 30, 170, 18));
        topoQualityGroup = new QGroupBox(viewTab);
        topoQualityGroup->setObjectName(QString::fromUtf8("topoQualityGroup"));
        topoQualityGroup->setGeometry(QRect(10, 240, 411, 130));
        label_7 = new QLabel(topoQualityGroup);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 30, 45, 16));
        actionTopoQualitySlider = new QSlider(topoQualityGroup);
        actionTopoQualitySlider->setObjectName(QString::fromUtf8("actionTopoQualitySlider"));
        actionTopoQualitySlider->setGeometry(QRect(58, 30, 291, 21));
        actionTopoQualitySlider->setOrientation(Qt::Horizontal);
        label_6 = new QLabel(topoQualityGroup);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(355, 30, 45, 20));
        label_8 = new QLabel(topoQualityGroup);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 50, 55, 16));
        label_9 = new QLabel(topoQualityGroup);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(350, 50, 55, 16));
        label_10 = new QLabel(topoQualityGroup);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 95, 155, 20));
        actionHeightMagnify = new QLineEdit(topoQualityGroup);
        actionHeightMagnify->setObjectName(QString::fromUtf8("actionHeightMagnify"));
        actionHeightMagnify->setGeometry(QRect(170, 95, 41, 20));
        label_11 = new QLabel(topoQualityGroup);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(220, 95, 71, 20));
        actionToolTipCheck = new QCheckBox(viewTab);
        actionToolTipCheck->setObjectName(QString::fromUtf8("actionToolTipCheck"));
        actionToolTipCheck->setGeometry(QRect(10, 10, 110, 20));
        actionShowGISInfo = new QCheckBox(viewTab);
        actionShowGISInfo->setObjectName(QString::fromUtf8("actionShowGISInfo"));
        actionShowGISInfo->setGeometry(QRect(230, 10, 150, 20));
        labelSizeGroup_2 = new QGroupBox(viewTab);
        labelSizeGroup_2->setObjectName(QString::fromUtf8("labelSizeGroup_2"));
        labelSizeGroup_2->setGeometry(QRect(10, 130, 411, 50));
        cameraFOVHorizontalSlider = new QSlider(labelSizeGroup_2);
        cameraFOVHorizontalSlider->setObjectName(QString::fromUtf8("cameraFOVHorizontalSlider"));
        cameraFOVHorizontalSlider->setGeometry(QRect(40, 20, 321, 20));
        cameraFOVHorizontalSlider->setMinimum(5);
        cameraFOVHorizontalSlider->setMaximum(175);
        cameraFOVHorizontalSlider->setPageStep(5);
        cameraFOVHorizontalSlider->setOrientation(Qt::Horizontal);
        label_24 = new QLabel(labelSizeGroup_2);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(10, 20, 21, 16));
        label_25 = new QLabel(labelSizeGroup_2);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(370, 20, 45, 20));
        optionProperty->addTab(viewTab, QString());
        cacheTab = new QWidget();
        cacheTab->setObjectName(QString::fromUtf8("cacheTab"));
        actionMemoryCacheSize = new QLineEdit(cacheTab);
        actionMemoryCacheSize->setObjectName(QString::fromUtf8("actionMemoryCacheSize"));
        actionMemoryCacheSize->setGeometry(QRect(290, 130, 100, 20));
        actionDiskCacheSize = new QLineEdit(cacheTab);
        actionDiskCacheSize->setObjectName(QString::fromUtf8("actionDiskCacheSize"));
        actionDiskCacheSize->setGeometry(QRect(290, 160, 100, 20));
        actionDeleteMemoryCache = new QPushButton(cacheTab);
        actionDeleteMemoryCache->setObjectName(QString::fromUtf8("actionDeleteMemoryCache"));
        actionDeleteMemoryCache->setGeometry(QRect(40, 220, 160, 24));
        actionDeleteMemoryCache->setFocusPolicy(Qt::StrongFocus);
        actionDeleteMemoryCache->setAutoDefault(false);
        actionDeleteDiskCache = new QPushButton(cacheTab);
        actionDeleteDiskCache->setObjectName(QString::fromUtf8("actionDeleteDiskCache"));
        actionDeleteDiskCache->setGeometry(QRect(210, 220, 180, 24));
        actionDeleteDiskCache->setFocusPolicy(Qt::StrongFocus);
        actionDeleteDiskCache->setAutoDefault(false);
        label_12 = new QLabel(cacheTab);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(40, 130, 195, 16));
        label_13 = new QLabel(cacheTab);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(40, 160, 195, 16));
        label_14 = new QLabel(cacheTab);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 40, 416, 21));
        label_15 = new QLabel(cacheTab);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 70, 416, 20));
        optionProperty->addTab(cacheTab, QString());
        navigationTab = new QWidget();
        navigationTab->setObjectName(QString::fromUtf8("navigationTab"));
        quickMoveGroup = new QGroupBox(navigationTab);
        quickMoveGroup->setObjectName(QString::fromUtf8("quickMoveGroup"));
        quickMoveGroup->setGeometry(QRect(10, 150, 411, 101));
        label = new QLabel(quickMoveGroup);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(230, 70, 101, 20));
        actionQuickMoveSpeed = new QLineEdit(quickMoveGroup);
        actionQuickMoveSpeed->setObjectName(QString::fromUtf8("actionQuickMoveSpeed"));
        actionQuickMoveSpeed->setGeometry(QRect(330, 70, 61, 20));
        actionQuickMoveSlider = new QSlider(quickMoveGroup);
        actionQuickMoveSlider->setObjectName(QString::fromUtf8("actionQuickMoveSlider"));
        actionQuickMoveSlider->setGeometry(QRect(49, 30, 311, 21));
        actionQuickMoveSlider->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(quickMoveGroup);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 30, 40, 20));
        label_3 = new QLabel(quickMoveGroup);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(365, 30, 40, 20));
        mouseWheelGroup = new QGroupBox(navigationTab);
        mouseWheelGroup->setObjectName(QString::fromUtf8("mouseWheelGroup"));
        mouseWheelGroup->setGeometry(QRect(10, 260, 411, 101));
        label_5 = new QLabel(mouseWheelGroup);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 30, 40, 20));
        actionMouseWheelSlider = new QSlider(mouseWheelGroup);
        actionMouseWheelSlider->setObjectName(QString::fromUtf8("actionMouseWheelSlider"));
        actionMouseWheelSlider->setGeometry(QRect(49, 30, 311, 21));
        actionMouseWheelSlider->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(mouseWheelGroup);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(365, 30, 40, 20));
        actionMouseWheelDirection = new QCheckBox(mouseWheelGroup);
        actionMouseWheelDirection->setObjectName(QString::fromUtf8("actionMouseWheelDirection"));
        actionMouseWheelDirection->setGeometry(QRect(20, 70, 251, 18));
        touringGroup = new QGroupBox(navigationTab);
        touringGroup->setObjectName(QString::fromUtf8("touringGroup"));
        touringGroup->setGeometry(QRect(10, 20, 411, 121));
        cameraTiltLineEdit = new QLineEdit(touringGroup);
        cameraTiltLineEdit->setObjectName(QString::fromUtf8("cameraTiltLineEdit"));
        cameraTiltLineEdit->setGeometry(QRect(101, 30, 60, 20));
        cameraTiltSlider = new QSlider(touringGroup);
        cameraTiltSlider->setObjectName(QString::fromUtf8("cameraTiltSlider"));
        cameraTiltSlider->setGeometry(QRect(210, 30, 191, 21));
        cameraTiltSlider->setMaximum(800);
        cameraTiltSlider->setSingleStep(1);
        cameraTiltSlider->setOrientation(Qt::Horizontal);
        cameraTiltLabel = new QLabel(touringGroup);
        cameraTiltLabel->setObjectName(QString::fromUtf8("cameraTiltLabel"));
        cameraTiltLabel->setGeometry(QRect(10, 30, 116, 20));
        degreesLabel = new QLabel(touringGroup);
        degreesLabel->setObjectName(QString::fromUtf8("degreesLabel"));
        degreesLabel->setGeometry(QRect(166, 30, 40, 20));
        metersLabel = new QLabel(touringGroup);
        metersLabel->setObjectName(QString::fromUtf8("metersLabel"));
        metersLabel->setGeometry(QRect(166, 60, 40, 20));
        cameraRangeLineEdit = new QLineEdit(touringGroup);
        cameraRangeLineEdit->setObjectName(QString::fromUtf8("cameraRangeLineEdit"));
        cameraRangeLineEdit->setGeometry(QRect(101, 60, 60, 20));
        cameraRangeLabel = new QLabel(touringGroup);
        cameraRangeLabel->setObjectName(QString::fromUtf8("cameraRangeLabel"));
        cameraRangeLabel->setGeometry(QRect(10, 60, 116, 20));
        cameraRangeSlider = new QSlider(touringGroup);
        cameraRangeSlider->setObjectName(QString::fromUtf8("cameraRangeSlider"));
        cameraRangeSlider->setGeometry(QRect(210, 60, 191, 21));
        cameraRangeSlider->setMinimum(1500);
        cameraRangeSlider->setMaximum(50000);
        cameraRangeSlider->setOrientation(Qt::Horizontal);
        speedLabel = new QLabel(touringGroup);
        speedLabel->setObjectName(QString::fromUtf8("speedLabel"));
        speedLabel->setGeometry(QRect(10, 90, 116, 20));
        speedSlider = new QSlider(touringGroup);
        speedSlider->setObjectName(QString::fromUtf8("speedSlider"));
        speedSlider->setGeometry(QRect(210, 90, 141, 21));
        speedSlider->setMinimum(500);
        speedSlider->setMaximum(10000);
        speedSlider->setValue(500);
        speedSlider->setOrientation(Qt::Horizontal);
        speedLineEdit = new QLineEdit(touringGroup);
        speedLineEdit->setObjectName(QString::fromUtf8("speedLineEdit"));
        speedLineEdit->setGeometry(QRect(101, 90, 60, 20));
        slowLabel = new QLabel(touringGroup);
        slowLabel->setObjectName(QString::fromUtf8("slowLabel"));
        slowLabel->setGeometry(QRect(166, 90, 40, 20));
        fastLabel = new QLabel(touringGroup);
        fastLabel->setObjectName(QString::fromUtf8("fastLabel"));
        fastLabel->setGeometry(QRect(360, 90, 40, 20));
        optionProperty->addTab(navigationTab, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        checkBoxEnableLighting = new QCheckBox(tab);
        checkBoxEnableLighting->setObjectName(QString::fromUtf8("checkBoxEnableLighting"));
        checkBoxEnableLighting->setGeometry(QRect(10, 10, 151, 18));
        groupBoxSunPosition = new QGroupBox(tab);
        groupBoxSunPosition->setObjectName(QString::fromUtf8("groupBoxSunPosition"));
        groupBoxSunPosition->setGeometry(QRect(10, 250, 421, 61));
        label_18 = new QLabel(groupBoxSunPosition);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(280, 25, 56, 16));
        lineEditSunHeight = new QLineEdit(groupBoxSunPosition);
        lineEditSunHeight->setObjectName(QString::fromUtf8("lineEditSunHeight"));
        lineEditSunHeight->setGeometry(QRect(345, 23, 60, 20));
        label_16 = new QLabel(groupBoxSunPosition);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(12, 25, 51, 16));
        lineEditSunLongitude = new QLineEdit(groupBoxSunPosition);
        lineEditSunLongitude->setObjectName(QString::fromUtf8("lineEditSunLongitude"));
        lineEditSunLongitude->setGeometry(QRect(70, 23, 60, 20));
        label_17 = new QLabel(groupBoxSunPosition);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(148, 25, 43, 16));
        lineEditSunLatitude = new QLineEdit(groupBoxSunPosition);
        lineEditSunLatitude->setObjectName(QString::fromUtf8("lineEditSunLatitude"));
        lineEditSunLatitude->setGeometry(QRect(200, 23, 60, 20));
        graphicsViewWorld = new QGraphicsView(tab);
        graphicsViewWorld->setObjectName(QString::fromUtf8("graphicsViewWorld"));
        graphicsViewWorld->setGeometry(QRect(9, 33, 420, 210));
        groupBoxSunProperty = new QGroupBox(tab);
        groupBoxSunProperty->setObjectName(QString::fromUtf8("groupBoxSunProperty"));
        groupBoxSunProperty->setGeometry(QRect(10, 322, 421, 64));
        layoutWidget = new QWidget(groupBoxSunProperty);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 14, 401, 41));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_20 = new QLabel(layoutWidget);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        horizontalLayout_3->addWidget(label_20);

        pushButtonAmbientColor = new QPushButton(layoutWidget);
        pushButtonAmbientColor->setObjectName(QString::fromUtf8("pushButtonAmbientColor"));

        horizontalLayout_3->addWidget(pushButtonAmbientColor);

        label_19 = new QLabel(layoutWidget);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        horizontalLayout_3->addWidget(label_19);

        pushButtonLightColor = new QPushButton(layoutWidget);
        pushButtonLightColor->setObjectName(QString::fromUtf8("pushButtonLightColor"));

        horizontalLayout_3->addWidget(pushButtonLightColor);

        label_21 = new QLabel(layoutWidget);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        horizontalLayout_3->addWidget(label_21);

        doubleSpinBoxLightConstantAttenuation = new QDoubleSpinBox(layoutWidget);
        doubleSpinBoxLightConstantAttenuation->setObjectName(QString::fromUtf8("doubleSpinBoxLightConstantAttenuation"));
        doubleSpinBoxLightConstantAttenuation->setMinimum(0.100000000000000);
        doubleSpinBoxLightConstantAttenuation->setSingleStep(0.100000000000000);

        horizontalLayout_3->addWidget(doubleSpinBoxLightConstantAttenuation);

        optionProperty->addTab(tab, QString());
        actionDefaultButton = new QPushButton(OptionDlg);
        actionDefaultButton->setObjectName(QString::fromUtf8("actionDefaultButton"));
        actionDefaultButton->setGeometry(QRect(10, 440, 120, 24));
        actionDefaultButton->setFocusPolicy(Qt::StrongFocus);
        actionDefaultButton->setAutoDefault(false);
        actionOkButton = new QPushButton(OptionDlg);
        actionOkButton->setObjectName(QString::fromUtf8("actionOkButton"));
        actionOkButton->setGeometry(QRect(215, 440, 75, 24));
        actionOkButton->setFocusPolicy(Qt::StrongFocus);
        actionOkButton->setAutoDefault(false);
        actionCancelButton = new QPushButton(OptionDlg);
        actionCancelButton->setObjectName(QString::fromUtf8("actionCancelButton"));
        actionCancelButton->setGeometry(QRect(295, 440, 75, 24));
        actionCancelButton->setFocusPolicy(Qt::StrongFocus);
        actionCancelButton->setAutoDefault(false);
        actionApplyButton = new QPushButton(OptionDlg);
        actionApplyButton->setObjectName(QString::fromUtf8("actionApplyButton"));
        actionApplyButton->setGeometry(QRect(375, 440, 75, 24));
        actionApplyButton->setFocusPolicy(Qt::StrongFocus);
        actionApplyButton->setAutoDefault(false);
        QWidget::setTabOrder(actionOkButton, actionCancelButton);
        QWidget::setTabOrder(actionCancelButton, actionApplyButton);
        QWidget::setTabOrder(actionApplyButton, optionProperty);
        QWidget::setTabOrder(optionProperty, actionToolTipCheck);
        QWidget::setTabOrder(actionToolTipCheck, actionShowGISInfo);
        QWidget::setTabOrder(actionShowGISInfo, actionDMS);
        QWidget::setTabOrder(actionDMS, action10NotationDM);
        QWidget::setTabOrder(action10NotationDM, action10Notation);
        QWidget::setTabOrder(action10Notation, actionMeterKilo);
        QWidget::setTabOrder(actionMeterKilo, actionFeetMile);
        QWidget::setTabOrder(actionFeetMile, actionSmallSize);
        QWidget::setTabOrder(actionSmallSize, actionMediumSize);
        QWidget::setTabOrder(actionMediumSize, actionLargeSize);
        QWidget::setTabOrder(actionLargeSize, actionTopoQualitySlider);
        QWidget::setTabOrder(actionTopoQualitySlider, actionHeightMagnify);
        QWidget::setTabOrder(actionHeightMagnify, actionMemoryCacheSize);
        QWidget::setTabOrder(actionMemoryCacheSize, actionDiskCacheSize);
        QWidget::setTabOrder(actionDiskCacheSize, actionDeleteMemoryCache);
        QWidget::setTabOrder(actionDeleteMemoryCache, actionDeleteDiskCache);
        QWidget::setTabOrder(actionDeleteDiskCache, cameraTiltLineEdit);
        QWidget::setTabOrder(cameraTiltLineEdit, cameraTiltSlider);
        QWidget::setTabOrder(cameraTiltSlider, cameraRangeLineEdit);
        QWidget::setTabOrder(cameraRangeLineEdit, cameraRangeSlider);
        QWidget::setTabOrder(cameraRangeSlider, speedLineEdit);
        QWidget::setTabOrder(speedLineEdit, speedSlider);
        QWidget::setTabOrder(speedSlider, actionQuickMoveSlider);
        QWidget::setTabOrder(actionQuickMoveSlider, actionQuickMoveSpeed);
        QWidget::setTabOrder(actionQuickMoveSpeed, actionMouseWheelSlider);
        QWidget::setTabOrder(actionMouseWheelSlider, actionMouseWheelDirection);
        QWidget::setTabOrder(actionMouseWheelDirection, actionDefaultButton);

        retranslateUi(OptionDlg);

        optionProperty->setCurrentIndex(3);
        actionOkButton->setDefault(true);


        QMetaObject::connectSlotsByName(OptionDlg);
    } // setupUi

    void retranslateUi(QDialog *OptionDlg)
    {
        OptionDlg->setWindowTitle(QCoreApplication::translate("OptionDlg", "Options", nullptr));
        labelSizeGroup->setTitle(QCoreApplication::translate("OptionDlg", "Labels/Icon Size", nullptr));
        actionSmallSize->setText(QCoreApplication::translate("OptionDlg", "Small", nullptr));
        actionMediumSize->setText(QCoreApplication::translate("OptionDlg", "Medium", nullptr));
        actionLargeSize->setText(QCoreApplication::translate("OptionDlg", "Large", nullptr));
        tudeShowGroup->setTitle(QCoreApplication::translate("OptionDlg", "Show Lat/Long", nullptr));
        action10Notation->setText(QCoreApplication::translate("OptionDlg", "Decimal Degrees", nullptr));
        actionDMS->setText(QCoreApplication::translate("OptionDlg", "Degrees, Minutes, Seconds", nullptr));
        action10NotationDM->setText(QCoreApplication::translate("OptionDlg", "Degrees, Decimal Minutes", nullptr));
        distanceGroup->setTitle(QCoreApplication::translate("OptionDlg", "Show Elevation", nullptr));
        actionFeetMile->setText(QCoreApplication::translate("OptionDlg", "Feet, Miles", nullptr));
        actionMeterKilo->setText(QCoreApplication::translate("OptionDlg", "Meters, Kilometers", nullptr));
        topoQualityGroup->setTitle(QCoreApplication::translate("OptionDlg", "Terrain Quality", nullptr));
        label_7->setText(QCoreApplication::translate("OptionDlg", "Lower", nullptr));
        label_6->setText(QCoreApplication::translate("OptionDlg", "Higher", nullptr));
        label_8->setText(QCoreApplication::translate("OptionDlg", "(faster)", nullptr));
        label_9->setText(QCoreApplication::translate("OptionDlg", "(slower)", nullptr));
        label_10->setText(QCoreApplication::translate("OptionDlg", "Elevation Exaggeration:", nullptr));
        label_11->setText(QCoreApplication::translate("OptionDlg", "(0.5 - 3)", nullptr));
        actionToolTipCheck->setText(QCoreApplication::translate("OptionDlg", "Show Tooltip", nullptr));
        actionShowGISInfo->setText(QCoreApplication::translate("OptionDlg", "Show GIS Info", nullptr));
        labelSizeGroup_2->setTitle(QCoreApplication::translate("OptionDlg", "Camera FOV", nullptr));
        label_24->setText(QCoreApplication::translate("OptionDlg", "5", nullptr));
        label_25->setText(QCoreApplication::translate("OptionDlg", "175", nullptr));
        optionProperty->setTabText(optionProperty->indexOf(viewTab), QCoreApplication::translate("OptionDlg", "3D View", nullptr));
        actionDeleteMemoryCache->setText(QCoreApplication::translate("OptionDlg", "Clear memory cache", nullptr));
        actionDeleteDiskCache->setText(QCoreApplication::translate("OptionDlg", "Clear disk cache", nullptr));
        label_12->setText(QCoreApplication::translate("OptionDlg", "Memory Cache Size(MB):", nullptr));
        label_13->setText(QCoreApplication::translate("OptionDlg", "Disk Cache Size(MB):", nullptr));
        label_14->setText(QCoreApplication::translate("OptionDlg", "Maximum memory cache size is dependent on the amount on the", nullptr));
        label_15->setText(QCoreApplication::translate("OptionDlg", "physical memory. Disk cache size may be up to 2GB.", nullptr));
        optionProperty->setTabText(optionProperty->indexOf(cacheTab), QCoreApplication::translate("OptionDlg", "Cache", nullptr));
        quickMoveGroup->setTitle(QCoreApplication::translate("OptionDlg", "Fly-To", nullptr));
        label->setText(QCoreApplication::translate("OptionDlg", "Fly-To Speed:", nullptr));
        label_2->setText(QCoreApplication::translate("OptionDlg", "Slow", nullptr));
        label_3->setText(QCoreApplication::translate("OptionDlg", "Fast", nullptr));
        mouseWheelGroup->setTitle(QCoreApplication::translate("OptionDlg", "Mouse Wheel", nullptr));
        label_5->setText(QCoreApplication::translate("OptionDlg", "Slow", nullptr));
        label_4->setText(QCoreApplication::translate("OptionDlg", "Fast", nullptr));
        actionMouseWheelDirection->setText(QCoreApplication::translate("OptionDlg", "Invert mouse wheel zoom direction", nullptr));
        touringGroup->setTitle(QCoreApplication::translate("OptionDlg", "Touring", nullptr));
        cameraTiltLabel->setText(QCoreApplication::translate("OptionDlg", "Camera Tilt Angle:", nullptr));
        degreesLabel->setText(QCoreApplication::translate("OptionDlg", "Degrees", nullptr));
        metersLabel->setText(QCoreApplication::translate("OptionDlg", "Meters", nullptr));
        cameraRangeLabel->setText(QCoreApplication::translate("OptionDlg", "Camera Range:", nullptr));
        speedLabel->setText(QCoreApplication::translate("OptionDlg", "Speed:", nullptr));
        slowLabel->setText(QCoreApplication::translate("OptionDlg", "Slow", nullptr));
        fastLabel->setText(QCoreApplication::translate("OptionDlg", "Fast", nullptr));
        optionProperty->setTabText(optionProperty->indexOf(navigationTab), QCoreApplication::translate("OptionDlg", "Navigation", nullptr));
        checkBoxEnableLighting->setText(QCoreApplication::translate("OptionDlg", "Enable Sun Lighting", nullptr));
        groupBoxSunPosition->setTitle(QCoreApplication::translate("OptionDlg", "Sun Position", nullptr));
        label_18->setText(QCoreApplication::translate("OptionDlg", "Height:(km)", nullptr));
        label_16->setText(QCoreApplication::translate("OptionDlg", "Longitude:", nullptr));
        label_17->setText(QCoreApplication::translate("OptionDlg", "Latitude:", nullptr));
        groupBoxSunProperty->setTitle(QCoreApplication::translate("OptionDlg", "Sun Property", nullptr));
        label_20->setText(QCoreApplication::translate("OptionDlg", "Ambient Color", nullptr));
        pushButtonAmbientColor->setText(QString());
        label_19->setText(QCoreApplication::translate("OptionDlg", "SunLight Color", nullptr));
        pushButtonLightColor->setText(QString());
        label_21->setText(QCoreApplication::translate("OptionDlg", "SunLight Level", nullptr));
        optionProperty->setTabText(optionProperty->indexOf(tab), QCoreApplication::translate("OptionDlg", "Sun Lighting", nullptr));
        actionDefaultButton->setText(QCoreApplication::translate("OptionDlg", "Restore Defaults", nullptr));
        actionOkButton->setText(QCoreApplication::translate("OptionDlg", "Ok", nullptr));
        actionCancelButton->setText(QCoreApplication::translate("OptionDlg", "Cancel", nullptr));
        actionApplyButton->setText(QCoreApplication::translate("OptionDlg", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OptionDlg: public Ui_OptionDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONDLG_H
