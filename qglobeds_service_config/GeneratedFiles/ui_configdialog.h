/********************************************************************************
** Form generated from reading UI file 'configdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGDIALOG_H
#define UI_CONFIGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfigDialog
{
public:
    QTabWidget *tabWidget;
    QWidget *ServerTab;
    QLabel *label_2;
    QSpinBox *srvportSpin;
    QSpinBox *srvconnectSpin;
    QLabel *label_7;
    QPushButton *browseButton;
    QLabel *label_6;
    QLineEdit *dbpathEdit;
    QWidget *DatabaseTab;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *dbusrEdit;
    QLineEdit *dbpasswdEdit;
    QLineEdit *dbnameEdit;
    QLabel *label;
    QLineEdit *srvaddrEdit;
    QWidget *tab;
    QTableView *pcTable;
    QTableView *hardTable;
    QPushButton *addDistItemBtn;
    QPushButton *rmvDistItemBtn;
    QGroupBox *disttypeGroup;
    QRadioButton *pcRadioBtn;
    QRadioButton *hardRadioBtn;
    QRadioButton *noneRadioBtn;
    QGroupBox *groupDisNum;
    QSpinBox *spinDistOrgNum;
    QSpinBox *spinDistNum;
    QLabel *labelDistOrgNum;
    QLabel *labeDistNum;
    QDialogButtonBox *buttonBox;
    QPushButton *serviceButton;

    void setupUi(QDialog *ConfigDialog)
    {
        if (ConfigDialog->objectName().isEmpty())
            ConfigDialog->setObjectName(QString::fromUtf8("ConfigDialog"));
        ConfigDialog->resize(498, 531);
        ConfigDialog->setMinimumSize(QSize(465, 313));
        ConfigDialog->setMaximumSize(QSize(9999, 9999));
        tabWidget = new QTabWidget(ConfigDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(18, 10, 461, 461));
        ServerTab = new QWidget();
        ServerTab->setObjectName(QString::fromUtf8("ServerTab"));
        label_2 = new QLabel(ServerTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(18, 30, 81, 16));
        QFont font;
        font.setPointSize(10);
        label_2->setFont(font);
        srvportSpin = new QSpinBox(ServerTab);
        srvportSpin->setObjectName(QString::fromUtf8("srvportSpin"));
        srvportSpin->setGeometry(QRect(141, 30, 151, 21));
        srvportSpin->setMaximum(65535);
        srvconnectSpin = new QSpinBox(ServerTab);
        srvconnectSpin->setObjectName(QString::fromUtf8("srvconnectSpin"));
        srvconnectSpin->setGeometry(QRect(141, 67, 151, 21));
        srvconnectSpin->setMaximum(65535);
        label_7 = new QLabel(ServerTab);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(18, 65, 111, 20));
        label_7->setFont(font);
        browseButton = new QPushButton(ServerTab);
        browseButton->setObjectName(QString::fromUtf8("browseButton"));
        browseButton->setGeometry(QRect(356, 99, 81, 23));
        label_6 = new QLabel(ServerTab);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(18, 106, 91, 16));
        label_6->setFont(font);
        dbpathEdit = new QLineEdit(ServerTab);
        dbpathEdit->setObjectName(QString::fromUtf8("dbpathEdit"));
        dbpathEdit->setGeometry(QRect(140, 100, 211, 20));
        tabWidget->addTab(ServerTab, QString());
        DatabaseTab = new QWidget();
        DatabaseTab->setObjectName(QString::fromUtf8("DatabaseTab"));
        label_3 = new QLabel(DatabaseTab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 61, 81, 16));
        label_3->setFont(font);
        label_4 = new QLabel(DatabaseTab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 95, 81, 16));
        label_4->setFont(font);
        label_5 = new QLabel(DatabaseTab);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 126, 101, 16));
        label_5->setFont(font);
        dbusrEdit = new QLineEdit(DatabaseTab);
        dbusrEdit->setObjectName(QString::fromUtf8("dbusrEdit"));
        dbusrEdit->setGeometry(QRect(130, 61, 301, 20));
        dbpasswdEdit = new QLineEdit(DatabaseTab);
        dbpasswdEdit->setObjectName(QString::fromUtf8("dbpasswdEdit"));
        dbpasswdEdit->setGeometry(QRect(130, 94, 301, 20));
        dbnameEdit = new QLineEdit(DatabaseTab);
        dbnameEdit->setObjectName(QString::fromUtf8("dbnameEdit"));
        dbnameEdit->setGeometry(QRect(130, 127, 301, 20));
        label = new QLabel(DatabaseTab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 30, 101, 16));
        label->setFont(font);
        srvaddrEdit = new QLineEdit(DatabaseTab);
        srvaddrEdit->setObjectName(QString::fromUtf8("srvaddrEdit"));
        srvaddrEdit->setGeometry(QRect(130, 27, 301, 20));
        tabWidget->addTab(DatabaseTab, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        pcTable = new QTableView(tab);
        pcTable->setObjectName(QString::fromUtf8("pcTable"));
        pcTable->setGeometry(QRect(20, 170, 401, 161));
        hardTable = new QTableView(tab);
        hardTable->setObjectName(QString::fromUtf8("hardTable"));
        hardTable->setGeometry(QRect(20, 240, 401, 131));
        addDistItemBtn = new QPushButton(tab);
        addDistItemBtn->setObjectName(QString::fromUtf8("addDistItemBtn"));
        addDistItemBtn->setGeometry(QRect(130, 390, 75, 25));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(addDistItemBtn->sizePolicy().hasHeightForWidth());
        addDistItemBtn->setSizePolicy(sizePolicy);
        addDistItemBtn->setMinimumSize(QSize(75, 25));
        addDistItemBtn->setMaximumSize(QSize(75, 25));
        rmvDistItemBtn = new QPushButton(tab);
        rmvDistItemBtn->setObjectName(QString::fromUtf8("rmvDistItemBtn"));
        rmvDistItemBtn->setGeometry(QRect(250, 390, 75, 25));
        disttypeGroup = new QGroupBox(tab);
        disttypeGroup->setObjectName(QString::fromUtf8("disttypeGroup"));
        disttypeGroup->setGeometry(QRect(20, 10, 401, 50));
        pcRadioBtn = new QRadioButton(disttypeGroup);
        pcRadioBtn->setObjectName(QString::fromUtf8("pcRadioBtn"));
        pcRadioBtn->setGeometry(QRect(20, 20, 91, 18));
        hardRadioBtn = new QRadioButton(disttypeGroup);
        hardRadioBtn->setObjectName(QString::fromUtf8("hardRadioBtn"));
        hardRadioBtn->setGeometry(QRect(160, 20, 91, 18));
        noneRadioBtn = new QRadioButton(disttypeGroup);
        noneRadioBtn->setObjectName(QString::fromUtf8("noneRadioBtn"));
        noneRadioBtn->setGeometry(QRect(280, 20, 84, 18));
        groupDisNum = new QGroupBox(tab);
        groupDisNum->setObjectName(QString::fromUtf8("groupDisNum"));
        groupDisNum->setGeometry(QRect(20, 70, 401, 51));
        spinDistOrgNum = new QSpinBox(groupDisNum);
        spinDistOrgNum->setObjectName(QString::fromUtf8("spinDistOrgNum"));
        spinDistOrgNum->setGeometry(QRect(100, 20, 42, 22));
        spinDistNum = new QSpinBox(groupDisNum);
        spinDistNum->setObjectName(QString::fromUtf8("spinDistNum"));
        spinDistNum->setGeometry(QRect(240, 20, 42, 22));
        labelDistOrgNum = new QLabel(groupDisNum);
        labelDistOrgNum->setObjectName(QString::fromUtf8("labelDistOrgNum"));
        labelDistOrgNum->setGeometry(QRect(20, 23, 71, 16));
        sizePolicy.setHeightForWidth(labelDistOrgNum->sizePolicy().hasHeightForWidth());
        labelDistOrgNum->setSizePolicy(sizePolicy);
        labeDistNum = new QLabel(groupDisNum);
        labeDistNum->setObjectName(QString::fromUtf8("labeDistNum"));
        labeDistNum->setGeometry(QRect(194, 23, 31, 16));
        sizePolicy.setHeightForWidth(labeDistNum->sizePolicy().hasHeightForWidth());
        labeDistNum->setSizePolicy(sizePolicy);
        tabWidget->addTab(tab, QString());
        buttonBox = new QDialogButtonBox(ConfigDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(245, 490, 231, 23));
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        serviceButton = new QPushButton(ConfigDialog);
        serviceButton->setObjectName(QString::fromUtf8("serviceButton"));
        serviceButton->setGeometry(QRect(30, 490, 75, 23));

        retranslateUi(ConfigDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfigDialog)
    {
        ConfigDialog->setWindowTitle(QCoreApplication::translate("ConfigDialog", "Service Options", nullptr));
        label_2->setText(QCoreApplication::translate("ConfigDialog", "Server Port:", nullptr));
        label_7->setText(QCoreApplication::translate("ConfigDialog", "Max Connections:", nullptr));
        browseButton->setText(QCoreApplication::translate("ConfigDialog", "Browse...", nullptr));
        label_6->setText(QCoreApplication::translate("ConfigDialog", "Database path:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(ServerTab), QCoreApplication::translate("ConfigDialog", "General", nullptr));
        label_3->setText(QCoreApplication::translate("ConfigDialog", "User name:", nullptr));
        label_4->setText(QCoreApplication::translate("ConfigDialog", "Password:", nullptr));
        label_5->setText(QCoreApplication::translate("ConfigDialog", "Database name:", nullptr));
        label->setText(QCoreApplication::translate("ConfigDialog", "Server Address:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(DatabaseTab), QCoreApplication::translate("ConfigDialog", "Database", nullptr));
        addDistItemBtn->setText(QCoreApplication::translate("ConfigDialog", "Add", nullptr));
        rmvDistItemBtn->setText(QCoreApplication::translate("ConfigDialog", "Remove", nullptr));
        disttypeGroup->setTitle(QCoreApplication::translate("ConfigDialog", "Distribute Type", nullptr));
        pcRadioBtn->setText(QCoreApplication::translate("ConfigDialog", "PC", nullptr));
        hardRadioBtn->setText(QCoreApplication::translate("ConfigDialog", "Hard", nullptr));
        noneRadioBtn->setText(QCoreApplication::translate("ConfigDialog", "None", nullptr));
        groupDisNum->setTitle(QCoreApplication::translate("ConfigDialog", "Distribute Count", nullptr));
        labelDistOrgNum->setText(QCoreApplication::translate("ConfigDialog", "Original Num", nullptr));
        labeDistNum->setText(QCoreApplication::translate("ConfigDialog", "Count", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("ConfigDialog", "Other", nullptr));
        serviceButton->setText(QCoreApplication::translate("ConfigDialog", "Start Service", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigDialog: public Ui_ConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGDIALOG_H
