/********************************************************************************
** Form generated from reading UI file 'objectplacesettingdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBJECTPLACESETTINGDLG_H
#define UI_OBJECTPLACESETTINGDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ObjectPlaceSettingDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *gruGeneral;
    QLabel *label_12;
    QSpinBox *spnStartLevel;
    QLineEdit *edtName_ko;
    QLabel *label_8;
    QLabel *label_14;
    QComboBox *cmbObject;
    QLabel *label_11;
    QLabel *label_7;
    QLineEdit *edtName;
    QSpinBox *spnEndLevel;
    QLabel *label_9;
    QComboBox *cmbShapeType;
    QGroupBox *gruIcon;
    QLabel *label_3;
    QLabel *label_10;
    QLineEdit *edtIconPath;
    QPushButton *btnIconPath;
    QSpinBox *spnIconSize;
    QSlider *sldIconSize;
    QGroupBox *gruLine;
    QDoubleSpinBox *spnLineThick;
    QLabel *label_2;
    QLabel *label_13;
    QSlider *sldLineThick;
    QLabel *label_4;
    QSpinBox *spnLineOpacity;
    QPushButton *btnLineColor;
    QGroupBox *gruFill;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label;
    QLineEdit *edtTexturePath;
    QPushButton *btnTexturePath;
    QSpinBox *spnFillOpacity;
    QPushButton *btnFillColor;
    QGroupBox *gruPreview;
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QPushButton *applyButton;

    void setupUi(QDialog *ObjectPlaceSettingDlg)
    {
        if (ObjectPlaceSettingDlg->objectName().isEmpty())
            ObjectPlaceSettingDlg->setObjectName(QString::fromUtf8("ObjectPlaceSettingDlg"));
        ObjectPlaceSettingDlg->resize(420, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ObjectPlaceSettingDlg->sizePolicy().hasHeightForWidth());
        ObjectPlaceSettingDlg->setSizePolicy(sizePolicy);
        ObjectPlaceSettingDlg->setMinimumSize(QSize(420, 600));
        ObjectPlaceSettingDlg->setMaximumSize(QSize(420, 600));
        QFont font;
        font.setPointSize(10);
        ObjectPlaceSettingDlg->setFont(font);
        verticalLayout_2 = new QVBoxLayout(ObjectPlaceSettingDlg);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        gruGeneral = new QGroupBox(ObjectPlaceSettingDlg);
        gruGeneral->setObjectName(QString::fromUtf8("gruGeneral"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(gruGeneral->sizePolicy().hasHeightForWidth());
        gruGeneral->setSizePolicy(sizePolicy1);
        gruGeneral->setMinimumSize(QSize(0, 120));
        label_12 = new QLabel(gruGeneral);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 90, 61, 21));
        spnStartLevel = new QSpinBox(gruGeneral);
        spnStartLevel->setObjectName(QString::fromUtf8("spnStartLevel"));
        spnStartLevel->setGeometry(QRect(80, 90, 51, 22));
        spnStartLevel->setMaximum(25);
        edtName_ko = new QLineEdit(gruGeneral);
        edtName_ko->setObjectName(QString::fromUtf8("edtName_ko"));
        edtName_ko->setGeometry(QRect(80, 55, 111, 22));
        label_8 = new QLabel(gruGeneral);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(210, 55, 61, 21));
        label_14 = new QLabel(gruGeneral);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(210, 90, 61, 21));
        cmbObject = new QComboBox(gruGeneral);
        cmbObject->setObjectName(QString::fromUtf8("cmbObject"));
        cmbObject->setGeometry(QRect(80, 20, 111, 22));
        label_11 = new QLabel(gruGeneral);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 20, 61, 21));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy2);
        label_11->setScaledContents(true);
        label_7 = new QLabel(gruGeneral);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 55, 61, 21));
        edtName = new QLineEdit(gruGeneral);
        edtName->setObjectName(QString::fromUtf8("edtName"));
        edtName->setGeometry(QRect(280, 55, 111, 22));
        spnEndLevel = new QSpinBox(gruGeneral);
        spnEndLevel->setObjectName(QString::fromUtf8("spnEndLevel"));
        spnEndLevel->setGeometry(QRect(280, 90, 51, 22));
        spnEndLevel->setMaximum(25);
        label_9 = new QLabel(gruGeneral);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(210, 20, 62, 21));
        cmbShapeType = new QComboBox(gruGeneral);
        cmbShapeType->setObjectName(QString::fromUtf8("cmbShapeType"));
        cmbShapeType->setGeometry(QRect(280, 20, 111, 22));

        verticalLayout_2->addWidget(gruGeneral);

        gruIcon = new QGroupBox(ObjectPlaceSettingDlg);
        gruIcon->setObjectName(QString::fromUtf8("gruIcon"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(gruIcon->sizePolicy().hasHeightForWidth());
        gruIcon->setSizePolicy(sizePolicy3);
        gruIcon->setMinimumSize(QSize(0, 80));
        gruIcon->setCheckable(true);
        label_3 = new QLabel(gruIcon);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 50, 61, 21));
        label_10 = new QLabel(gruIcon);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 20, 61, 21));
        edtIconPath = new QLineEdit(gruIcon);
        edtIconPath->setObjectName(QString::fromUtf8("edtIconPath"));
        edtIconPath->setEnabled(true);
        edtIconPath->setGeometry(QRect(80, 20, 271, 22));
        btnIconPath = new QPushButton(gruIcon);
        btnIconPath->setObjectName(QString::fromUtf8("btnIconPath"));
        btnIconPath->setGeometry(QRect(360, 20, 31, 22));
        spnIconSize = new QSpinBox(gruIcon);
        spnIconSize->setObjectName(QString::fromUtf8("spnIconSize"));
        spnIconSize->setGeometry(QRect(80, 50, 61, 22));
        spnIconSize->setMaximum(256);
        sldIconSize = new QSlider(gruIcon);
        sldIconSize->setObjectName(QString::fromUtf8("sldIconSize"));
        sldIconSize->setGeometry(QRect(150, 50, 241, 21));
        sldIconSize->setMinimum(0);
        sldIconSize->setMaximum(256);
        sldIconSize->setOrientation(Qt::Horizontal);
        sldIconSize->setTickPosition(QSlider::NoTicks);

        verticalLayout_2->addWidget(gruIcon);

        gruLine = new QGroupBox(ObjectPlaceSettingDlg);
        gruLine->setObjectName(QString::fromUtf8("gruLine"));
        gruLine->setEnabled(false);
        sizePolicy3.setHeightForWidth(gruLine->sizePolicy().hasHeightForWidth());
        gruLine->setSizePolicy(sizePolicy3);
        gruLine->setMinimumSize(QSize(0, 80));
        gruLine->setCheckable(true);
        gruLine->setChecked(false);
        spnLineThick = new QDoubleSpinBox(gruLine);
        spnLineThick->setObjectName(QString::fromUtf8("spnLineThick"));
        spnLineThick->setGeometry(QRect(80, 50, 61, 22));
        spnLineThick->setDecimals(1);
        spnLineThick->setMaximum(10.000000000000000);
        spnLineThick->setSingleStep(0.100000000000000);
        label_2 = new QLabel(gruLine);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 20, 61, 23));
        label_13 = new QLabel(gruLine);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 50, 61, 21));
        sldLineThick = new QSlider(gruLine);
        sldLineThick->setObjectName(QString::fromUtf8("sldLineThick"));
        sldLineThick->setGeometry(QRect(150, 50, 241, 22));
        sldLineThick->setMinimum(0);
        sldLineThick->setMaximum(100);
        sldLineThick->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(gruLine);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(170, 20, 51, 21));
        spnLineOpacity = new QSpinBox(gruLine);
        spnLineOpacity->setObjectName(QString::fromUtf8("spnLineOpacity"));
        spnLineOpacity->setGeometry(QRect(230, 20, 61, 22));
        spnLineOpacity->setMaximum(100);
        btnLineColor = new QPushButton(gruLine);
        btnLineColor->setObjectName(QString::fromUtf8("btnLineColor"));
        btnLineColor->setGeometry(QRect(80, 18, 41, 25));

        verticalLayout_2->addWidget(gruLine);

        gruFill = new QGroupBox(ObjectPlaceSettingDlg);
        gruFill->setObjectName(QString::fromUtf8("gruFill"));
        gruFill->setEnabled(false);
        sizePolicy3.setHeightForWidth(gruFill->sizePolicy().hasHeightForWidth());
        gruFill->setSizePolicy(sizePolicy3);
        gruFill->setMinimumSize(QSize(0, 80));
        gruFill->setCheckable(true);
        gruFill->setChecked(false);
        label_5 = new QLabel(gruFill);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 21, 61, 20));
        label_6 = new QLabel(gruFill);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(170, 20, 51, 21));
        label = new QLabel(gruFill);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 50, 61, 20));
        edtTexturePath = new QLineEdit(gruFill);
        edtTexturePath->setObjectName(QString::fromUtf8("edtTexturePath"));
        edtTexturePath->setGeometry(QRect(80, 50, 271, 22));
        btnTexturePath = new QPushButton(gruFill);
        btnTexturePath->setObjectName(QString::fromUtf8("btnTexturePath"));
        btnTexturePath->setGeometry(QRect(360, 50, 31, 22));
        spnFillOpacity = new QSpinBox(gruFill);
        spnFillOpacity->setObjectName(QString::fromUtf8("spnFillOpacity"));
        spnFillOpacity->setGeometry(QRect(230, 20, 61, 22));
        spnFillOpacity->setMaximum(100);
        btnFillColor = new QPushButton(gruFill);
        btnFillColor->setObjectName(QString::fromUtf8("btnFillColor"));
        btnFillColor->setGeometry(QRect(80, 18, 41, 25));

        verticalLayout_2->addWidget(gruFill);

        gruPreview = new QGroupBox(ObjectPlaceSettingDlg);
        gruPreview->setObjectName(QString::fromUtf8("gruPreview"));
        sizePolicy1.setHeightForWidth(gruPreview->sizePolicy().hasHeightForWidth());
        gruPreview->setSizePolicy(sizePolicy1);
        gruPreview->setMinimumSize(QSize(0, 160));
        verticalLayout = new QVBoxLayout(gruPreview);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(7, 0, 7, 7);
        graphicsView = new QGraphicsView(gruPreview);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        verticalLayout->addWidget(graphicsView);


        verticalLayout_2->addWidget(gruPreview);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okButton = new QPushButton(ObjectPlaceSettingDlg);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setAutoDefault(false);

        horizontalLayout->addWidget(okButton);

        cancelButton = new QPushButton(ObjectPlaceSettingDlg);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setFocusPolicy(Qt::StrongFocus);
        cancelButton->setAutoDefault(false);

        horizontalLayout->addWidget(cancelButton);

        applyButton = new QPushButton(ObjectPlaceSettingDlg);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));
        applyButton->setFocusPolicy(Qt::StrongFocus);
        applyButton->setAutoDefault(false);

        horizontalLayout->addWidget(applyButton);


        verticalLayout_2->addLayout(horizontalLayout);

        QWidget::setTabOrder(cmbObject, edtName_ko);
        QWidget::setTabOrder(edtName_ko, edtName);
        QWidget::setTabOrder(edtName, cmbShapeType);
        QWidget::setTabOrder(cmbShapeType, spnStartLevel);
        QWidget::setTabOrder(spnStartLevel, spnEndLevel);
        QWidget::setTabOrder(spnEndLevel, gruIcon);
        QWidget::setTabOrder(gruIcon, edtIconPath);
        QWidget::setTabOrder(edtIconPath, btnIconPath);
        QWidget::setTabOrder(btnIconPath, spnIconSize);
        QWidget::setTabOrder(spnIconSize, sldIconSize);
        QWidget::setTabOrder(sldIconSize, gruLine);
        QWidget::setTabOrder(gruLine, spnLineOpacity);
        QWidget::setTabOrder(spnLineOpacity, spnLineThick);
        QWidget::setTabOrder(spnLineThick, sldLineThick);
        QWidget::setTabOrder(sldLineThick, gruFill);
        QWidget::setTabOrder(gruFill, spnFillOpacity);
        QWidget::setTabOrder(spnFillOpacity, edtTexturePath);
        QWidget::setTabOrder(edtTexturePath, btnTexturePath);
        QWidget::setTabOrder(btnTexturePath, graphicsView);
        QWidget::setTabOrder(graphicsView, okButton);
        QWidget::setTabOrder(okButton, cancelButton);
        QWidget::setTabOrder(cancelButton, applyButton);

        retranslateUi(ObjectPlaceSettingDlg);

        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(ObjectPlaceSettingDlg);
    } // setupUi

    void retranslateUi(QDialog *ObjectPlaceSettingDlg)
    {
        ObjectPlaceSettingDlg->setWindowTitle(QCoreApplication::translate("ObjectPlaceSettingDlg", "New Polygon", nullptr));
        gruGeneral->setTitle(QCoreApplication::translate("ObjectPlaceSettingDlg", "General", nullptr));
        label_12->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "StartLevel", nullptr));
        label_8->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "Name", nullptr));
        label_14->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "EndLevel", nullptr));
        label_11->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "Object", nullptr));
        label_7->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "Name_ko ", nullptr));
        label_9->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "ShapeType", nullptr));
        gruIcon->setTitle(QCoreApplication::translate("ObjectPlaceSettingDlg", "Icon", nullptr));
        label_3->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "IconSize", nullptr));
        label_10->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "IconPath", nullptr));
        btnIconPath->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "...", nullptr));
        gruLine->setTitle(QCoreApplication::translate("ObjectPlaceSettingDlg", "Line", nullptr));
        label_2->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "LineColor", nullptr));
        label_13->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "LineThick", nullptr));
        label_4->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "Opacity", nullptr));
        spnLineOpacity->setSuffix(QCoreApplication::translate("ObjectPlaceSettingDlg", " %", nullptr));
        gruFill->setTitle(QCoreApplication::translate("ObjectPlaceSettingDlg", "Fill", nullptr));
        label_5->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "FillColor", nullptr));
        label_6->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "Opacity", nullptr));
        label->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "FillTexture", nullptr));
        btnTexturePath->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "...", nullptr));
        spnFillOpacity->setSuffix(QCoreApplication::translate("ObjectPlaceSettingDlg", " %", nullptr));
        gruPreview->setTitle(QCoreApplication::translate("ObjectPlaceSettingDlg", "Preview", nullptr));
        okButton->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "Ok", nullptr));
        cancelButton->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "Cancel", nullptr));
        applyButton->setText(QCoreApplication::translate("ObjectPlaceSettingDlg", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ObjectPlaceSettingDlg: public Ui_ObjectPlaceSettingDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBJECTPLACESETTINGDLG_H
