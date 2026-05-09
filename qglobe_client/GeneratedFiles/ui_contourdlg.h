/********************************************************************************
** Form generated from reading UI file 'contourdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTOURDLG_H
#define UI_CONTOURDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_ContourDlg
{
public:
    QPushButton *okButton;
    QPushButton *cancelButton;
    QPushButton *defaultValueButton;
    QLabel *intervalLabel;
    QGroupBox *ravineGroup;
    QLabel *ravineColorLabel;
    QLabel *ravineThickLabel;
    QSpinBox *ravineThickSpin;
    QToolButton *ravineColorButton;
    QGroupBox *mainCurveGroup;
    QLabel *mainColorLabel;
    QLabel *mainThickLabel;
    QSpinBox *mainThickSpin;
    QToolButton *mainColorButton;
    QLineEdit *intervalEdit;
    QLabel *intervalUnitLabel;

    void setupUi(QDialog *ContourDlg)
    {
        if (ContourDlg->objectName().isEmpty())
            ContourDlg->setObjectName(QString::fromUtf8("ContourDlg"));
        ContourDlg->resize(400, 184);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ContourDlg->sizePolicy().hasHeightForWidth());
        ContourDlg->setSizePolicy(sizePolicy);
        ContourDlg->setMinimumSize(QSize(400, 184));
        ContourDlg->setMaximumSize(QSize(400, 184));
        ContourDlg->setModal(true);
        okButton = new QPushButton(ContourDlg);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(235, 150, 75, 24));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(okButton->sizePolicy().hasHeightForWidth());
        okButton->setSizePolicy(sizePolicy1);
        okButton->setMinimumSize(QSize(75, 24));
        okButton->setMaximumSize(QSize(75, 24));
        okButton->setFocusPolicy(Qt::StrongFocus);
        okButton->setAutoDefault(false);
        cancelButton = new QPushButton(ContourDlg);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(315, 150, 75, 24));
        sizePolicy1.setHeightForWidth(cancelButton->sizePolicy().hasHeightForWidth());
        cancelButton->setSizePolicy(sizePolicy1);
        cancelButton->setMinimumSize(QSize(75, 24));
        cancelButton->setMaximumSize(QSize(75, 24));
        cancelButton->setFocusPolicy(Qt::StrongFocus);
        cancelButton->setAutoDefault(false);
        defaultValueButton = new QPushButton(ContourDlg);
        defaultValueButton->setObjectName(QString::fromUtf8("defaultValueButton"));
        defaultValueButton->setGeometry(QRect(10, 150, 110, 24));
        sizePolicy1.setHeightForWidth(defaultValueButton->sizePolicy().hasHeightForWidth());
        defaultValueButton->setSizePolicy(sizePolicy1);
        defaultValueButton->setFocusPolicy(Qt::StrongFocus);
        defaultValueButton->setAutoDefault(false);
        intervalLabel = new QLabel(ContourDlg);
        intervalLabel->setObjectName(QString::fromUtf8("intervalLabel"));
        intervalLabel->setGeometry(QRect(10, 10, 75, 22));
        sizePolicy1.setHeightForWidth(intervalLabel->sizePolicy().hasHeightForWidth());
        intervalLabel->setSizePolicy(sizePolicy1);
        ravineGroup = new QGroupBox(ContourDlg);
        ravineGroup->setObjectName(QString::fromUtf8("ravineGroup"));
        ravineGroup->setGeometry(QRect(10, 42, 180, 91));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ravineGroup->sizePolicy().hasHeightForWidth());
        ravineGroup->setSizePolicy(sizePolicy2);
        ravineColorLabel = new QLabel(ravineGroup);
        ravineColorLabel->setObjectName(QString::fromUtf8("ravineColorLabel"));
        ravineColorLabel->setGeometry(QRect(10, 25, 40, 22));
        sizePolicy1.setHeightForWidth(ravineColorLabel->sizePolicy().hasHeightForWidth());
        ravineColorLabel->setSizePolicy(sizePolicy1);
        ravineThickLabel = new QLabel(ravineGroup);
        ravineThickLabel->setObjectName(QString::fromUtf8("ravineThickLabel"));
        ravineThickLabel->setGeometry(QRect(10, 55, 40, 22));
        sizePolicy1.setHeightForWidth(ravineThickLabel->sizePolicy().hasHeightForWidth());
        ravineThickLabel->setSizePolicy(sizePolicy1);
        ravineThickSpin = new QSpinBox(ravineGroup);
        ravineThickSpin->setObjectName(QString::fromUtf8("ravineThickSpin"));
        ravineThickSpin->setGeometry(QRect(57, 55, 50, 22));
        ravineColorButton = new QToolButton(ravineGroup);
        ravineColorButton->setObjectName(QString::fromUtf8("ravineColorButton"));
        ravineColorButton->setGeometry(QRect(57, 25, 30, 22));
        mainCurveGroup = new QGroupBox(ContourDlg);
        mainCurveGroup->setObjectName(QString::fromUtf8("mainCurveGroup"));
        mainCurveGroup->setGeometry(QRect(210, 42, 180, 91));
        mainColorLabel = new QLabel(mainCurveGroup);
        mainColorLabel->setObjectName(QString::fromUtf8("mainColorLabel"));
        mainColorLabel->setGeometry(QRect(10, 25, 40, 22));
        sizePolicy1.setHeightForWidth(mainColorLabel->sizePolicy().hasHeightForWidth());
        mainColorLabel->setSizePolicy(sizePolicy1);
        mainThickLabel = new QLabel(mainCurveGroup);
        mainThickLabel->setObjectName(QString::fromUtf8("mainThickLabel"));
        mainThickLabel->setGeometry(QRect(10, 55, 40, 22));
        sizePolicy1.setHeightForWidth(mainThickLabel->sizePolicy().hasHeightForWidth());
        mainThickLabel->setSizePolicy(sizePolicy1);
        mainThickSpin = new QSpinBox(mainCurveGroup);
        mainThickSpin->setObjectName(QString::fromUtf8("mainThickSpin"));
        mainThickSpin->setGeometry(QRect(57, 55, 50, 22));
        mainColorButton = new QToolButton(mainCurveGroup);
        mainColorButton->setObjectName(QString::fromUtf8("mainColorButton"));
        mainColorButton->setGeometry(QRect(57, 25, 30, 22));
        intervalEdit = new QLineEdit(ContourDlg);
        intervalEdit->setObjectName(QString::fromUtf8("intervalEdit"));
        intervalEdit->setGeometry(QRect(100, 10, 60, 22));
        intervalUnitLabel = new QLabel(ContourDlg);
        intervalUnitLabel->setObjectName(QString::fromUtf8("intervalUnitLabel"));
        intervalUnitLabel->setGeometry(QRect(165, 10, 150, 22));
        QWidget::setTabOrder(intervalEdit, ravineColorButton);
        QWidget::setTabOrder(ravineColorButton, ravineThickSpin);
        QWidget::setTabOrder(ravineThickSpin, mainColorButton);
        QWidget::setTabOrder(mainColorButton, mainThickSpin);
        QWidget::setTabOrder(mainThickSpin, defaultValueButton);
        QWidget::setTabOrder(defaultValueButton, okButton);
        QWidget::setTabOrder(okButton, cancelButton);

        retranslateUi(ContourDlg);

        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(ContourDlg);
    } // setupUi

    void retranslateUi(QDialog *ContourDlg)
    {
        ContourDlg->setWindowTitle(QCoreApplication::translate("ContourDlg", "Contour Line Setting", nullptr));
        okButton->setText(QCoreApplication::translate("ContourDlg", "Ok", nullptr));
        cancelButton->setText(QCoreApplication::translate("ContourDlg", "Cancel", nullptr));
        defaultValueButton->setText(QCoreApplication::translate("ContourDlg", "Restore Defaults", nullptr));
        intervalLabel->setText(QCoreApplication::translate("ContourDlg", "Interval:", nullptr));
        ravineGroup->setTitle(QCoreApplication::translate("ContourDlg", "Index Contour", nullptr));
        ravineColorLabel->setText(QCoreApplication::translate("ContourDlg", "Color:", nullptr));
        ravineThickLabel->setText(QCoreApplication::translate("ContourDlg", "Thick:", nullptr));
        ravineColorButton->setText(QString());
        mainCurveGroup->setTitle(QCoreApplication::translate("ContourDlg", "Intermediate Contour", nullptr));
        mainColorLabel->setText(QCoreApplication::translate("ContourDlg", "Color:", nullptr));
        mainThickLabel->setText(QCoreApplication::translate("ContourDlg", "Thick:", nullptr));
        mainColorButton->setText(QString());
        intervalUnitLabel->setText(QCoreApplication::translate("ContourDlg", "(20 ~ 10000m)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ContourDlg: public Ui_ContourDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTOURDLG_H
