/********************************************************************************
** Form generated from reading UI file 'pathdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATHDLG_H
#define UI_PATHDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PathDlg
{
public:
    QLabel *name;
    QLineEdit *nameEdit;
    QTabWidget *pathPropertyTab;
    QWidget *descriptionTab;
    QTextEdit *descriptionEdit;
    QWidget *viewTab;
    QLineEdit *headingLineEdit;
    QLabel *longitudeLabel;
    QLabel *headingLabel;
    QLineEdit *longitudeLineEdit;
    QLineEdit *rangeLineEdit;
    QLineEdit *latitudeLineEdit;
    QLabel *rangeLabel;
    QLabel *latitudeLabel;
    QLabel *tiltLabel;
    QLineEdit *tiltLineEdit;
    QPushButton *snapshotViewButton;
    QPushButton *resetButton;
    QWidget *shapePropertyTab;
    QGroupBox *heightGroupBox;
    QComboBox *heightModeCombo;
    QLabel *heightLabel;
    QLineEdit *heightLineEdit;
    QSlider *lineHeightlSlider;
    QCheckBox *extendCheckBox;
    QGroupBox *pointListGroupBox;
    QTableView *pListTableView;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *editButton;
    QGroupBox *fillGroupBox;
    QDoubleSpinBox *fillOpacitySpin;
    QLabel *fillColorLabel;
    QPushButton *fillColorButton;
    QLabel *fillOpacityLabel;
    QGroupBox *lineGroupBox;
    QDoubleSpinBox *lineOpacitySpin;
    QDoubleSpinBox *lineThickSpin;
    QLabel *lineColorLabel;
    QPushButton *lineColorButton;
    QLabel *lineOpacityLabel;
    QLabel *lineThickLabel;
    QWidget *gisPropertyTab;
    QLabel *lengthLabel;
    QGroupBox *calcGroupBox;
    QLabel *calcLengthLabel;
    QPushButton *calcPushButton;
    QLabel *calcLengthValueLabel;
    QGroupBox *maxHeightGroupBox;
    QLabel *maxHeightLongiValueLabel;
    QLabel *maxHeightValueLabel;
    QLabel *maxHeightLabel;
    QLabel *maxHeightLatiValueLabel;
    QLabel *maxHeightLatiLabel;
    QLabel *maxHeightLongiLabel;
    QGroupBox *minHeightGroupBox;
    QLabel *minHeightLongiValueLabel;
    QLabel *minHeightLabel;
    QLabel *minHeightLongiLabel;
    QLabel *minHeightLatiValueLabel;
    QLabel *minHeightValueLabel;
    QLabel *minHeightLatiLabel;
    QGroupBox *groupBox;
    QLabel *maxRateLongiValueLabel;
    QLabel *maxRateAngleLabel;
    QLabel *maxRateValueLabel;
    QLabel *maxRateLatiLabel;
    QLabel *maxRateLongiLabel;
    QLabel *maxRateLatiValueLabel;
    QLabel *lengthValueLabel;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *PathDlg)
    {
        if (PathDlg->objectName().isEmpty())
            PathDlg->setObjectName(QString::fromUtf8("PathDlg"));
        PathDlg->resize(400, 624);
        name = new QLabel(PathDlg);
        name->setObjectName(QString::fromUtf8("name"));
        name->setGeometry(QRect(10, 10, 40, 20));
        nameEdit = new QLineEdit(PathDlg);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));
        nameEdit->setGeometry(QRect(60, 10, 331, 20));
        pathPropertyTab = new QTabWidget(PathDlg);
        pathPropertyTab->setObjectName(QString::fromUtf8("pathPropertyTab"));
        pathPropertyTab->setGeometry(QRect(10, 50, 380, 530));
        descriptionTab = new QWidget();
        descriptionTab->setObjectName(QString::fromUtf8("descriptionTab"));
        descriptionEdit = new QTextEdit(descriptionTab);
        descriptionEdit->setObjectName(QString::fromUtf8("descriptionEdit"));
        descriptionEdit->setGeometry(QRect(10, 10, 355, 485));
        pathPropertyTab->addTab(descriptionTab, QString());
        viewTab = new QWidget();
        viewTab->setObjectName(QString::fromUtf8("viewTab"));
        headingLineEdit = new QLineEdit(viewTab);
        headingLineEdit->setObjectName(QString::fromUtf8("headingLineEdit"));
        headingLineEdit->setGeometry(QRect(160, 140, 160, 20));
        headingLineEdit->setDragEnabled(false);
        headingLineEdit->setReadOnly(false);
        longitudeLabel = new QLabel(viewTab);
        longitudeLabel->setObjectName(QString::fromUtf8("longitudeLabel"));
        longitudeLabel->setGeometry(QRect(60, 80, 90, 20));
        headingLabel = new QLabel(viewTab);
        headingLabel->setObjectName(QString::fromUtf8("headingLabel"));
        headingLabel->setGeometry(QRect(60, 140, 90, 20));
        longitudeLineEdit = new QLineEdit(viewTab);
        longitudeLineEdit->setObjectName(QString::fromUtf8("longitudeLineEdit"));
        longitudeLineEdit->setGeometry(QRect(160, 80, 160, 20));
        longitudeLineEdit->setReadOnly(false);
        rangeLineEdit = new QLineEdit(viewTab);
        rangeLineEdit->setObjectName(QString::fromUtf8("rangeLineEdit"));
        rangeLineEdit->setGeometry(QRect(160, 110, 160, 20));
        rangeLineEdit->setReadOnly(false);
        latitudeLineEdit = new QLineEdit(viewTab);
        latitudeLineEdit->setObjectName(QString::fromUtf8("latitudeLineEdit"));
        latitudeLineEdit->setEnabled(true);
        latitudeLineEdit->setGeometry(QRect(160, 50, 160, 20));
        latitudeLineEdit->setReadOnly(false);
        rangeLabel = new QLabel(viewTab);
        rangeLabel->setObjectName(QString::fromUtf8("rangeLabel"));
        rangeLabel->setGeometry(QRect(60, 110, 90, 20));
        latitudeLabel = new QLabel(viewTab);
        latitudeLabel->setObjectName(QString::fromUtf8("latitudeLabel"));
        latitudeLabel->setGeometry(QRect(60, 50, 90, 20));
        tiltLabel = new QLabel(viewTab);
        tiltLabel->setObjectName(QString::fromUtf8("tiltLabel"));
        tiltLabel->setGeometry(QRect(60, 170, 90, 20));
        tiltLineEdit = new QLineEdit(viewTab);
        tiltLineEdit->setObjectName(QString::fromUtf8("tiltLineEdit"));
        tiltLineEdit->setGeometry(QRect(160, 170, 160, 20));
        tiltLineEdit->setReadOnly(false);
        snapshotViewButton = new QPushButton(viewTab);
        snapshotViewButton->setObjectName(QString::fromUtf8("snapshotViewButton"));
        snapshotViewButton->setGeometry(QRect(130, 220, 150, 24));
        snapshotViewButton->setFocusPolicy(Qt::StrongFocus);
        snapshotViewButton->setAutoDefault(false);
        resetButton = new QPushButton(viewTab);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));
        resetButton->setGeometry(QRect(290, 220, 75, 24));
        resetButton->setFocusPolicy(Qt::StrongFocus);
        resetButton->setAutoDefault(false);
        pathPropertyTab->addTab(viewTab, QString());
        shapePropertyTab = new QWidget();
        shapePropertyTab->setObjectName(QString::fromUtf8("shapePropertyTab"));
        shapePropertyTab->setFocusPolicy(Qt::StrongFocus);
        heightGroupBox = new QGroupBox(shapePropertyTab);
        heightGroupBox->setObjectName(QString::fromUtf8("heightGroupBox"));
        heightGroupBox->setGeometry(QRect(10, 160, 360, 130));
        heightModeCombo = new QComboBox(heightGroupBox);
        heightModeCombo->setObjectName(QString::fromUtf8("heightModeCombo"));
        heightModeCombo->setGeometry(QRect(10, 30, 150, 22));
        heightLabel = new QLabel(heightGroupBox);
        heightLabel->setObjectName(QString::fromUtf8("heightLabel"));
        heightLabel->setGeometry(QRect(180, 30, 50, 22));
        heightLineEdit = new QLineEdit(heightGroupBox);
        heightLineEdit->setObjectName(QString::fromUtf8("heightLineEdit"));
        heightLineEdit->setGeometry(QRect(230, 30, 70, 22));
        heightLineEdit->setMaxLength(32767);
        heightLineEdit->setCursorPosition(0);
        heightLineEdit->setDragEnabled(false);
        heightLineEdit->setReadOnly(false);
        lineHeightlSlider = new QSlider(heightGroupBox);
        lineHeightlSlider->setObjectName(QString::fromUtf8("lineHeightlSlider"));
        lineHeightlSlider->setGeometry(QRect(10, 70, 340, 21));
        lineHeightlSlider->setMaximum(512);
        lineHeightlSlider->setSingleStep(1);
        lineHeightlSlider->setPageStep(32);
        lineHeightlSlider->setOrientation(Qt::Horizontal);
        lineHeightlSlider->setInvertedAppearance(false);
        lineHeightlSlider->setInvertedControls(false);
        lineHeightlSlider->setTickPosition(QSlider::NoTicks);
        lineHeightlSlider->setTickInterval(0);
        extendCheckBox = new QCheckBox(heightGroupBox);
        extendCheckBox->setObjectName(QString::fromUtf8("extendCheckBox"));
        extendCheckBox->setGeometry(QRect(10, 100, 241, 18));
        pointListGroupBox = new QGroupBox(shapePropertyTab);
        pointListGroupBox->setObjectName(QString::fromUtf8("pointListGroupBox"));
        pointListGroupBox->setGeometry(QRect(10, 300, 360, 200));
        pListTableView = new QTableView(pointListGroupBox);
        pListTableView->setObjectName(QString::fromUtf8("pListTableView"));
        pListTableView->setGeometry(QRect(10, 20, 340, 136));
        pListTableView->setSelectionMode(QAbstractItemView::SingleSelection);
        pListTableView->verticalHeader()->setDefaultSectionSize(18);
        addButton = new QPushButton(pointListGroupBox);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        addButton->setGeometry(QRect(10, 166, 75, 24));
        addButton->setFocusPolicy(Qt::StrongFocus);
        addButton->setAutoDefault(false);
        deleteButton = new QPushButton(pointListGroupBox);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));
        deleteButton->setGeometry(QRect(275, 166, 75, 24));
        deleteButton->setFocusPolicy(Qt::StrongFocus);
        deleteButton->setAutoDefault(false);
        editButton = new QPushButton(pointListGroupBox);
        editButton->setObjectName(QString::fromUtf8("editButton"));
        editButton->setGeometry(QRect(142, 166, 75, 24));
        editButton->setFocusPolicy(Qt::StrongFocus);
        editButton->setAutoDefault(false);
        fillGroupBox = new QGroupBox(shapePropertyTab);
        fillGroupBox->setObjectName(QString::fromUtf8("fillGroupBox"));
        fillGroupBox->setGeometry(QRect(10, 90, 360, 60));
        fillGroupBox->setCheckable(true);
        fillOpacitySpin = new QDoubleSpinBox(fillGroupBox);
        fillOpacitySpin->setObjectName(QString::fromUtf8("fillOpacitySpin"));
        fillOpacitySpin->setGeometry(QRect(150, 25, 60, 23));
        fillOpacitySpin->setDecimals(0);
        fillOpacitySpin->setMinimum(0.000000000000000);
        fillOpacitySpin->setSingleStep(1.000000000000000);
        fillOpacitySpin->setValue(50.000000000000000);
        fillColorLabel = new QLabel(fillGroupBox);
        fillColorLabel->setObjectName(QString::fromUtf8("fillColorLabel"));
        fillColorLabel->setGeometry(QRect(10, 25, 50, 23));
        fillColorButton = new QPushButton(fillGroupBox);
        fillColorButton->setObjectName(QString::fromUtf8("fillColorButton"));
        fillColorButton->setGeometry(QRect(40, 25, 30, 23));
        fillColorButton->setFocusPolicy(Qt::StrongFocus);
        fillColorButton->setAutoDefault(false);
        fillOpacityLabel = new QLabel(fillGroupBox);
        fillOpacityLabel->setObjectName(QString::fromUtf8("fillOpacityLabel"));
        fillOpacityLabel->setGeometry(QRect(90, 25, 60, 23));
        lineGroupBox = new QGroupBox(shapePropertyTab);
        lineGroupBox->setObjectName(QString::fromUtf8("lineGroupBox"));
        lineGroupBox->setGeometry(QRect(10, 20, 360, 60));
        lineGroupBox->setCheckable(true);
        lineOpacitySpin = new QDoubleSpinBox(lineGroupBox);
        lineOpacitySpin->setObjectName(QString::fromUtf8("lineOpacitySpin"));
        lineOpacitySpin->setGeometry(QRect(270, 25, 60, 23));
        lineOpacitySpin->setDecimals(0);
        lineOpacitySpin->setMinimum(0.000000000000000);
        lineOpacitySpin->setSingleStep(1.000000000000000);
        lineOpacitySpin->setValue(100.000000000000000);
        lineThickSpin = new QDoubleSpinBox(lineGroupBox);
        lineThickSpin->setObjectName(QString::fromUtf8("lineThickSpin"));
        lineThickSpin->setGeometry(QRect(130, 25, 60, 23));
        lineThickSpin->setDecimals(1);
        lineThickSpin->setMaximum(100.000000000000000);
        lineThickSpin->setSingleStep(0.100000000000000);
        lineColorLabel = new QLabel(lineGroupBox);
        lineColorLabel->setObjectName(QString::fromUtf8("lineColorLabel"));
        lineColorLabel->setGeometry(QRect(10, 25, 50, 23));
        lineColorButton = new QPushButton(lineGroupBox);
        lineColorButton->setObjectName(QString::fromUtf8("lineColorButton"));
        lineColorButton->setGeometry(QRect(40, 25, 30, 23));
        lineColorButton->setFocusPolicy(Qt::StrongFocus);
        lineColorButton->setAutoDefault(false);
        lineOpacityLabel = new QLabel(lineGroupBox);
        lineOpacityLabel->setObjectName(QString::fromUtf8("lineOpacityLabel"));
        lineOpacityLabel->setGeometry(QRect(210, 25, 60, 23));
        lineThickLabel = new QLabel(lineGroupBox);
        lineThickLabel->setObjectName(QString::fromUtf8("lineThickLabel"));
        lineThickLabel->setGeometry(QRect(90, 25, 50, 23));
        pathPropertyTab->addTab(shapePropertyTab, QString());
        gisPropertyTab = new QWidget();
        gisPropertyTab->setObjectName(QString::fromUtf8("gisPropertyTab"));
        lengthLabel = new QLabel(gisPropertyTab);
        lengthLabel->setObjectName(QString::fromUtf8("lengthLabel"));
        lengthLabel->setGeometry(QRect(10, 20, 60, 20));
        calcGroupBox = new QGroupBox(gisPropertyTab);
        calcGroupBox->setObjectName(QString::fromUtf8("calcGroupBox"));
        calcGroupBox->setGeometry(QRect(10, 60, 355, 351));
        calcGroupBox->setCheckable(true);
        calcLengthLabel = new QLabel(calcGroupBox);
        calcLengthLabel->setObjectName(QString::fromUtf8("calcLengthLabel"));
        calcLengthLabel->setGeometry(QRect(40, 30, 110, 20));
        calcPushButton = new QPushButton(calcGroupBox);
        calcPushButton->setObjectName(QString::fromUtf8("calcPushButton"));
        calcPushButton->setGeometry(QRect(120, 310, 120, 24));
        calcPushButton->setAutoDefault(false);
        calcLengthValueLabel = new QLabel(calcGroupBox);
        calcLengthValueLabel->setObjectName(QString::fromUtf8("calcLengthValueLabel"));
        calcLengthValueLabel->setGeometry(QRect(130, 30, 180, 20));
        calcLengthValueLabel->setFrameShape(QFrame::Panel);
        calcLengthValueLabel->setFrameShadow(QFrame::Sunken);
        maxHeightGroupBox = new QGroupBox(calcGroupBox);
        maxHeightGroupBox->setObjectName(QString::fromUtf8("maxHeightGroupBox"));
        maxHeightGroupBox->setGeometry(QRect(20, 70, 310, 70));
        maxHeightGroupBox->setFlat(true);
        maxHeightLongiValueLabel = new QLabel(maxHeightGroupBox);
        maxHeightLongiValueLabel->setObjectName(QString::fromUtf8("maxHeightLongiValueLabel"));
        maxHeightLongiValueLabel->setGeometry(QRect(220, 40, 71, 20));
        maxHeightLongiValueLabel->setFrameShape(QFrame::Panel);
        maxHeightLongiValueLabel->setFrameShadow(QFrame::Sunken);
        maxHeightValueLabel = new QLabel(maxHeightGroupBox);
        maxHeightValueLabel->setObjectName(QString::fromUtf8("maxHeightValueLabel"));
        maxHeightValueLabel->setGeometry(QRect(110, 10, 180, 20));
        maxHeightValueLabel->setFrameShape(QFrame::Panel);
        maxHeightValueLabel->setFrameShadow(QFrame::Sunken);
        maxHeightLabel = new QLabel(maxHeightGroupBox);
        maxHeightLabel->setObjectName(QString::fromUtf8("maxHeightLabel"));
        maxHeightLabel->setGeometry(QRect(20, 10, 91, 20));
        maxHeightLatiValueLabel = new QLabel(maxHeightGroupBox);
        maxHeightLatiValueLabel->setObjectName(QString::fromUtf8("maxHeightLatiValueLabel"));
        maxHeightLatiValueLabel->setGeometry(QRect(70, 40, 71, 20));
        maxHeightLatiValueLabel->setFrameShape(QFrame::Panel);
        maxHeightLatiValueLabel->setFrameShadow(QFrame::Sunken);
        maxHeightLatiLabel = new QLabel(maxHeightGroupBox);
        maxHeightLatiLabel->setObjectName(QString::fromUtf8("maxHeightLatiLabel"));
        maxHeightLatiLabel->setGeometry(QRect(20, 40, 71, 20));
        maxHeightLongiLabel = new QLabel(maxHeightGroupBox);
        maxHeightLongiLabel->setObjectName(QString::fromUtf8("maxHeightLongiLabel"));
        maxHeightLongiLabel->setGeometry(QRect(170, 40, 71, 20));
        minHeightGroupBox = new QGroupBox(calcGroupBox);
        minHeightGroupBox->setObjectName(QString::fromUtf8("minHeightGroupBox"));
        minHeightGroupBox->setGeometry(QRect(20, 150, 310, 70));
        minHeightGroupBox->setFlat(true);
        minHeightLongiValueLabel = new QLabel(minHeightGroupBox);
        minHeightLongiValueLabel->setObjectName(QString::fromUtf8("minHeightLongiValueLabel"));
        minHeightLongiValueLabel->setGeometry(QRect(220, 40, 71, 20));
        minHeightLongiValueLabel->setFrameShape(QFrame::Panel);
        minHeightLongiValueLabel->setFrameShadow(QFrame::Sunken);
        minHeightLabel = new QLabel(minHeightGroupBox);
        minHeightLabel->setObjectName(QString::fromUtf8("minHeightLabel"));
        minHeightLabel->setGeometry(QRect(20, 10, 110, 20));
        minHeightLongiLabel = new QLabel(minHeightGroupBox);
        minHeightLongiLabel->setObjectName(QString::fromUtf8("minHeightLongiLabel"));
        minHeightLongiLabel->setGeometry(QRect(170, 40, 61, 20));
        minHeightLatiValueLabel = new QLabel(minHeightGroupBox);
        minHeightLatiValueLabel->setObjectName(QString::fromUtf8("minHeightLatiValueLabel"));
        minHeightLatiValueLabel->setGeometry(QRect(70, 40, 71, 20));
        minHeightLatiValueLabel->setFrameShape(QFrame::Panel);
        minHeightLatiValueLabel->setFrameShadow(QFrame::Sunken);
        minHeightValueLabel = new QLabel(minHeightGroupBox);
        minHeightValueLabel->setObjectName(QString::fromUtf8("minHeightValueLabel"));
        minHeightValueLabel->setGeometry(QRect(110, 10, 180, 20));
        minHeightValueLabel->setFrameShape(QFrame::Panel);
        minHeightValueLabel->setFrameShadow(QFrame::Sunken);
        minHeightLatiLabel = new QLabel(minHeightGroupBox);
        minHeightLatiLabel->setObjectName(QString::fromUtf8("minHeightLatiLabel"));
        minHeightLatiLabel->setGeometry(QRect(20, 40, 71, 20));
        groupBox = new QGroupBox(calcGroupBox);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 230, 310, 70));
        groupBox->setFlat(true);
        maxRateLongiValueLabel = new QLabel(groupBox);
        maxRateLongiValueLabel->setObjectName(QString::fromUtf8("maxRateLongiValueLabel"));
        maxRateLongiValueLabel->setGeometry(QRect(220, 40, 71, 20));
        maxRateLongiValueLabel->setFrameShape(QFrame::Panel);
        maxRateLongiValueLabel->setFrameShadow(QFrame::Sunken);
        maxRateAngleLabel = new QLabel(groupBox);
        maxRateAngleLabel->setObjectName(QString::fromUtf8("maxRateAngleLabel"));
        maxRateAngleLabel->setGeometry(QRect(20, 10, 110, 20));
        maxRateValueLabel = new QLabel(groupBox);
        maxRateValueLabel->setObjectName(QString::fromUtf8("maxRateValueLabel"));
        maxRateValueLabel->setGeometry(QRect(110, 10, 180, 20));
        maxRateValueLabel->setFrameShape(QFrame::Panel);
        maxRateValueLabel->setFrameShadow(QFrame::Sunken);
        maxRateLatiLabel = new QLabel(groupBox);
        maxRateLatiLabel->setObjectName(QString::fromUtf8("maxRateLatiLabel"));
        maxRateLatiLabel->setGeometry(QRect(20, 40, 71, 20));
        maxRateLongiLabel = new QLabel(groupBox);
        maxRateLongiLabel->setObjectName(QString::fromUtf8("maxRateLongiLabel"));
        maxRateLongiLabel->setGeometry(QRect(170, 40, 71, 20));
        maxRateLatiValueLabel = new QLabel(groupBox);
        maxRateLatiValueLabel->setObjectName(QString::fromUtf8("maxRateLatiValueLabel"));
        maxRateLatiValueLabel->setGeometry(QRect(70, 40, 71, 20));
        maxRateLatiValueLabel->setFrameShape(QFrame::Panel);
        maxRateLatiValueLabel->setFrameShadow(QFrame::Sunken);
        lengthValueLabel = new QLabel(gisPropertyTab);
        lengthValueLabel->setObjectName(QString::fromUtf8("lengthValueLabel"));
        lengthValueLabel->setGeometry(QRect(70, 20, 290, 20));
        lengthValueLabel->setFrameShape(QFrame::Panel);
        lengthValueLabel->setFrameShadow(QFrame::Sunken);
        pathPropertyTab->addTab(gisPropertyTab, QString());
        okButton = new QPushButton(PathDlg);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(220, 590, 75, 24));
        okButton->setAutoDefault(false);
        cancelButton = new QPushButton(PathDlg);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(315, 590, 75, 24));
        cancelButton->setFocusPolicy(Qt::StrongFocus);
        cancelButton->setAutoDefault(false);
        QWidget::setTabOrder(nameEdit, pathPropertyTab);
        QWidget::setTabOrder(pathPropertyTab, descriptionEdit);
        QWidget::setTabOrder(descriptionEdit, latitudeLineEdit);
        QWidget::setTabOrder(latitudeLineEdit, longitudeLineEdit);
        QWidget::setTabOrder(longitudeLineEdit, rangeLineEdit);
        QWidget::setTabOrder(rangeLineEdit, headingLineEdit);
        QWidget::setTabOrder(headingLineEdit, tiltLineEdit);
        QWidget::setTabOrder(tiltLineEdit, snapshotViewButton);
        QWidget::setTabOrder(snapshotViewButton, resetButton);
        QWidget::setTabOrder(resetButton, lineGroupBox);
        QWidget::setTabOrder(lineGroupBox, lineColorButton);
        QWidget::setTabOrder(lineColorButton, lineThickSpin);
        QWidget::setTabOrder(lineThickSpin, lineOpacitySpin);
        QWidget::setTabOrder(lineOpacitySpin, fillGroupBox);
        QWidget::setTabOrder(fillGroupBox, fillColorButton);
        QWidget::setTabOrder(fillColorButton, fillOpacitySpin);
        QWidget::setTabOrder(fillOpacitySpin, heightModeCombo);
        QWidget::setTabOrder(heightModeCombo, heightLineEdit);
        QWidget::setTabOrder(heightLineEdit, lineHeightlSlider);
        QWidget::setTabOrder(lineHeightlSlider, extendCheckBox);
        QWidget::setTabOrder(extendCheckBox, pListTableView);
        QWidget::setTabOrder(pListTableView, addButton);
        QWidget::setTabOrder(addButton, editButton);
        QWidget::setTabOrder(editButton, deleteButton);
        QWidget::setTabOrder(deleteButton, calcGroupBox);
        QWidget::setTabOrder(calcGroupBox, calcPushButton);
        QWidget::setTabOrder(calcPushButton, okButton);
        QWidget::setTabOrder(okButton, cancelButton);

        retranslateUi(PathDlg);

        pathPropertyTab->setCurrentIndex(3);
        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(PathDlg);
    } // setupUi

    void retranslateUi(QDialog *PathDlg)
    {
        PathDlg->setWindowTitle(QCoreApplication::translate("PathDlg", "New Path", nullptr));
        name->setText(QCoreApplication::translate("PathDlg", "Name:", nullptr));
        pathPropertyTab->setTabText(pathPropertyTab->indexOf(descriptionTab), QCoreApplication::translate("PathDlg", "Description", nullptr));
        longitudeLabel->setText(QCoreApplication::translate("PathDlg", "Longitude:", nullptr));
        headingLabel->setText(QCoreApplication::translate("PathDlg", "Heading:", nullptr));
        rangeLabel->setText(QCoreApplication::translate("PathDlg", "Range:", nullptr));
        latitudeLabel->setText(QCoreApplication::translate("PathDlg", "Latitude:", nullptr));
        tiltLabel->setText(QCoreApplication::translate("PathDlg", "Tilt:", nullptr));
        snapshotViewButton->setText(QCoreApplication::translate("PathDlg", "Snapshot current view", nullptr));
        resetButton->setText(QCoreApplication::translate("PathDlg", "Reset", nullptr));
        pathPropertyTab->setTabText(pathPropertyTab->indexOf(viewTab), QCoreApplication::translate("PathDlg", "View", nullptr));
        heightGroupBox->setTitle(QCoreApplication::translate("PathDlg", "Height", nullptr));
        heightLabel->setText(QCoreApplication::translate("PathDlg", "Height:", nullptr));
        heightLineEdit->setInputMask(QString());
        heightLineEdit->setText(QString());
        extendCheckBox->setText(QCoreApplication::translate("PathDlg", "Extend Side To Earth Surface", nullptr));
        pointListGroupBox->setTitle(QCoreApplication::translate("PathDlg", "Point List", nullptr));
        addButton->setText(QCoreApplication::translate("PathDlg", "Add", nullptr));
        deleteButton->setText(QCoreApplication::translate("PathDlg", "Del", nullptr));
        editButton->setText(QCoreApplication::translate("PathDlg", "Edit", nullptr));
        fillGroupBox->setTitle(QCoreApplication::translate("PathDlg", "Fill", nullptr));
        fillOpacitySpin->setSuffix(QString());
        fillColorLabel->setText(QCoreApplication::translate("PathDlg", "Color:", nullptr));
        fillColorButton->setText(QString());
        fillOpacityLabel->setText(QCoreApplication::translate("PathDlg", "Opacity:", nullptr));
        lineGroupBox->setTitle(QCoreApplication::translate("PathDlg", "Line", nullptr));
        lineOpacitySpin->setPrefix(QString());
        lineOpacitySpin->setSuffix(QString());
        lineColorLabel->setText(QCoreApplication::translate("PathDlg", "Color:", nullptr));
        lineColorButton->setText(QString());
        lineOpacityLabel->setText(QCoreApplication::translate("PathDlg", "Opacity:", nullptr));
        lineThickLabel->setText(QCoreApplication::translate("PathDlg", "Thick:", nullptr));
        pathPropertyTab->setTabText(pathPropertyTab->indexOf(shapePropertyTab), QCoreApplication::translate("PathDlg", "Shape Property", nullptr));
        lengthLabel->setText(QCoreApplication::translate("PathDlg", "Length:", nullptr));
        calcGroupBox->setTitle(QCoreApplication::translate("PathDlg", "Property Calc With DEM", nullptr));
        calcLengthLabel->setText(QCoreApplication::translate("PathDlg", "Length:", nullptr));
        calcPushButton->setText(QCoreApplication::translate("PathDlg", "Start Calc", nullptr));
        calcLengthValueLabel->setText(QString());
        maxHeightGroupBox->setTitle(QString());
        maxHeightLongiValueLabel->setText(QString());
        maxHeightValueLabel->setText(QString());
        maxHeightLabel->setText(QCoreApplication::translate("PathDlg", "Max Height:", nullptr));
        maxHeightLatiValueLabel->setText(QString());
        maxHeightLatiLabel->setText(QCoreApplication::translate("PathDlg", "Latitude:", nullptr));
        maxHeightLongiLabel->setText(QCoreApplication::translate("PathDlg", "Longitude:", nullptr));
        minHeightGroupBox->setTitle(QString());
        minHeightLongiValueLabel->setText(QString());
        minHeightLabel->setText(QCoreApplication::translate("PathDlg", "Min Height:", nullptr));
        minHeightLongiLabel->setText(QCoreApplication::translate("PathDlg", "Longitude:", nullptr));
        minHeightLatiValueLabel->setText(QString());
        minHeightValueLabel->setText(QString());
        minHeightLatiLabel->setText(QCoreApplication::translate("PathDlg", "Latitude:", nullptr));
        groupBox->setTitle(QString());
        maxRateLongiValueLabel->setText(QString());
        maxRateAngleLabel->setText(QCoreApplication::translate("PathDlg", "Max Rate Angle:", nullptr));
        maxRateValueLabel->setText(QString());
        maxRateLatiLabel->setText(QCoreApplication::translate("PathDlg", "Latitude:", nullptr));
        maxRateLongiLabel->setText(QCoreApplication::translate("PathDlg", "Longitude:", nullptr));
        maxRateLatiValueLabel->setText(QString());
        lengthValueLabel->setText(QString());
        pathPropertyTab->setTabText(pathPropertyTab->indexOf(gisPropertyTab), QCoreApplication::translate("PathDlg", "GIS Property", nullptr));
        okButton->setText(QCoreApplication::translate("PathDlg", "Ok", nullptr));
        cancelButton->setText(QCoreApplication::translate("PathDlg", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PathDlg: public Ui_PathDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATHDLG_H
