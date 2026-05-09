/********************************************************************************
** Form generated from reading UI file 'buildingpropdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUILDINGPROPDLG_H
#define UI_BUILDINGPROPDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
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
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BuildingPropDlg
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *grpBuilding;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QGridLayout *gridLayout;
    QLabel *label_20;
    QHBoxLayout *horizontalLayout_10;
    QComboBox *cmbB_Posts;
    QLabel *label_2;
    QLabel *label_21;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *edtB_Name;
    QLabel *label_22;
    QLineEdit *edtB_Floor;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *edtB_Address;
    QComboBox *cmbB_Ban;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btnDelPeople;
    QPushButton *btnAddPeople;
    QTabWidget *tabB_Peoples;
    QWidget *tabEmp;
    QVBoxLayout *verticalLayout_2;
    QTableView *tblEmplyee;
    QWidget *tabPop;
    QVBoxLayout *verticalLayout_3;
    QTableView *tblPopulation;
    QGroupBox *grpPeople;
    QVBoxLayout *verticalLayout_7;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QHBoxLayout *horizontalLayout_11;
    QLineEdit *edtP_Post;
    QHBoxLayout *horizontalLayout_8;
    QLineEdit *edtP_Name;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_32;
    QSpinBox *spnP_AgeFrom;
    QLabel *label_33;
    QSpinBox *spnP_AgeTo;
    QLabel *label_9;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *edtP_Address;
    QLabel *label_7;
    QCheckBox *chkP_PreAdd;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnCondition;
    QPushButton *btnClear;
    QPushButton *btnSearch;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnToHouse;
    QPushButton *btnToBuilding;
    QGroupBox *groupBox_4;
    QVBoxLayout *_2;
    QTableView *tblResult;

    void setupUi(QDialog *BuildingPropDlg)
    {
        if (BuildingPropDlg->objectName().isEmpty())
            BuildingPropDlg->setObjectName(QString::fromUtf8("BuildingPropDlg"));
        BuildingPropDlg->resize(860, 600);
        QFont font;
        font.setPointSize(10);
        BuildingPropDlg->setFont(font);
        horizontalLayout = new QHBoxLayout(BuildingPropDlg);
        horizontalLayout->setSpacing(10);
        horizontalLayout->setContentsMargins(5, 5, 5, 5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        grpBuilding = new QGroupBox(BuildingPropDlg);
        grpBuilding->setObjectName(QString::fromUtf8("grpBuilding"));
        verticalLayout_6 = new QVBoxLayout(grpBuilding);
        verticalLayout_6->setSpacing(10);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        groupBox_2 = new QGroupBox(grpBuilding);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setFont(font);
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(-1, 5, -1, 5);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_20 = new QLabel(groupBox_2);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout->addWidget(label_20, 1, 0, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        cmbB_Posts = new QComboBox(groupBox_2);
        cmbB_Posts->setObjectName(QString::fromUtf8("cmbB_Posts"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cmbB_Posts->sizePolicy().hasHeightForWidth());
        cmbB_Posts->setSizePolicy(sizePolicy);
        cmbB_Posts->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);

        horizontalLayout_10->addWidget(cmbB_Posts);


        gridLayout->addLayout(horizontalLayout_10, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_21 = new QLabel(groupBox_2);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout->addWidget(label_21, 2, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        edtB_Name = new QLineEdit(groupBox_2);
        edtB_Name->setObjectName(QString::fromUtf8("edtB_Name"));
        edtB_Name->setReadOnly(true);

        horizontalLayout_4->addWidget(edtB_Name);

        label_22 = new QLabel(groupBox_2);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_22);

        edtB_Floor = new QLineEdit(groupBox_2);
        edtB_Floor->setObjectName(QString::fromUtf8("edtB_Floor"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(edtB_Floor->sizePolicy().hasHeightForWidth());
        edtB_Floor->setSizePolicy(sizePolicy1);
        edtB_Floor->setMaximumSize(QSize(50, 16777215));
        edtB_Floor->setReadOnly(true);

        horizontalLayout_4->addWidget(edtB_Floor);


        gridLayout->addLayout(horizontalLayout_4, 0, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        edtB_Address = new QLineEdit(groupBox_2);
        edtB_Address->setObjectName(QString::fromUtf8("edtB_Address"));
        edtB_Address->setReadOnly(true);

        horizontalLayout_3->addWidget(edtB_Address);

        cmbB_Ban = new QComboBox(groupBox_2);
        cmbB_Ban->setObjectName(QString::fromUtf8("cmbB_Ban"));
        sizePolicy.setHeightForWidth(cmbB_Ban->sizePolicy().hasHeightForWidth());
        cmbB_Ban->setSizePolicy(sizePolicy);
        cmbB_Ban->setMaximumSize(QSize(50, 16777215));
        cmbB_Ban->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);

        horizontalLayout_3->addWidget(cmbB_Ban);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);


        gridLayout->addLayout(horizontalLayout_3, 2, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_3 = new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        btnDelPeople = new QPushButton(groupBox_2);
        btnDelPeople->setObjectName(QString::fromUtf8("btnDelPeople"));
        btnDelPeople->setMaximumSize(QSize(16777215, 22));

        horizontalLayout_5->addWidget(btnDelPeople);

        btnAddPeople = new QPushButton(groupBox_2);
        btnAddPeople->setObjectName(QString::fromUtf8("btnAddPeople"));
        btnAddPeople->setMaximumSize(QSize(16777215, 22));
        btnAddPeople->setCheckable(true);

        horizontalLayout_5->addWidget(btnAddPeople);


        gridLayout->addLayout(horizontalLayout_5, 3, 1, 1, 1);


        verticalLayout_4->addLayout(gridLayout);


        verticalLayout_6->addWidget(groupBox_2);

        tabB_Peoples = new QTabWidget(grpBuilding);
        tabB_Peoples->setObjectName(QString::fromUtf8("tabB_Peoples"));
        tabB_Peoples->setFont(font);
        tabEmp = new QWidget();
        tabEmp->setObjectName(QString::fromUtf8("tabEmp"));
        verticalLayout_2 = new QVBoxLayout(tabEmp);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tblEmplyee = new QTableView(tabEmp);
        tblEmplyee->setObjectName(QString::fromUtf8("tblEmplyee"));
        tblEmplyee->setSelectionMode(QAbstractItemView::SingleSelection);
        tblEmplyee->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblEmplyee->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_2->addWidget(tblEmplyee);

        tabB_Peoples->addTab(tabEmp, QString());
        tabPop = new QWidget();
        tabPop->setObjectName(QString::fromUtf8("tabPop"));
        verticalLayout_3 = new QVBoxLayout(tabPop);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tblPopulation = new QTableView(tabPop);
        tblPopulation->setObjectName(QString::fromUtf8("tblPopulation"));
        tblPopulation->setSelectionMode(QAbstractItemView::MultiSelection);
        tblPopulation->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblPopulation->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_3->addWidget(tblPopulation);

        tabB_Peoples->addTab(tabPop, QString());

        verticalLayout_6->addWidget(tabB_Peoples);


        horizontalLayout->addWidget(grpBuilding);

        grpPeople = new QGroupBox(BuildingPropDlg);
        grpPeople->setObjectName(QString::fromUtf8("grpPeople"));
        verticalLayout_7 = new QVBoxLayout(grpPeople);
        verticalLayout_7->setSpacing(4);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        groupBox_5 = new QGroupBox(grpPeople);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setFont(font);
        verticalLayout = new QVBoxLayout(groupBox_5);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 5, -1, 5);
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(groupBox_5);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        edtP_Post = new QLineEdit(groupBox_5);
        edtP_Post->setObjectName(QString::fromUtf8("edtP_Post"));

        horizontalLayout_11->addWidget(edtP_Post);


        gridLayout_2->addLayout(horizontalLayout_11, 1, 1, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        edtP_Name = new QLineEdit(groupBox_5);
        edtP_Name->setObjectName(QString::fromUtf8("edtP_Name"));
        edtP_Name->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_8->addWidget(edtP_Name);

        horizontalSpacer_4 = new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_4);

        label_32 = new QLabel(groupBox_5);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setAlignment(Qt::AlignCenter);

        horizontalLayout_8->addWidget(label_32);

        spnP_AgeFrom = new QSpinBox(groupBox_5);
        spnP_AgeFrom->setObjectName(QString::fromUtf8("spnP_AgeFrom"));
        spnP_AgeFrom->setMaximum(150);

        horizontalLayout_8->addWidget(spnP_AgeFrom);

        label_33 = new QLabel(groupBox_5);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_33->sizePolicy().hasHeightForWidth());
        label_33->setSizePolicy(sizePolicy2);

        horizontalLayout_8->addWidget(label_33);

        spnP_AgeTo = new QSpinBox(groupBox_5);
        spnP_AgeTo->setObjectName(QString::fromUtf8("spnP_AgeTo"));
        spnP_AgeTo->setMaximum(150);

        horizontalLayout_8->addWidget(spnP_AgeTo);


        gridLayout_2->addLayout(horizontalLayout_8, 0, 1, 1, 1);

        label_9 = new QLabel(groupBox_5);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_2->addWidget(label_9, 1, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        edtP_Address = new QLineEdit(groupBox_5);
        edtP_Address->setObjectName(QString::fromUtf8("edtP_Address"));

        horizontalLayout_6->addWidget(edtP_Address);


        gridLayout_2->addLayout(horizontalLayout_6, 2, 1, 1, 1);

        label_7 = new QLabel(groupBox_5);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 2, 0, 1, 1);

        chkP_PreAdd = new QCheckBox(groupBox_5);
        chkP_PreAdd->setObjectName(QString::fromUtf8("chkP_PreAdd"));
        chkP_PreAdd->setLayoutDirection(Qt::RightToLeft);
        chkP_PreAdd->setChecked(true);

        gridLayout_2->addWidget(chkP_PreAdd, 3, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btnCondition = new QPushButton(groupBox_5);
        btnCondition->setObjectName(QString::fromUtf8("btnCondition"));
        btnCondition->setMaximumSize(QSize(16777215, 22));

        horizontalLayout_2->addWidget(btnCondition);

        btnClear = new QPushButton(groupBox_5);
        btnClear->setObjectName(QString::fromUtf8("btnClear"));
        btnClear->setMaximumSize(QSize(16777215, 22));

        horizontalLayout_2->addWidget(btnClear);

        btnSearch = new QPushButton(groupBox_5);
        btnSearch->setObjectName(QString::fromUtf8("btnSearch"));
        btnSearch->setMaximumSize(QSize(16777215, 22));

        horizontalLayout_2->addWidget(btnSearch);


        gridLayout_2->addLayout(horizontalLayout_2, 3, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_2);


        verticalLayout_7->addWidget(groupBox_5);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(-1, -1, 10, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        btnToHouse = new QPushButton(grpPeople);
        btnToHouse->setObjectName(QString::fromUtf8("btnToHouse"));
        btnToHouse->setMaximumSize(QSize(16777215, 22));
        btnToHouse->setFont(font);

        horizontalLayout_7->addWidget(btnToHouse);

        btnToBuilding = new QPushButton(grpPeople);
        btnToBuilding->setObjectName(QString::fromUtf8("btnToBuilding"));
        btnToBuilding->setMaximumSize(QSize(16777215, 22));
        btnToBuilding->setFont(font);

        horizontalLayout_7->addWidget(btnToBuilding);


        verticalLayout_7->addLayout(horizontalLayout_7);

        groupBox_4 = new QGroupBox(grpPeople);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        _2 = new QVBoxLayout(groupBox_4);
        _2->setSpacing(6);
        _2->setContentsMargins(11, 11, 11, 11);
        _2->setObjectName(QString::fromUtf8("_2"));
        tblResult = new QTableView(groupBox_4);
        tblResult->setObjectName(QString::fromUtf8("tblResult"));
        tblResult->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblResult->setSelectionMode(QAbstractItemView::MultiSelection);
        tblResult->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblResult->horizontalHeader()->setCascadingSectionResizes(true);
        tblResult->horizontalHeader()->setStretchLastSection(true);
        tblResult->verticalHeader()->setCascadingSectionResizes(true);

        _2->addWidget(tblResult);


        verticalLayout_7->addWidget(groupBox_4);


        horizontalLayout->addWidget(grpPeople);

        QWidget::setTabOrder(edtB_Name, edtB_Floor);
        QWidget::setTabOrder(edtB_Floor, cmbB_Posts);
        QWidget::setTabOrder(cmbB_Posts, edtB_Address);
        QWidget::setTabOrder(edtB_Address, cmbB_Ban);
        QWidget::setTabOrder(cmbB_Ban, tabB_Peoples);
        QWidget::setTabOrder(tabB_Peoples, tblEmplyee);
        QWidget::setTabOrder(tblEmplyee, btnAddPeople);
        QWidget::setTabOrder(btnAddPeople, btnDelPeople);
        QWidget::setTabOrder(btnDelPeople, edtP_Name);
        QWidget::setTabOrder(edtP_Name, spnP_AgeFrom);
        QWidget::setTabOrder(spnP_AgeFrom, spnP_AgeTo);
        QWidget::setTabOrder(spnP_AgeTo, edtP_Post);
        QWidget::setTabOrder(edtP_Post, edtP_Address);
        QWidget::setTabOrder(edtP_Address, btnSearch);
        QWidget::setTabOrder(btnSearch, btnClear);
        QWidget::setTabOrder(btnClear, btnCondition);
        QWidget::setTabOrder(btnCondition, chkP_PreAdd);
        QWidget::setTabOrder(chkP_PreAdd, btnToBuilding);
        QWidget::setTabOrder(btnToBuilding, btnToHouse);
        QWidget::setTabOrder(btnToHouse, tblResult);
        QWidget::setTabOrder(tblResult, tblPopulation);

        retranslateUi(BuildingPropDlg);

        tabB_Peoples->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(BuildingPropDlg);
    } // setupUi

    void retranslateUi(QDialog *BuildingPropDlg)
    {
        BuildingPropDlg->setWindowTitle(QCoreApplication::translate("BuildingPropDlg", "BuildingPropDlg", nullptr));
        grpBuilding->setTitle(QString());
        groupBox_2->setTitle(QCoreApplication::translate("BuildingPropDlg", "Building Property", nullptr));
        label_20->setText(QCoreApplication::translate("BuildingPropDlg", "Posts         ", nullptr));
        label_2->setText(QCoreApplication::translate("BuildingPropDlg", "Name          ", nullptr));
        label_21->setText(QCoreApplication::translate("BuildingPropDlg", "Address      ", nullptr));
        label_22->setText(QCoreApplication::translate("BuildingPropDlg", "Floor", nullptr));
        label_3->setText(QCoreApplication::translate("BuildingPropDlg", "Ban", nullptr));
        btnDelPeople->setText(QCoreApplication::translate("BuildingPropDlg", "Delete People", nullptr));
        btnAddPeople->setText(QCoreApplication::translate("BuildingPropDlg", "Add People", nullptr));
        tabB_Peoples->setTabText(tabB_Peoples->indexOf(tabEmp), QCoreApplication::translate("BuildingPropDlg", "Emplyee", nullptr));
        tabB_Peoples->setTabText(tabB_Peoples->indexOf(tabPop), QCoreApplication::translate("BuildingPropDlg", "Population", nullptr));
        grpPeople->setTitle(QString());
        groupBox_5->setTitle(QCoreApplication::translate("BuildingPropDlg", "People Search", nullptr));
        label->setText(QCoreApplication::translate("BuildingPropDlg", "PeopleName", nullptr));
        label_32->setText(QCoreApplication::translate("BuildingPropDlg", "Age", nullptr));
        label_33->setText(QCoreApplication::translate("BuildingPropDlg", "--", nullptr));
        label_9->setText(QCoreApplication::translate("BuildingPropDlg", "Post", nullptr));
        label_7->setText(QCoreApplication::translate("BuildingPropDlg", "Address", nullptr));
        chkP_PreAdd->setText(QCoreApplication::translate("BuildingPropDlg", "PreAdd", nullptr));
        btnCondition->setText(QCoreApplication::translate("BuildingPropDlg", "Condition", nullptr));
        btnClear->setText(QCoreApplication::translate("BuildingPropDlg", "Clear", nullptr));
        btnSearch->setText(QCoreApplication::translate("BuildingPropDlg", "Search", nullptr));
        btnToHouse->setText(QCoreApplication::translate("BuildingPropDlg", "To People", nullptr));
        btnToBuilding->setText(QCoreApplication::translate("BuildingPropDlg", "To Employee", nullptr));
        groupBox_4->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class BuildingPropDlg: public Ui_BuildingPropDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUILDINGPROPDLG_H
