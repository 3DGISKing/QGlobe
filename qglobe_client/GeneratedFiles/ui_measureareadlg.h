/********************************************************************************
** Form generated from reading UI file 'measureareadlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEASUREAREADLG_H
#define UI_MEASUREAREADLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDial>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MeasureAreaDlg
{
public:
    QPushButton *deleteButton;
    QLabel *lineAreaTitle;
    QLabel *lineArea;
    QGroupBox *groupBoxSimulation;
    QLabel *label;
    QLineEdit *lineEditMoveDirection;
    QLabel *label_2;
    QLineEdit *lineEditMoveVelocity;
    QLabel *label_3;
    QLineEdit *lineEditGrowVelocity;
    QDial *dialMoveDirection;
    QLabel *label_4;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonStart;
    QPushButton *pushButtonStop;
    QPushButton *btnSearch;

    void setupUi(QDialog *MeasureAreaDlg)
    {
        if (MeasureAreaDlg->objectName().isEmpty())
            MeasureAreaDlg->setObjectName(QString::fromUtf8("MeasureAreaDlg"));
        MeasureAreaDlg->resize(296, 208);
        MeasureAreaDlg->setMaximumSize(QSize(296, 208));
        deleteButton = new QPushButton(MeasureAreaDlg);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));
        deleteButton->setGeometry(QRect(210, 180, 75, 23));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(deleteButton->sizePolicy().hasHeightForWidth());
        deleteButton->setSizePolicy(sizePolicy);
        deleteButton->setFocusPolicy(Qt::StrongFocus);
        deleteButton->setLayoutDirection(Qt::RightToLeft);
        deleteButton->setAutoDefault(false);
        lineAreaTitle = new QLabel(MeasureAreaDlg);
        lineAreaTitle->setObjectName(QString::fromUtf8("lineAreaTitle"));
        lineAreaTitle->setGeometry(QRect(10, 10, 65, 20));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(3);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(lineAreaTitle->sizePolicy().hasHeightForWidth());
        lineAreaTitle->setSizePolicy(sizePolicy1);
        lineAreaTitle->setSizeIncrement(QSize(1, 1));
        lineArea = new QLabel(MeasureAreaDlg);
        lineArea->setObjectName(QString::fromUtf8("lineArea"));
        lineArea->setGeometry(QRect(80, 10, 161, 20));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(3);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(lineArea->sizePolicy().hasHeightForWidth());
        lineArea->setSizePolicy(sizePolicy2);
        lineArea->setSizeIncrement(QSize(1, 1));
        groupBoxSimulation = new QGroupBox(MeasureAreaDlg);
        groupBoxSimulation->setObjectName(QString::fromUtf8("groupBoxSimulation"));
        groupBoxSimulation->setGeometry(QRect(10, 30, 281, 141));
        groupBoxSimulation->setCheckable(true);
        label = new QLabel(groupBoxSimulation);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(11, 21, 71, 16));
        lineEditMoveDirection = new QLineEdit(groupBoxSimulation);
        lineEditMoveDirection->setObjectName(QString::fromUtf8("lineEditMoveDirection"));
        lineEditMoveDirection->setGeometry(QRect(110, 21, 61, 20));
        lineEditMoveDirection->setReadOnly(true);
        label_2 = new QLabel(groupBoxSimulation);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(11, 51, 65, 16));
        lineEditMoveVelocity = new QLineEdit(groupBoxSimulation);
        lineEditMoveVelocity->setObjectName(QString::fromUtf8("lineEditMoveVelocity"));
        lineEditMoveVelocity->setGeometry(QRect(110, 51, 61, 20));
        label_3 = new QLabel(groupBoxSimulation);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(11, 81, 64, 16));
        lineEditGrowVelocity = new QLineEdit(groupBoxSimulation);
        lineEditGrowVelocity->setObjectName(QString::fromUtf8("lineEditGrowVelocity"));
        lineEditGrowVelocity->setGeometry(QRect(110, 81, 61, 20));
        dialMoveDirection = new QDial(groupBoxSimulation);
        dialMoveDirection->setObjectName(QString::fromUtf8("dialMoveDirection"));
        dialMoveDirection->setGeometry(QRect(190, 30, 71, 71));
        dialMoveDirection->setMaximum(359);
        dialMoveDirection->setWrapping(true);
        dialMoveDirection->setNotchTarget(45.000000000000000);
        dialMoveDirection->setNotchesVisible(true);
        label_4 = new QLabel(groupBoxSimulation);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(215, 10, 21, 16));
        label_4->setAlignment(Qt::AlignCenter);
        layoutWidget = new QWidget(groupBoxSimulation);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(11, 107, 261, 27));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonStart = new QPushButton(layoutWidget);
        pushButtonStart->setObjectName(QString::fromUtf8("pushButtonStart"));

        horizontalLayout->addWidget(pushButtonStart);

        pushButtonStop = new QPushButton(layoutWidget);
        pushButtonStop->setObjectName(QString::fromUtf8("pushButtonStop"));

        horizontalLayout->addWidget(pushButtonStop);

        label->raise();
        lineEditMoveDirection->raise();
        label_2->raise();
        lineEditMoveVelocity->raise();
        label_3->raise();
        lineEditGrowVelocity->raise();
        dialMoveDirection->raise();
        label_4->raise();
        layoutWidget->raise();
        lineAreaTitle->raise();
        btnSearch = new QPushButton(MeasureAreaDlg);
        btnSearch->setObjectName(QString::fromUtf8("btnSearch"));
        btnSearch->setGeometry(QRect(10, 180, 75, 23));

        retranslateUi(MeasureAreaDlg);

        QMetaObject::connectSlotsByName(MeasureAreaDlg);
    } // setupUi

    void retranslateUi(QDialog *MeasureAreaDlg)
    {
        MeasureAreaDlg->setWindowTitle(QCoreApplication::translate("MeasureAreaDlg", "MeasureArea", nullptr));
        deleteButton->setText(QCoreApplication::translate("MeasureAreaDlg", "Clear", nullptr));
        lineAreaTitle->setText(QCoreApplication::translate("MeasureAreaDlg", "Area:", nullptr));
        lineArea->setText(QString());
        groupBoxSimulation->setTitle(QCoreApplication::translate("MeasureAreaDlg", "Simulation", nullptr));
        label->setText(QCoreApplication::translate("MeasureAreaDlg", "Move Direction", nullptr));
        label_2->setText(QCoreApplication::translate("MeasureAreaDlg", "Move Velocity", nullptr));
        label_3->setText(QCoreApplication::translate("MeasureAreaDlg", "Grow Velocity", nullptr));
        label_4->setText(QCoreApplication::translate("MeasureAreaDlg", "N", nullptr));
        pushButtonStart->setText(QCoreApplication::translate("MeasureAreaDlg", "Start", nullptr));
        pushButtonStop->setText(QCoreApplication::translate("MeasureAreaDlg", "Stop", nullptr));
        btnSearch->setText(QCoreApplication::translate("MeasureAreaDlg", "Search", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MeasureAreaDlg: public Ui_MeasureAreaDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEASUREAREADLG_H
