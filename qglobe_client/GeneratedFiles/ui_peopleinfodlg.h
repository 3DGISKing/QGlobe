/********************************************************************************
** Form generated from reading UI file 'peopleinfodlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PEOPLEINFODLG_H
#define UI_PEOPLEINFODLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PeopleInfoDlg
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *_4;
    QLabel *lblImage;
    QVBoxLayout *_3;
    QGridLayout *_7;
    QLabel *label_29;
    QLabel *label_14;
    QLabel *label_31;
    QLabel *label_15;
    QLabel *label_32;
    QLabel *label_33;
    QLineEdit *edtName;
    QLineEdit *edtAlias;
    QLineEdit *edtSex;
    QLineEdit *edtBirthday;
    QLineEdit *edtParty;
    QLineEdit *edtNumber;
    QGridLayout *_8;
    QLabel *label_19;
    QLineEdit *edtAddress;
    QLabel *label_20;
    QLineEdit *edtBirthPlace;
    QLabel *label_21;
    QLineEdit *edtPost;
    QGridLayout *_11;
    QLabel *label_42;
    QLabel *label_43;
    QLabel *label_39;
    QLabel *label_44;
    QLineEdit *edtKnowledge;
    QLineEdit *edtStartDate;
    QLineEdit *edtClassOrg;
    QLineEdit *edtSocialOrg;
    QHBoxLayout *_6;
    QPushButton *btnHisFam;
    QPushButton *btnIssue;
    QPushButton *btnGrasp;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnClose;
    QPushButton *btnToBuilding;
    QPushButton *btnToHouse;
    QFrame *frmHisFam;
    QVBoxLayout *_9;
    QTableView *tblHistory;
    QTableView *tblFamily;
    QFrame *frmIssue;
    QGridLayout *gridLayout;
    QHBoxLayout *_10;
    QLineEdit *edtIssueClass1;
    QLabel *label_30;
    QLineEdit *edtIssueClass2;
    QLabel *label_23;
    QLabel *label_25;
    QTextEdit *txtContent;
    QLabel *label_26;
    QLineEdit *edtMisc;
    QFrame *frmGrasp;
    QVBoxLayout *verticalLayout_2;
    QTableView *tblGrasp;

    void setupUi(QDialog *PeopleInfoDlg)
    {
        if (PeopleInfoDlg->objectName().isEmpty())
            PeopleInfoDlg->setObjectName(QString::fromUtf8("PeopleInfoDlg"));
        PeopleInfoDlg->resize(637, 516);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(100);
        sizePolicy.setHeightForWidth(PeopleInfoDlg->sizePolicy().hasHeightForWidth());
        PeopleInfoDlg->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(10);
        PeopleInfoDlg->setFont(font);
        verticalLayout = new QVBoxLayout(PeopleInfoDlg);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        _4 = new QHBoxLayout();
        _4->setSpacing(20);
        _4->setObjectName(QString::fromUtf8("_4"));
        _4->setContentsMargins(10, -1, 10, -1);
        lblImage = new QLabel(PeopleInfoDlg);
        lblImage->setObjectName(QString::fromUtf8("lblImage"));
        lblImage->setMinimumSize(QSize(180, 210));
        lblImage->setMaximumSize(QSize(180, 210));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        lblImage->setPalette(palette);
        lblImage->setAutoFillBackground(true);
        lblImage->setFrameShape(QFrame::StyledPanel);
        lblImage->setAlignment(Qt::AlignCenter);

        _4->addWidget(lblImage);

        _3 = new QVBoxLayout();
        _3->setSpacing(5);
        _3->setObjectName(QString::fromUtf8("_3"));
        _7 = new QGridLayout();
        _7->setSpacing(6);
        _7->setObjectName(QString::fromUtf8("_7"));
        _7->setHorizontalSpacing(15);
        _7->setVerticalSpacing(5);
        label_29 = new QLabel(PeopleInfoDlg);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        _7->addWidget(label_29, 1, 0, 1, 1);

        label_14 = new QLabel(PeopleInfoDlg);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        _7->addWidget(label_14, 1, 2, 1, 1);

        label_31 = new QLabel(PeopleInfoDlg);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setMinimumSize(QSize(70, 0));

        _7->addWidget(label_31, 0, 2, 1, 1);

        label_15 = new QLabel(PeopleInfoDlg);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMinimumSize(QSize(70, 0));

        _7->addWidget(label_15, 0, 0, 1, 1);

        label_32 = new QLabel(PeopleInfoDlg);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        _7->addWidget(label_32, 2, 0, 1, 1);

        label_33 = new QLabel(PeopleInfoDlg);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        _7->addWidget(label_33, 2, 2, 1, 1);

        edtName = new QLineEdit(PeopleInfoDlg);
        edtName->setObjectName(QString::fromUtf8("edtName"));
        edtName->setReadOnly(true);

        _7->addWidget(edtName, 0, 1, 1, 1);

        edtAlias = new QLineEdit(PeopleInfoDlg);
        edtAlias->setObjectName(QString::fromUtf8("edtAlias"));
        edtAlias->setReadOnly(true);

        _7->addWidget(edtAlias, 0, 3, 1, 1);

        edtSex = new QLineEdit(PeopleInfoDlg);
        edtSex->setObjectName(QString::fromUtf8("edtSex"));
        edtSex->setReadOnly(true);

        _7->addWidget(edtSex, 1, 1, 1, 1);

        edtBirthday = new QLineEdit(PeopleInfoDlg);
        edtBirthday->setObjectName(QString::fromUtf8("edtBirthday"));
        edtBirthday->setReadOnly(true);

        _7->addWidget(edtBirthday, 1, 3, 1, 1);

        edtParty = new QLineEdit(PeopleInfoDlg);
        edtParty->setObjectName(QString::fromUtf8("edtParty"));
        edtParty->setReadOnly(true);

        _7->addWidget(edtParty, 2, 1, 1, 1);

        edtNumber = new QLineEdit(PeopleInfoDlg);
        edtNumber->setObjectName(QString::fromUtf8("edtNumber"));
        edtNumber->setReadOnly(true);

        _7->addWidget(edtNumber, 2, 3, 1, 1);


        _3->addLayout(_7);

        _8 = new QGridLayout();
        _8->setSpacing(6);
        _8->setObjectName(QString::fromUtf8("_8"));
        _8->setHorizontalSpacing(15);
        _8->setVerticalSpacing(5);
        label_19 = new QLabel(PeopleInfoDlg);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        _8->addWidget(label_19, 1, 0, 1, 1);

        edtAddress = new QLineEdit(PeopleInfoDlg);
        edtAddress->setObjectName(QString::fromUtf8("edtAddress"));
        edtAddress->setReadOnly(true);

        _8->addWidget(edtAddress, 1, 1, 1, 1);

        label_20 = new QLabel(PeopleInfoDlg);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setMinimumSize(QSize(70, 0));

        _8->addWidget(label_20, 0, 0, 1, 1);

        edtBirthPlace = new QLineEdit(PeopleInfoDlg);
        edtBirthPlace->setObjectName(QString::fromUtf8("edtBirthPlace"));
        edtBirthPlace->setReadOnly(true);

        _8->addWidget(edtBirthPlace, 0, 1, 1, 1);

        label_21 = new QLabel(PeopleInfoDlg);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        _8->addWidget(label_21, 2, 0, 1, 1);

        edtPost = new QLineEdit(PeopleInfoDlg);
        edtPost->setObjectName(QString::fromUtf8("edtPost"));
        edtPost->setReadOnly(true);

        _8->addWidget(edtPost, 2, 1, 1, 1);


        _3->addLayout(_8);

        _11 = new QGridLayout();
        _11->setSpacing(6);
        _11->setObjectName(QString::fromUtf8("_11"));
        _11->setHorizontalSpacing(15);
        _11->setVerticalSpacing(5);
        label_42 = new QLabel(PeopleInfoDlg);
        label_42->setObjectName(QString::fromUtf8("label_42"));

        _11->addWidget(label_42, 1, 2, 1, 1);

        label_43 = new QLabel(PeopleInfoDlg);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        label_43->setMinimumSize(QSize(70, 0));

        _11->addWidget(label_43, 0, 0, 1, 1);

        label_39 = new QLabel(PeopleInfoDlg);
        label_39->setObjectName(QString::fromUtf8("label_39"));

        _11->addWidget(label_39, 1, 0, 1, 1);

        label_44 = new QLabel(PeopleInfoDlg);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setMinimumSize(QSize(70, 0));

        _11->addWidget(label_44, 0, 2, 1, 1);

        edtKnowledge = new QLineEdit(PeopleInfoDlg);
        edtKnowledge->setObjectName(QString::fromUtf8("edtKnowledge"));
        edtKnowledge->setReadOnly(true);

        _11->addWidget(edtKnowledge, 0, 1, 1, 1);

        edtStartDate = new QLineEdit(PeopleInfoDlg);
        edtStartDate->setObjectName(QString::fromUtf8("edtStartDate"));
        edtStartDate->setReadOnly(true);

        _11->addWidget(edtStartDate, 0, 3, 1, 1);

        edtClassOrg = new QLineEdit(PeopleInfoDlg);
        edtClassOrg->setObjectName(QString::fromUtf8("edtClassOrg"));
        edtClassOrg->setReadOnly(true);

        _11->addWidget(edtClassOrg, 1, 1, 1, 1);

        edtSocialOrg = new QLineEdit(PeopleInfoDlg);
        edtSocialOrg->setObjectName(QString::fromUtf8("edtSocialOrg"));
        edtSocialOrg->setReadOnly(true);

        _11->addWidget(edtSocialOrg, 1, 3, 1, 1);


        _3->addLayout(_11);


        _4->addLayout(_3);


        verticalLayout->addLayout(_4);

        _6 = new QHBoxLayout();
        _6->setSpacing(10);
        _6->setObjectName(QString::fromUtf8("_6"));
        _6->setSizeConstraint(QLayout::SetNoConstraint);
        _6->setContentsMargins(10, -1, 10, -1);
        btnHisFam = new QPushButton(PeopleInfoDlg);
        btnHisFam->setObjectName(QString::fromUtf8("btnHisFam"));
        btnHisFam->setCheckable(true);
        btnHisFam->setChecked(true);

        _6->addWidget(btnHisFam);

        btnIssue = new QPushButton(PeopleInfoDlg);
        btnIssue->setObjectName(QString::fromUtf8("btnIssue"));
        btnIssue->setCheckable(true);

        _6->addWidget(btnIssue);

        btnGrasp = new QPushButton(PeopleInfoDlg);
        btnGrasp->setObjectName(QString::fromUtf8("btnGrasp"));
        btnGrasp->setCheckable(true);

        _6->addWidget(btnGrasp);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        _6->addItem(horizontalSpacer);

        btnClose = new QPushButton(PeopleInfoDlg);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));

        _6->addWidget(btnClose);

        btnToBuilding = new QPushButton(PeopleInfoDlg);
        btnToBuilding->setObjectName(QString::fromUtf8("btnToBuilding"));
        btnToBuilding->setEnabled(false);

        _6->addWidget(btnToBuilding);

        btnToHouse = new QPushButton(PeopleInfoDlg);
        btnToHouse->setObjectName(QString::fromUtf8("btnToHouse"));
        btnToHouse->setEnabled(false);

        _6->addWidget(btnToHouse);


        verticalLayout->addLayout(_6);

        frmHisFam = new QFrame(PeopleInfoDlg);
        frmHisFam->setObjectName(QString::fromUtf8("frmHisFam"));
        frmHisFam->setFrameShape(QFrame::StyledPanel);
        frmHisFam->setFrameShadow(QFrame::Raised);
        _9 = new QVBoxLayout(frmHisFam);
        _9->setSpacing(5);
        _9->setContentsMargins(0, 0, 0, 0);
        _9->setObjectName(QString::fromUtf8("_9"));
        tblHistory = new QTableView(frmHisFam);
        tblHistory->setObjectName(QString::fromUtf8("tblHistory"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tblHistory->sizePolicy().hasHeightForWidth());
        tblHistory->setSizePolicy(sizePolicy1);
        tblHistory->setFont(font);
        tblHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblHistory->setSelectionMode(QAbstractItemView::SingleSelection);
        tblHistory->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblHistory->horizontalHeader()->setCascadingSectionResizes(true);
        tblHistory->horizontalHeader()->setDefaultSectionSize(60);
        tblHistory->horizontalHeader()->setStretchLastSection(true);
        tblHistory->verticalHeader()->setVisible(false);
        tblHistory->verticalHeader()->setDefaultSectionSize(24);

        _9->addWidget(tblHistory);

        tblFamily = new QTableView(frmHisFam);
        tblFamily->setObjectName(QString::fromUtf8("tblFamily"));
        sizePolicy1.setHeightForWidth(tblFamily->sizePolicy().hasHeightForWidth());
        tblFamily->setSizePolicy(sizePolicy1);
        tblFamily->setFont(font);
        tblFamily->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblFamily->setSelectionMode(QAbstractItemView::SingleSelection);
        tblFamily->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblFamily->horizontalHeader()->setCascadingSectionResizes(true);
        tblFamily->horizontalHeader()->setDefaultSectionSize(60);
        tblFamily->horizontalHeader()->setStretchLastSection(true);
        tblFamily->verticalHeader()->setVisible(false);
        tblFamily->verticalHeader()->setDefaultSectionSize(24);

        _9->addWidget(tblFamily);


        verticalLayout->addWidget(frmHisFam);

        frmIssue = new QFrame(PeopleInfoDlg);
        frmIssue->setObjectName(QString::fromUtf8("frmIssue"));
        frmIssue->setFrameShape(QFrame::StyledPanel);
        frmIssue->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frmIssue);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(15);
        gridLayout->setVerticalSpacing(10);
        _10 = new QHBoxLayout();
        _10->setSpacing(10);
        _10->setObjectName(QString::fromUtf8("_10"));
        edtIssueClass1 = new QLineEdit(frmIssue);
        edtIssueClass1->setObjectName(QString::fromUtf8("edtIssueClass1"));
        edtIssueClass1->setReadOnly(true);

        _10->addWidget(edtIssueClass1);

        label_30 = new QLabel(frmIssue);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setMinimumSize(QSize(70, 0));

        _10->addWidget(label_30);

        edtIssueClass2 = new QLineEdit(frmIssue);
        edtIssueClass2->setObjectName(QString::fromUtf8("edtIssueClass2"));
        edtIssueClass2->setReadOnly(true);

        _10->addWidget(edtIssueClass2);


        gridLayout->addLayout(_10, 0, 1, 1, 1);

        label_23 = new QLabel(frmIssue);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout->addWidget(label_23, 1, 0, 1, 1);

        label_25 = new QLabel(frmIssue);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout->addWidget(label_25, 0, 0, 1, 1);

        txtContent = new QTextEdit(frmIssue);
        txtContent->setObjectName(QString::fromUtf8("txtContent"));
        txtContent->setReadOnly(true);

        gridLayout->addWidget(txtContent, 1, 1, 1, 1);

        label_26 = new QLabel(frmIssue);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout->addWidget(label_26, 2, 0, 1, 1);

        edtMisc = new QLineEdit(frmIssue);
        edtMisc->setObjectName(QString::fromUtf8("edtMisc"));
        edtMisc->setReadOnly(true);

        gridLayout->addWidget(edtMisc, 2, 1, 1, 1);


        verticalLayout->addWidget(frmIssue);

        frmGrasp = new QFrame(PeopleInfoDlg);
        frmGrasp->setObjectName(QString::fromUtf8("frmGrasp"));
        frmGrasp->setFrameShape(QFrame::StyledPanel);
        frmGrasp->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frmGrasp);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tblGrasp = new QTableView(frmGrasp);
        tblGrasp->setObjectName(QString::fromUtf8("tblGrasp"));
        sizePolicy1.setHeightForWidth(tblGrasp->sizePolicy().hasHeightForWidth());
        tblGrasp->setSizePolicy(sizePolicy1);
        tblGrasp->setFont(font);
        tblGrasp->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblGrasp->setSelectionMode(QAbstractItemView::NoSelection);
        tblGrasp->horizontalHeader()->setCascadingSectionResizes(true);
        tblGrasp->horizontalHeader()->setDefaultSectionSize(60);
        tblGrasp->horizontalHeader()->setStretchLastSection(true);
        tblGrasp->verticalHeader()->setVisible(false);
        tblGrasp->verticalHeader()->setDefaultSectionSize(50);

        verticalLayout_2->addWidget(tblGrasp);


        verticalLayout->addWidget(frmGrasp);

        QWidget::setTabOrder(edtName, edtAlias);
        QWidget::setTabOrder(edtAlias, edtSex);
        QWidget::setTabOrder(edtSex, edtBirthday);
        QWidget::setTabOrder(edtBirthday, edtParty);
        QWidget::setTabOrder(edtParty, edtNumber);
        QWidget::setTabOrder(edtNumber, edtBirthPlace);
        QWidget::setTabOrder(edtBirthPlace, edtAddress);
        QWidget::setTabOrder(edtAddress, edtPost);
        QWidget::setTabOrder(edtPost, edtKnowledge);
        QWidget::setTabOrder(edtKnowledge, edtStartDate);
        QWidget::setTabOrder(edtStartDate, edtClassOrg);
        QWidget::setTabOrder(edtClassOrg, edtSocialOrg);
        QWidget::setTabOrder(edtSocialOrg, tblHistory);
        QWidget::setTabOrder(tblHistory, tblFamily);
        QWidget::setTabOrder(tblFamily, btnHisFam);
        QWidget::setTabOrder(btnHisFam, edtIssueClass1);
        QWidget::setTabOrder(edtIssueClass1, edtIssueClass2);
        QWidget::setTabOrder(edtIssueClass2, txtContent);
        QWidget::setTabOrder(txtContent, edtMisc);
        QWidget::setTabOrder(edtMisc, btnIssue);
        QWidget::setTabOrder(btnIssue, tblGrasp);
        QWidget::setTabOrder(tblGrasp, btnGrasp);

        retranslateUi(PeopleInfoDlg);

        QMetaObject::connectSlotsByName(PeopleInfoDlg);
    } // setupUi

    void retranslateUi(QDialog *PeopleInfoDlg)
    {
        PeopleInfoDlg->setWindowTitle(QCoreApplication::translate("PeopleInfoDlg", "PeopleInfoDlg", nullptr));
        label_29->setText(QCoreApplication::translate("PeopleInfoDlg", "sex", nullptr));
        label_14->setText(QCoreApplication::translate("PeopleInfoDlg", "birthday", nullptr));
        label_31->setText(QCoreApplication::translate("PeopleInfoDlg", "alias", nullptr));
        label_15->setText(QCoreApplication::translate("PeopleInfoDlg", "name", nullptr));
        label_32->setText(QCoreApplication::translate("PeopleInfoDlg", "party", nullptr));
        label_33->setText(QCoreApplication::translate("PeopleInfoDlg", "number", nullptr));
        label_19->setText(QCoreApplication::translate("PeopleInfoDlg", "address", nullptr));
        label_20->setText(QCoreApplication::translate("PeopleInfoDlg", "birthplace", nullptr));
        label_21->setText(QCoreApplication::translate("PeopleInfoDlg", "post", nullptr));
        label_42->setText(QCoreApplication::translate("PeopleInfoDlg", "socialOrg", nullptr));
        label_43->setText(QCoreApplication::translate("PeopleInfoDlg", "knowledge", nullptr));
        label_39->setText(QCoreApplication::translate("PeopleInfoDlg", "classOrg", nullptr));
        label_44->setText(QCoreApplication::translate("PeopleInfoDlg", "startdate", nullptr));
        btnHisFam->setText(QCoreApplication::translate("PeopleInfoDlg", "HisFam", nullptr));
        btnIssue->setText(QCoreApplication::translate("PeopleInfoDlg", "Issue", nullptr));
        btnGrasp->setText(QCoreApplication::translate("PeopleInfoDlg", "Grasp", nullptr));
        btnClose->setText(QCoreApplication::translate("PeopleInfoDlg", "Close", nullptr));
        btnToBuilding->setText(QCoreApplication::translate("PeopleInfoDlg", "ToBuilding", nullptr));
        btnToHouse->setText(QCoreApplication::translate("PeopleInfoDlg", "ToHouse", nullptr));
        label_30->setText(QCoreApplication::translate("PeopleInfoDlg", "issueclass2", nullptr));
        label_23->setText(QCoreApplication::translate("PeopleInfoDlg", "content", nullptr));
        label_25->setText(QCoreApplication::translate("PeopleInfoDlg", "issueclass1", nullptr));
        label_26->setText(QCoreApplication::translate("PeopleInfoDlg", "misc", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PeopleInfoDlg: public Ui_PeopleInfoDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PEOPLEINFODLG_H
