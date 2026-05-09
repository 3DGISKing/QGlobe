/********************************************************************************
** Form generated from reading UI file 'rulerdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RULERDLG_H
#define UI_RULERDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RulerDlg
{
public:
    QTabWidget *rulerModeTab;
    QWidget *lineTab;
    QLabel *lineLengthValue;
    QLabel *lineAngleValue;
    QComboBox *lineMetricsCombo;
    QLabel *lineAngleUnit;
    QLabel *lineLengthTitle;
    QLabel *lineAngleTitle;
    QWidget *pathTab;
    QComboBox *pathMetricsCombo;
    QLabel *pathLengthTitle;
    QLabel *pathLengthValue;
    QPushButton *deleteButton;
    QCheckBox *mouseCheck;

    void setupUi(QDialog *RulerDlg)
    {
        if (RulerDlg->objectName().isEmpty())
            RulerDlg->setObjectName(QString::fromUtf8("RulerDlg"));
        RulerDlg->resize(400, 165);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RulerDlg->sizePolicy().hasHeightForWidth());
        RulerDlg->setSizePolicy(sizePolicy);
        RulerDlg->setMinimumSize(QSize(400, 165));
        RulerDlg->setMaximumSize(QSize(400, 165));
        RulerDlg->setSizeGripEnabled(false);
        rulerModeTab = new QTabWidget(RulerDlg);
        rulerModeTab->setObjectName(QString::fromUtf8("rulerModeTab"));
        rulerModeTab->setGeometry(QRect(10, 10, 380, 121));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(rulerModeTab->sizePolicy().hasHeightForWidth());
        rulerModeTab->setSizePolicy(sizePolicy1);
        lineTab = new QWidget();
        lineTab->setObjectName(QString::fromUtf8("lineTab"));
        lineLengthValue = new QLabel(lineTab);
        lineLengthValue->setObjectName(QString::fromUtf8("lineLengthValue"));
        lineLengthValue->setGeometry(QRect(75, 20, 180, 20));
        lineAngleValue = new QLabel(lineTab);
        lineAngleValue->setObjectName(QString::fromUtf8("lineAngleValue"));
        lineAngleValue->setGeometry(QRect(75, 50, 180, 20));
        lineMetricsCombo = new QComboBox(lineTab);
        lineMetricsCombo->setObjectName(QString::fromUtf8("lineMetricsCombo"));
        lineMetricsCombo->setGeometry(QRect(260, 20, 100, 20));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(3);
        sizePolicy2.setVerticalStretch(3);
        sizePolicy2.setHeightForWidth(lineMetricsCombo->sizePolicy().hasHeightForWidth());
        lineMetricsCombo->setSizePolicy(sizePolicy2);
        lineMetricsCombo->setSizeIncrement(QSize(1, 1));
        lineMetricsCombo->setMaxVisibleItems(20);
        lineAngleUnit = new QLabel(lineTab);
        lineAngleUnit->setObjectName(QString::fromUtf8("lineAngleUnit"));
        lineAngleUnit->setGeometry(QRect(260, 50, 100, 20));
        lineLengthTitle = new QLabel(lineTab);
        lineLengthTitle->setObjectName(QString::fromUtf8("lineLengthTitle"));
        lineLengthTitle->setGeometry(QRect(10, 20, 65, 20));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(3);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(lineLengthTitle->sizePolicy().hasHeightForWidth());
        lineLengthTitle->setSizePolicy(sizePolicy3);
        lineLengthTitle->setSizeIncrement(QSize(1, 1));
        lineAngleTitle = new QLabel(lineTab);
        lineAngleTitle->setObjectName(QString::fromUtf8("lineAngleTitle"));
        lineAngleTitle->setGeometry(QRect(10, 50, 65, 20));
        rulerModeTab->addTab(lineTab, QString());
        pathTab = new QWidget();
        pathTab->setObjectName(QString::fromUtf8("pathTab"));
        pathMetricsCombo = new QComboBox(pathTab);
        pathMetricsCombo->setObjectName(QString::fromUtf8("pathMetricsCombo"));
        pathMetricsCombo->setGeometry(QRect(260, 20, 100, 20));
        pathMetricsCombo->setMaxVisibleItems(20);
        pathLengthTitle = new QLabel(pathTab);
        pathLengthTitle->setObjectName(QString::fromUtf8("pathLengthTitle"));
        pathLengthTitle->setGeometry(QRect(10, 20, 65, 20));
        pathLengthValue = new QLabel(pathTab);
        pathLengthValue->setObjectName(QString::fromUtf8("pathLengthValue"));
        pathLengthValue->setGeometry(QRect(75, 20, 180, 20));
        rulerModeTab->addTab(pathTab, QString());
        deleteButton = new QPushButton(RulerDlg);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));
        deleteButton->setGeometry(QRect(315, 135, 75, 24));
        sizePolicy.setHeightForWidth(deleteButton->sizePolicy().hasHeightForWidth());
        deleteButton->setSizePolicy(sizePolicy);
        deleteButton->setFocusPolicy(Qt::StrongFocus);
        deleteButton->setLayoutDirection(Qt::RightToLeft);
        deleteButton->setAutoDefault(false);
        mouseCheck = new QCheckBox(RulerDlg);
        mouseCheck->setObjectName(QString::fromUtf8("mouseCheck"));
        mouseCheck->setGeometry(QRect(10, 135, 150, 24));
        sizePolicy.setHeightForWidth(mouseCheck->sizePolicy().hasHeightForWidth());
        mouseCheck->setSizePolicy(sizePolicy);
        QWidget::setTabOrder(rulerModeTab, lineMetricsCombo);
        QWidget::setTabOrder(lineMetricsCombo, pathMetricsCombo);
        QWidget::setTabOrder(pathMetricsCombo, mouseCheck);
        QWidget::setTabOrder(mouseCheck, deleteButton);

        retranslateUi(RulerDlg);

        rulerModeTab->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(RulerDlg);
    } // setupUi

    void retranslateUi(QDialog *RulerDlg)
    {
        RulerDlg->setWindowTitle(QCoreApplication::translate("RulerDlg", "Ruler", nullptr));
        lineLengthValue->setText(QString());
        lineAngleValue->setText(QString());
        lineAngleUnit->setText(QCoreApplication::translate("RulerDlg", "degrees", nullptr));
        lineLengthTitle->setText(QCoreApplication::translate("RulerDlg", "Length:", nullptr));
        lineAngleTitle->setText(QCoreApplication::translate("RulerDlg", "Heading:", nullptr));
        rulerModeTab->setTabText(rulerModeTab->indexOf(lineTab), QCoreApplication::translate("RulerDlg", "Line", nullptr));
        pathLengthTitle->setText(QCoreApplication::translate("RulerDlg", "Length:", nullptr));
        pathLengthValue->setText(QString());
        rulerModeTab->setTabText(rulerModeTab->indexOf(pathTab), QCoreApplication::translate("RulerDlg", "Path", nullptr));
        deleteButton->setText(QCoreApplication::translate("RulerDlg", "Clear", nullptr));
        mouseCheck->setText(QCoreApplication::translate("RulerDlg", "Mouse Navigation", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RulerDlg: public Ui_RulerDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RULERDLG_H
