/********************************************************************************
** Form generated from reading UI file 'objectadddlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBJECTADDDLG_H
#define UI_OBJECTADDDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ObjectAddDlg
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *filenameEdit;
    QPushButton *openButton;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_12;
    QComboBox *comboBoxObjectType;
    QTabWidget *tabWidget;
    QWidget *viewTab;
    QGroupBox *groupBox_2;
    QDoubleSpinBox *heightDoubleSpinBox;
    QLabel *fillColorLabel;
    QToolButton *setDirectionButton;
    QPushButton *Rotate90AroundXButton;
    QDoubleSpinBox *lengthDoubleSpinBox;
    QDoubleSpinBox *azimuthDoubleSpinBox;
    QLabel *latitudeLabel;
    QToolButton *backgroundMoveButton;
    QDoubleSpinBox *heightDoubleSpinBox1;
    QToolButton *scalingXAxisButton;
    QToolButton *scalingYAxisButton;
    QLabel *fillColorLabel_3;
    QToolButton *scalingAllAxisButton;
    QToolButton *selectButton;
    QToolButton *setHeightButton;
    QLabel *rangeLabel;
    QDoubleSpinBox *longitudeDoubleSpinBox;
    QPushButton *flipButton;
    QDoubleSpinBox *widthDoubleSpinBox;
    QToolButton *scalingZAxisButton;
    QLabel *longitudeLabel;
    QLabel *rangeLabel_2;
    QLabel *fillColorLabel_4;
    QPushButton *Rotate90AroundYButton;
    QToolButton *setPositionButton;
    QPushButton *InvertXAxisButton;
    QDoubleSpinBox *latitudeDoubleSpinBox;
    QToolButton *scalingXYAxisButton;
    QToolButton *RotateXAxisButton;
    QToolButton *RotateYAxisButton;
    QCheckBox *checkBoxPositionSave;
    QCheckBox *checkBoxMultiSelect;
    QGroupBox *groupBox_3;
    QCheckBox *checkBoxFog;
    QPushButton *pushButtonDiffuseColor;
    QPushButton *pushButtonEmissiveColor;
    QComboBox *materialComboBox;
    QLabel *label_5;
    QLabel *label_4;
    QSlider *horizontalSliderShiness;
    QPushButton *pushButtonSpecularColor;
    QCheckBox *checkBoxLighting;
    QLabel *label_14;
    QLabel *label_8;
    QLabel *label;
    QLabel *label_13;
    QPushButton *pushButtonAmbientColor;
    QWidget *buildingPropertyTab;
    QLabel *label_2;
    QLineEdit *lineEditBuildingName;
    QLineEdit *lineEditPostName;
    QLabel *label_3;
    QSpinBox *spinBoxFloor;
    QLabel *label_11;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_15;
    QTextEdit *textEditMisc;
    QLabel *label_6;
    QComboBox *comboBoxProvince;
    QLabel *label_7;
    QComboBox *comboBoxCounty;
    QLabel *label_10;
    QLineEdit *lineEditBan;
    QLabel *label_9;
    QComboBox *comboBoxVilliage;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonDelete;
    QPushButton *saveButton;
    QPushButton *closeButton;
    QButtonGroup *buttonGroup;

    void setupUi(QDialog *ObjectAddDlg)
    {
        if (ObjectAddDlg->objectName().isEmpty())
            ObjectAddDlg->setObjectName(QString::fromUtf8("ObjectAddDlg"));
        ObjectAddDlg->resize(392, 572);
        verticalLayout = new QVBoxLayout(ObjectAddDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        filenameEdit = new QLineEdit(ObjectAddDlg);
        filenameEdit->setObjectName(QString::fromUtf8("filenameEdit"));
        filenameEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(filenameEdit);

        openButton = new QPushButton(ObjectAddDlg);
        openButton->setObjectName(QString::fromUtf8("openButton"));
        openButton->setFocusPolicy(Qt::StrongFocus);
        openButton->setCheckable(false);
        openButton->setAutoDefault(false);

        horizontalLayout_4->addWidget(openButton);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_12 = new QLabel(ObjectAddDlg);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_7->addWidget(label_12);

        comboBoxObjectType = new QComboBox(ObjectAddDlg);
        comboBoxObjectType->setObjectName(QString::fromUtf8("comboBoxObjectType"));
        comboBoxObjectType->setEnabled(true);

        horizontalLayout_7->addWidget(comboBoxObjectType);


        verticalLayout->addLayout(horizontalLayout_7);

        tabWidget = new QTabWidget(ObjectAddDlg);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        viewTab = new QWidget();
        viewTab->setObjectName(QString::fromUtf8("viewTab"));
        groupBox_2 = new QGroupBox(viewTab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 0, 351, 290));
        heightDoubleSpinBox = new QDoubleSpinBox(groupBox_2);
        heightDoubleSpinBox->setObjectName(QString::fromUtf8("heightDoubleSpinBox"));
        heightDoubleSpinBox->setEnabled(true);
        heightDoubleSpinBox->setGeometry(QRect(70, 232, 90, 20));
        heightDoubleSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        heightDoubleSpinBox->setMinimum(-6378137.000000000000000);
        heightDoubleSpinBox->setMaximum(6378137.000000000000000);
        fillColorLabel = new QLabel(groupBox_2);
        fillColorLabel->setObjectName(QString::fromUtf8("fillColorLabel"));
        fillColorLabel->setGeometry(QRect(170, 173, 50, 23));
        setDirectionButton = new QToolButton(groupBox_2);
        buttonGroup = new QButtonGroup(ObjectAddDlg);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(setDirectionButton);
        setDirectionButton->setObjectName(QString::fromUtf8("setDirectionButton"));
        setDirectionButton->setEnabled(true);
        setDirectionButton->setGeometry(QRect(10, 60, 100, 21));
        setDirectionButton->setCheckable(true);
        setDirectionButton->setAutoExclusive(true);
        Rotate90AroundXButton = new QPushButton(groupBox_2);
        Rotate90AroundXButton->setObjectName(QString::fromUtf8("Rotate90AroundXButton"));
        Rotate90AroundXButton->setEnabled(true);
        Rotate90AroundXButton->setGeometry(QRect(230, 84, 100, 25));
        lengthDoubleSpinBox = new QDoubleSpinBox(groupBox_2);
        lengthDoubleSpinBox->setObjectName(QString::fromUtf8("lengthDoubleSpinBox"));
        lengthDoubleSpinBox->setEnabled(true);
        lengthDoubleSpinBox->setGeometry(QRect(230, 173, 90, 20));
        lengthDoubleSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lengthDoubleSpinBox->setDecimals(3);
        lengthDoubleSpinBox->setMinimum(0.000000000000000);
        lengthDoubleSpinBox->setMaximum(10000.000000000000000);
        lengthDoubleSpinBox->setSingleStep(0.500000000000000);
        lengthDoubleSpinBox->setValue(50.000000000000000);
        azimuthDoubleSpinBox = new QDoubleSpinBox(groupBox_2);
        azimuthDoubleSpinBox->setObjectName(QString::fromUtf8("azimuthDoubleSpinBox"));
        azimuthDoubleSpinBox->setEnabled(true);
        azimuthDoubleSpinBox->setGeometry(QRect(70, 262, 90, 20));
        azimuthDoubleSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        azimuthDoubleSpinBox->setMaximum(360.000000000000000);
        latitudeLabel = new QLabel(groupBox_2);
        latitudeLabel->setObjectName(QString::fromUtf8("latitudeLabel"));
        latitudeLabel->setGeometry(QRect(10, 203, 90, 20));
        backgroundMoveButton = new QToolButton(groupBox_2);
        buttonGroup->addButton(backgroundMoveButton);
        backgroundMoveButton->setObjectName(QString::fromUtf8("backgroundMoveButton"));
        backgroundMoveButton->setEnabled(true);
        backgroundMoveButton->setGeometry(QRect(230, 40, 100, 21));
        backgroundMoveButton->setCheckable(true);
        backgroundMoveButton->setAutoExclusive(true);
        heightDoubleSpinBox1 = new QDoubleSpinBox(groupBox_2);
        heightDoubleSpinBox1->setObjectName(QString::fromUtf8("heightDoubleSpinBox1"));
        heightDoubleSpinBox1->setEnabled(true);
        heightDoubleSpinBox1->setGeometry(QRect(230, 236, 90, 20));
        heightDoubleSpinBox1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        heightDoubleSpinBox1->setDecimals(3);
        heightDoubleSpinBox1->setMinimum(0.000000000000000);
        heightDoubleSpinBox1->setMaximum(10000.000000000000000);
        heightDoubleSpinBox1->setSingleStep(0.500000000000000);
        heightDoubleSpinBox1->setValue(50.000000000000000);
        scalingXAxisButton = new QToolButton(groupBox_2);
        buttonGroup->addButton(scalingXAxisButton);
        scalingXAxisButton->setObjectName(QString::fromUtf8("scalingXAxisButton"));
        scalingXAxisButton->setEnabled(true);
        scalingXAxisButton->setGeometry(QRect(120, 80, 100, 21));
        scalingXAxisButton->setCheckable(true);
        scalingXAxisButton->setAutoExclusive(true);
        scalingYAxisButton = new QToolButton(groupBox_2);
        buttonGroup->addButton(scalingYAxisButton);
        scalingYAxisButton->setObjectName(QString::fromUtf8("scalingYAxisButton"));
        scalingYAxisButton->setEnabled(true);
        scalingYAxisButton->setGeometry(QRect(120, 100, 100, 21));
        scalingYAxisButton->setCheckable(true);
        scalingYAxisButton->setAutoExclusive(true);
        fillColorLabel_3 = new QLabel(groupBox_2);
        fillColorLabel_3->setObjectName(QString::fromUtf8("fillColorLabel_3"));
        fillColorLabel_3->setGeometry(QRect(170, 201, 50, 23));
        scalingAllAxisButton = new QToolButton(groupBox_2);
        buttonGroup->addButton(scalingAllAxisButton);
        scalingAllAxisButton->setObjectName(QString::fromUtf8("scalingAllAxisButton"));
        scalingAllAxisButton->setEnabled(true);
        scalingAllAxisButton->setGeometry(QRect(120, 40, 100, 21));
        scalingAllAxisButton->setCheckable(true);
        scalingAllAxisButton->setAutoExclusive(true);
        selectButton = new QToolButton(groupBox_2);
        buttonGroup->addButton(selectButton);
        selectButton->setObjectName(QString::fromUtf8("selectButton"));
        selectButton->setEnabled(true);
        selectButton->setGeometry(QRect(230, 60, 100, 21));
        selectButton->setCheckable(true);
        selectButton->setChecked(false);
        selectButton->setAutoExclusive(true);
        setHeightButton = new QToolButton(groupBox_2);
        buttonGroup->addButton(setHeightButton);
        setHeightButton->setObjectName(QString::fromUtf8("setHeightButton"));
        setHeightButton->setEnabled(true);
        setHeightButton->setGeometry(QRect(10, 80, 100, 21));
        setHeightButton->setCheckable(true);
        setHeightButton->setAutoExclusive(true);
        rangeLabel = new QLabel(groupBox_2);
        rangeLabel->setObjectName(QString::fromUtf8("rangeLabel"));
        rangeLabel->setGeometry(QRect(10, 232, 90, 20));
        longitudeDoubleSpinBox = new QDoubleSpinBox(groupBox_2);
        longitudeDoubleSpinBox->setObjectName(QString::fromUtf8("longitudeDoubleSpinBox"));
        longitudeDoubleSpinBox->setEnabled(true);
        longitudeDoubleSpinBox->setGeometry(QRect(70, 173, 90, 20));
        longitudeDoubleSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        longitudeDoubleSpinBox->setDecimals(6);
        longitudeDoubleSpinBox->setMinimum(-180.000000000000000);
        longitudeDoubleSpinBox->setMaximum(180.000000000000000);
        longitudeDoubleSpinBox->setSingleStep(0.000001000000000);
        flipButton = new QPushButton(groupBox_2);
        flipButton->setObjectName(QString::fromUtf8("flipButton"));
        flipButton->setEnabled(true);
        flipButton->setGeometry(QRect(10, 144, 100, 25));
        widthDoubleSpinBox = new QDoubleSpinBox(groupBox_2);
        widthDoubleSpinBox->setObjectName(QString::fromUtf8("widthDoubleSpinBox"));
        widthDoubleSpinBox->setEnabled(true);
        widthDoubleSpinBox->setGeometry(QRect(230, 203, 90, 20));
        widthDoubleSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        widthDoubleSpinBox->setDecimals(3);
        widthDoubleSpinBox->setMinimum(0.000000000000000);
        widthDoubleSpinBox->setMaximum(10000.000000000000000);
        widthDoubleSpinBox->setSingleStep(0.500000000000000);
        widthDoubleSpinBox->setValue(50.000000000000000);
        scalingZAxisButton = new QToolButton(groupBox_2);
        buttonGroup->addButton(scalingZAxisButton);
        scalingZAxisButton->setObjectName(QString::fromUtf8("scalingZAxisButton"));
        scalingZAxisButton->setEnabled(true);
        scalingZAxisButton->setGeometry(QRect(120, 120, 100, 21));
        scalingZAxisButton->setCheckable(true);
        scalingZAxisButton->setAutoExclusive(true);
        longitudeLabel = new QLabel(groupBox_2);
        longitudeLabel->setObjectName(QString::fromUtf8("longitudeLabel"));
        longitudeLabel->setGeometry(QRect(10, 173, 90, 20));
        rangeLabel_2 = new QLabel(groupBox_2);
        rangeLabel_2->setObjectName(QString::fromUtf8("rangeLabel_2"));
        rangeLabel_2->setEnabled(true);
        rangeLabel_2->setGeometry(QRect(10, 262, 90, 20));
        fillColorLabel_4 = new QLabel(groupBox_2);
        fillColorLabel_4->setObjectName(QString::fromUtf8("fillColorLabel_4"));
        fillColorLabel_4->setGeometry(QRect(171, 252, 50, 23));
        Rotate90AroundYButton = new QPushButton(groupBox_2);
        Rotate90AroundYButton->setObjectName(QString::fromUtf8("Rotate90AroundYButton"));
        Rotate90AroundYButton->setGeometry(QRect(230, 115, 100, 25));
        setPositionButton = new QToolButton(groupBox_2);
        buttonGroup->addButton(setPositionButton);
        setPositionButton->setObjectName(QString::fromUtf8("setPositionButton"));
        setPositionButton->setEnabled(true);
        setPositionButton->setGeometry(QRect(10, 40, 101, 21));
        setPositionButton->setCheckable(true);
        setPositionButton->setChecked(true);
        setPositionButton->setAutoExclusive(true);
        InvertXAxisButton = new QPushButton(groupBox_2);
        InvertXAxisButton->setObjectName(QString::fromUtf8("InvertXAxisButton"));
        InvertXAxisButton->setEnabled(true);
        InvertXAxisButton->setGeometry(QRect(120, 144, 100, 25));
        latitudeDoubleSpinBox = new QDoubleSpinBox(groupBox_2);
        latitudeDoubleSpinBox->setObjectName(QString::fromUtf8("latitudeDoubleSpinBox"));
        latitudeDoubleSpinBox->setEnabled(true);
        latitudeDoubleSpinBox->setGeometry(QRect(70, 205, 90, 20));
        latitudeDoubleSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        latitudeDoubleSpinBox->setDecimals(6);
        latitudeDoubleSpinBox->setMinimum(-90.000000000000000);
        latitudeDoubleSpinBox->setMaximum(90.989999999999995);
        latitudeDoubleSpinBox->setSingleStep(0.000001000000000);
        scalingXYAxisButton = new QToolButton(groupBox_2);
        buttonGroup->addButton(scalingXYAxisButton);
        scalingXYAxisButton->setObjectName(QString::fromUtf8("scalingXYAxisButton"));
        scalingXYAxisButton->setEnabled(true);
        scalingXYAxisButton->setGeometry(QRect(120, 60, 100, 21));
        scalingXYAxisButton->setCheckable(true);
        scalingXYAxisButton->setAutoExclusive(true);
        RotateXAxisButton = new QToolButton(groupBox_2);
        buttonGroup->addButton(RotateXAxisButton);
        RotateXAxisButton->setObjectName(QString::fromUtf8("RotateXAxisButton"));
        RotateXAxisButton->setEnabled(true);
        RotateXAxisButton->setGeometry(QRect(10, 100, 100, 21));
        RotateXAxisButton->setCheckable(true);
        RotateXAxisButton->setAutoExclusive(true);
        RotateYAxisButton = new QToolButton(groupBox_2);
        buttonGroup->addButton(RotateYAxisButton);
        RotateYAxisButton->setObjectName(QString::fromUtf8("RotateYAxisButton"));
        RotateYAxisButton->setEnabled(true);
        RotateYAxisButton->setGeometry(QRect(10, 120, 100, 21));
        RotateYAxisButton->setCheckable(true);
        RotateYAxisButton->setAutoExclusive(true);
        checkBoxPositionSave = new QCheckBox(groupBox_2);
        checkBoxPositionSave->setObjectName(QString::fromUtf8("checkBoxPositionSave"));
        checkBoxPositionSave->setGeometry(QRect(10, 16, 375, 18));
        checkBoxMultiSelect = new QCheckBox(groupBox_2);
        checkBoxMultiSelect->setObjectName(QString::fromUtf8("checkBoxMultiSelect"));
        checkBoxMultiSelect->setGeometry(QRect(210, 16, 375, 18));
        groupBox_3 = new QGroupBox(viewTab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 292, 351, 141));
        checkBoxFog = new QCheckBox(groupBox_3);
        checkBoxFog->setObjectName(QString::fromUtf8("checkBoxFog"));
        checkBoxFog->setGeometry(QRect(200, 118, 81, 18));
        pushButtonDiffuseColor = new QPushButton(groupBox_3);
        pushButtonDiffuseColor->setObjectName(QString::fromUtf8("pushButtonDiffuseColor"));
        pushButtonDiffuseColor->setGeometry(QRect(100, 40, 50, 25));
        pushButtonEmissiveColor = new QPushButton(groupBox_3);
        pushButtonEmissiveColor->setObjectName(QString::fromUtf8("pushButtonEmissiveColor"));
        pushButtonEmissiveColor->setGeometry(QRect(290, 67, 50, 25));
        materialComboBox = new QComboBox(groupBox_3);
        materialComboBox->setObjectName(QString::fromUtf8("materialComboBox"));
        materialComboBox->setEnabled(true);
        materialComboBox->setGeometry(QRect(180, 14, 161, 20));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 67, 81, 25));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 40, 81, 25));
        horizontalSliderShiness = new QSlider(groupBox_3);
        horizontalSliderShiness->setObjectName(QString::fromUtf8("horizontalSliderShiness"));
        horizontalSliderShiness->setGeometry(QRect(100, 98, 241, 16));
        horizontalSliderShiness->setMaximum(128);
        horizontalSliderShiness->setOrientation(Qt::Horizontal);
        pushButtonSpecularColor = new QPushButton(groupBox_3);
        pushButtonSpecularColor->setObjectName(QString::fromUtf8("pushButtonSpecularColor"));
        pushButtonSpecularColor->setGeometry(QRect(100, 67, 50, 25));
        checkBoxLighting = new QCheckBox(groupBox_3);
        checkBoxLighting->setObjectName(QString::fromUtf8("checkBoxLighting"));
        checkBoxLighting->setGeometry(QRect(10, 118, 151, 18));
        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 93, 71, 25));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(190, 67, 81, 25));
        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(13, 15, 162, 20));
        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(190, 40, 91, 25));
        pushButtonAmbientColor = new QPushButton(groupBox_3);
        pushButtonAmbientColor->setObjectName(QString::fromUtf8("pushButtonAmbientColor"));
        pushButtonAmbientColor->setGeometry(QRect(290, 40, 50, 25));
        tabWidget->addTab(viewTab, QString());
        buildingPropertyTab = new QWidget();
        buildingPropertyTab->setObjectName(QString::fromUtf8("buildingPropertyTab"));
        label_2 = new QLabel(buildingPropertyTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 10, 81, 16));
        lineEditBuildingName = new QLineEdit(buildingPropertyTab);
        lineEditBuildingName->setObjectName(QString::fromUtf8("lineEditBuildingName"));
        lineEditBuildingName->setGeometry(QRect(80, 10, 281, 20));
        lineEditPostName = new QLineEdit(buildingPropertyTab);
        lineEditPostName->setObjectName(QString::fromUtf8("lineEditPostName"));
        lineEditPostName->setGeometry(QRect(80, 40, 281, 20));
        label_3 = new QLabel(buildingPropertyTab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 38, 81, 16));
        spinBoxFloor = new QSpinBox(buildingPropertyTab);
        spinBoxFloor->setObjectName(QString::fromUtf8("spinBoxFloor"));
        spinBoxFloor->setGeometry(QRect(83, 130, 81, 20));
        spinBoxFloor->setMinimum(1);
        spinBoxFloor->setMaximum(100);
        label_11 = new QLabel(buildingPropertyTab);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(11, 130, 71, 16));
        groupBox = new QGroupBox(buildingPropertyTab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(8, 153, 351, 271));
        horizontalLayout_15 = new QHBoxLayout(groupBox);
        horizontalLayout_15->setSpacing(5);
        horizontalLayout_15->setContentsMargins(5, 5, 5, 5);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        textEditMisc = new QTextEdit(groupBox);
        textEditMisc->setObjectName(QString::fromUtf8("textEditMisc"));

        horizontalLayout_15->addWidget(textEditMisc);

        label_6 = new QLabel(buildingPropertyTab);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 70, 81, 16));
        comboBoxProvince = new QComboBox(buildingPropertyTab);
        comboBoxProvince->setObjectName(QString::fromUtf8("comboBoxProvince"));
        comboBoxProvince->setGeometry(QRect(82, 70, 81, 20));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboBoxProvince->sizePolicy().hasHeightForWidth());
        comboBoxProvince->setSizePolicy(sizePolicy);
        label_7 = new QLabel(buildingPropertyTab);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(180, 70, 51, 16));
        comboBoxCounty = new QComboBox(buildingPropertyTab);
        comboBoxCounty->setObjectName(QString::fromUtf8("comboBoxCounty"));
        comboBoxCounty->setGeometry(QRect(251, 70, 111, 20));
        sizePolicy.setHeightForWidth(comboBoxCounty->sizePolicy().hasHeightForWidth());
        comboBoxCounty->setSizePolicy(sizePolicy);
        label_10 = new QLabel(buildingPropertyTab);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(180, 100, 51, 16));
        lineEditBan = new QLineEdit(buildingPropertyTab);
        lineEditBan->setObjectName(QString::fromUtf8("lineEditBan"));
        lineEditBan->setGeometry(QRect(267, 100, 88, 20));
        label_9 = new QLabel(buildingPropertyTab);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(11, 101, 81, 16));
        comboBoxVilliage = new QComboBox(buildingPropertyTab);
        comboBoxVilliage->setObjectName(QString::fromUtf8("comboBoxVilliage"));
        comboBoxVilliage->setGeometry(QRect(83, 100, 81, 20));
        sizePolicy.setHeightForWidth(comboBoxVilliage->sizePolicy().hasHeightForWidth());
        comboBoxVilliage->setSizePolicy(sizePolicy);
        tabWidget->addTab(buildingPropertyTab, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonDelete = new QPushButton(ObjectAddDlg);
        pushButtonDelete->setObjectName(QString::fromUtf8("pushButtonDelete"));
        pushButtonDelete->setEnabled(true);

        horizontalLayout->addWidget(pushButtonDelete);

        saveButton = new QPushButton(ObjectAddDlg);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setEnabled(true);
        saveButton->setAutoDefault(false);

        horizontalLayout->addWidget(saveButton);

        closeButton = new QPushButton(ObjectAddDlg);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setAutoDefault(false);

        horizontalLayout->addWidget(closeButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ObjectAddDlg);

        tabWidget->setCurrentIndex(1);
        saveButton->setDefault(true);
        closeButton->setDefault(true);


        QMetaObject::connectSlotsByName(ObjectAddDlg);
    } // setupUi

    void retranslateUi(QDialog *ObjectAddDlg)
    {
        ObjectAddDlg->setWindowTitle(QCoreApplication::translate("ObjectAddDlg", "ObjectAddDlg", nullptr));
        openButton->setText(QCoreApplication::translate("ObjectAddDlg", "Open...", nullptr));
#if QT_CONFIG(shortcut)
        openButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        label_12->setText(QCoreApplication::translate("ObjectAddDlg", "Object Type", nullptr));
#if QT_CONFIG(whatsthis)
        tabWidget->setWhatsThis(QCoreApplication::translate("ObjectAddDlg", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        groupBox_2->setTitle(QCoreApplication::translate("ObjectAddDlg", "Geometry", nullptr));
        fillColorLabel->setText(QCoreApplication::translate("ObjectAddDlg", "Length", nullptr));
        setDirectionButton->setText(QCoreApplication::translate("ObjectAddDlg", "Adjust Azimuth", nullptr));
#if QT_CONFIG(shortcut)
        setDirectionButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "A", nullptr));
#endif // QT_CONFIG(shortcut)
        Rotate90AroundXButton->setText(QCoreApplication::translate("ObjectAddDlg", "Rotate 90  X Axis ", nullptr));
#if QT_CONFIG(shortcut)
        Rotate90AroundXButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "Ctrl+F", nullptr));
#endif // QT_CONFIG(shortcut)
        lengthDoubleSpinBox->setSuffix(QCoreApplication::translate("ObjectAddDlg", "m", nullptr));
        latitudeLabel->setText(QCoreApplication::translate("ObjectAddDlg", "Latitude:", nullptr));
        backgroundMoveButton->setText(QCoreApplication::translate("ObjectAddDlg", "Background Move", nullptr));
#if QT_CONFIG(shortcut)
        backgroundMoveButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "C", nullptr));
#endif // QT_CONFIG(shortcut)
        heightDoubleSpinBox1->setSuffix(QCoreApplication::translate("ObjectAddDlg", "m", nullptr));
        scalingXAxisButton->setText(QCoreApplication::translate("ObjectAddDlg", "Scaling X Axis", nullptr));
#if QT_CONFIG(shortcut)
        scalingXAxisButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "X", nullptr));
#endif // QT_CONFIG(shortcut)
        scalingYAxisButton->setText(QCoreApplication::translate("ObjectAddDlg", "Scaling Y Axis", nullptr));
#if QT_CONFIG(shortcut)
        scalingYAxisButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "Y", nullptr));
#endif // QT_CONFIG(shortcut)
        fillColorLabel_3->setText(QCoreApplication::translate("ObjectAddDlg", "Width", nullptr));
        scalingAllAxisButton->setText(QCoreApplication::translate("ObjectAddDlg", "Scaling All Axiss", nullptr));
#if QT_CONFIG(shortcut)
        scalingAllAxisButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "S", nullptr));
#endif // QT_CONFIG(shortcut)
        selectButton->setText(QCoreApplication::translate("ObjectAddDlg", "Select", nullptr));
#if QT_CONFIG(shortcut)
        selectButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "Ctrl+Shift+S", nullptr));
#endif // QT_CONFIG(shortcut)
        setHeightButton->setText(QCoreApplication::translate("ObjectAddDlg", "Adjust Altitude", nullptr));
#if QT_CONFIG(shortcut)
        setHeightButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "H", nullptr));
#endif // QT_CONFIG(shortcut)
        rangeLabel->setText(QCoreApplication::translate("ObjectAddDlg", "Altitude", nullptr));
        flipButton->setText(QCoreApplication::translate("ObjectAddDlg", "Flip", nullptr));
#if QT_CONFIG(shortcut)
        flipButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "Ctrl+F", nullptr));
#endif // QT_CONFIG(shortcut)
        widthDoubleSpinBox->setSuffix(QCoreApplication::translate("ObjectAddDlg", "m", nullptr));
        scalingZAxisButton->setText(QCoreApplication::translate("ObjectAddDlg", "Scaling Z Axis", nullptr));
#if QT_CONFIG(shortcut)
        scalingZAxisButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "Z", nullptr));
#endif // QT_CONFIG(shortcut)
        longitudeLabel->setText(QCoreApplication::translate("ObjectAddDlg", "Longitude:", nullptr));
        rangeLabel_2->setText(QCoreApplication::translate("ObjectAddDlg", "Azimuth", nullptr));
        fillColorLabel_4->setText(QCoreApplication::translate("ObjectAddDlg", "Height", nullptr));
        Rotate90AroundYButton->setText(QCoreApplication::translate("ObjectAddDlg", "Rotate 90 Y Axis", nullptr));
        setPositionButton->setText(QCoreApplication::translate("ObjectAddDlg", "Set Position", nullptr));
#if QT_CONFIG(shortcut)
        setPositionButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "P", nullptr));
#endif // QT_CONFIG(shortcut)
        InvertXAxisButton->setText(QCoreApplication::translate("ObjectAddDlg", "Invert XAxis", nullptr));
#if QT_CONFIG(shortcut)
        InvertXAxisButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "Ctrl+F", nullptr));
#endif // QT_CONFIG(shortcut)
        scalingXYAxisButton->setText(QCoreApplication::translate("ObjectAddDlg", "Scaling XY Axis", nullptr));
#if QT_CONFIG(shortcut)
        scalingXYAxisButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "Y", nullptr));
#endif // QT_CONFIG(shortcut)
        RotateXAxisButton->setText(QCoreApplication::translate("ObjectAddDlg", "Rotate X Axis", nullptr));
#if QT_CONFIG(shortcut)
        RotateXAxisButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "X", nullptr));
#endif // QT_CONFIG(shortcut)
        RotateYAxisButton->setText(QCoreApplication::translate("ObjectAddDlg", "Rotate Y Axis", nullptr));
#if QT_CONFIG(shortcut)
        RotateYAxisButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "X", nullptr));
#endif // QT_CONFIG(shortcut)
        checkBoxPositionSave->setText(QCoreApplication::translate("ObjectAddDlg", "Set Position And Immediately Save", nullptr));
        checkBoxMultiSelect->setText(QCoreApplication::translate("ObjectAddDlg", "Multi Select", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("ObjectAddDlg", "Material", nullptr));
        checkBoxFog->setText(QCoreApplication::translate("ObjectAddDlg", "Enable Fog", nullptr));
        pushButtonDiffuseColor->setText(QString());
        pushButtonEmissiveColor->setText(QString());
        label_5->setText(QCoreApplication::translate("ObjectAddDlg", "Ambient Color", nullptr));
        label_4->setText(QCoreApplication::translate("ObjectAddDlg", "Diffuse Color", nullptr));
        pushButtonSpecularColor->setText(QString());
        checkBoxLighting->setText(QCoreApplication::translate("ObjectAddDlg", "Enable Lighting", nullptr));
        label_14->setText(QCoreApplication::translate("ObjectAddDlg", "Shiness", nullptr));
        label_8->setText(QCoreApplication::translate("ObjectAddDlg", "Specular Color", nullptr));
        label->setText(QCoreApplication::translate("ObjectAddDlg", "Material Type", nullptr));
        label_13->setText(QCoreApplication::translate("ObjectAddDlg", "Emissive Color", nullptr));
        pushButtonAmbientColor->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(viewTab), QCoreApplication::translate("ObjectAddDlg", "Geometry", nullptr));
        label_2->setText(QCoreApplication::translate("ObjectAddDlg", "BuildingName", nullptr));
        label_3->setText(QCoreApplication::translate("ObjectAddDlg", "PostName     ", nullptr));
        spinBoxFloor->setSuffix(QString());
        label_11->setText(QCoreApplication::translate("ObjectAddDlg", "Floor", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ObjectAddDlg", "Misc", nullptr));
        label_6->setText(QCoreApplication::translate("ObjectAddDlg", "Province", nullptr));
        label_7->setText(QCoreApplication::translate("ObjectAddDlg", "County", nullptr));
        label_10->setText(QCoreApplication::translate("ObjectAddDlg", "Ban      ", nullptr));
        label_9->setText(QCoreApplication::translate("ObjectAddDlg", "Villiage   ", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(buildingPropertyTab), QCoreApplication::translate("ObjectAddDlg", "Building Property", nullptr));
        pushButtonDelete->setText(QCoreApplication::translate("ObjectAddDlg", "Delete", nullptr));
        saveButton->setText(QCoreApplication::translate("ObjectAddDlg", "Save", nullptr));
#if QT_CONFIG(shortcut)
        saveButton->setShortcut(QCoreApplication::translate("ObjectAddDlg", "Ctrl+S", "qqq"));
#endif // QT_CONFIG(shortcut)
        closeButton->setText(QCoreApplication::translate("ObjectAddDlg", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ObjectAddDlg: public Ui_ObjectAddDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBJECTADDDLG_H
