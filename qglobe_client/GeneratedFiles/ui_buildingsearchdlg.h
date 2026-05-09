/********************************************************************************
** Form generated from reading UI file 'buildingsearchdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUILDINGSEARCHDLG_H
#define UI_BUILDINGSEARCHDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BuildingSearchDlg
{
public:
    QVBoxLayout *verticalLayout_7;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *comboBoxProvince;
    QComboBox *comboBoxCounty;
    QLabel *label;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *comboBoxVilliage;
    QSpinBox *spinBoxBan;
    QLabel *label_5;
    QLineEdit *lineEditName;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_8;
    QSpinBox *spinBoxFloor;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *pushButtonFind;
    QHBoxLayout *horizontalLayout_7;
    QTableView *tableView;

    void setupUi(QDialog *BuildingSearchDlg)
    {
        if (BuildingSearchDlg->objectName().isEmpty())
            BuildingSearchDlg->setObjectName(QString::fromUtf8("BuildingSearchDlg"));
        BuildingSearchDlg->resize(400, 550);
        verticalLayout_7 = new QVBoxLayout(BuildingSearchDlg);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        groupBox = new QGroupBox(BuildingSearchDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(5);
        verticalLayout_2->setContentsMargins(5, 5, 5, 5);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy1);
        verticalLayout_3 = new QVBoxLayout(groupBox_4);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(5, 5, 5, 5);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        comboBoxProvince = new QComboBox(groupBox_4);
        comboBoxProvince->setObjectName(QString::fromUtf8("comboBoxProvince"));

        horizontalLayout_2->addWidget(comboBoxProvince);

        comboBoxCounty = new QComboBox(groupBox_4);
        comboBoxCounty->setObjectName(QString::fromUtf8("comboBoxCounty"));

        horizontalLayout_2->addWidget(comboBoxCounty);


        gridLayout->addLayout(horizontalLayout_2, 1, 1, 1, 1);

        label = new QLabel(groupBox_4);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        comboBoxVilliage = new QComboBox(groupBox_4);
        comboBoxVilliage->setObjectName(QString::fromUtf8("comboBoxVilliage"));
        comboBoxVilliage->setEditable(true);

        horizontalLayout_4->addWidget(comboBoxVilliage);

        spinBoxBan = new QSpinBox(groupBox_4);
        spinBoxBan->setObjectName(QString::fromUtf8("spinBoxBan"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(spinBoxBan->sizePolicy().hasHeightForWidth());
        spinBoxBan->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(spinBoxBan);

        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_4->addWidget(label_5);


        gridLayout->addLayout(horizontalLayout_4, 2, 1, 1, 1);

        lineEditName = new QLineEdit(groupBox_4);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));

        gridLayout->addWidget(lineEditName, 0, 1, 1, 1);

        label_7 = new QLabel(groupBox_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 3, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        spinBoxFloor = new QSpinBox(groupBox_4);
        spinBoxFloor->setObjectName(QString::fromUtf8("spinBoxFloor"));

        horizontalLayout_8->addWidget(spinBoxFloor);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_8);

        pushButtonFind = new QPushButton(groupBox_4);
        pushButtonFind->setObjectName(QString::fromUtf8("pushButtonFind"));

        horizontalLayout_8->addWidget(pushButtonFind);


        gridLayout->addLayout(horizontalLayout_8, 3, 1, 1, 1);


        verticalLayout_3->addLayout(gridLayout);


        verticalLayout_2->addWidget(groupBox_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(1, 1, 1, 1);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        tableView = new QTableView(groupBox);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        horizontalLayout_7->addWidget(tableView);


        verticalLayout_2->addLayout(horizontalLayout_7);


        verticalLayout_7->addWidget(groupBox);


        retranslateUi(BuildingSearchDlg);

        QMetaObject::connectSlotsByName(BuildingSearchDlg);
    } // setupUi

    void retranslateUi(QDialog *BuildingSearchDlg)
    {
        BuildingSearchDlg->setWindowTitle(QCoreApplication::translate("BuildingSearchDlg", "BuildingSearchDlg", nullptr));
        groupBox->setTitle(QCoreApplication::translate("BuildingSearchDlg", "Search Result", nullptr));
        groupBox_4->setTitle(QString());
        label->setText(QCoreApplication::translate("BuildingSearchDlg", "Building Name", nullptr));
        label_2->setText(QCoreApplication::translate("BuildingSearchDlg", "Address", nullptr));
        label_5->setText(QCoreApplication::translate("BuildingSearchDlg", "Ban      ", nullptr));
        label_7->setText(QCoreApplication::translate("BuildingSearchDlg", "Floor            ", nullptr));
        pushButtonFind->setText(QCoreApplication::translate("BuildingSearchDlg", "Find", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BuildingSearchDlg: public Ui_BuildingSearchDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUILDINGSEARCHDLG_H
