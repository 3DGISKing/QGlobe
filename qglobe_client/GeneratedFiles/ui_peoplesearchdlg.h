/********************************************************************************
** Form generated from reading UI file 'peoplesearchdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PEOPLESEARCHDLG_H
#define UI_PEOPLESEARCHDLG_H

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

class Ui_PeopleSearchDlg
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QLineEdit *edtAddress;
    QLabel *label_9;
    QLabel *label;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *cmbParty;
    QLabel *label_7;
    QComboBox *cmbKnowledge;
    QLabel *label_10;
    QLabel *label_3;
    QLabel *label_8;
    QLabel *label_6;
    QLineEdit *edtBirthPlace;
    QHBoxLayout *horizontalLayout;
    QLineEdit *edtName;
    QLabel *label_2;
    QComboBox *cmbSex;
    QLineEdit *edtPost;
    QHBoxLayout *horizontalLayout_2;
    QSpinBox *spnAgeFrom;
    QLabel *label_4;
    QSpinBox *spnAgeTo;
    QLabel *label_5;
    QLabel *label_11;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *cmbSocialOrg;
    QSpacerItem *horizontalSpacer;
    QLabel *label_13;
    QHBoxLayout *horizontalLayout_5;
    QComboBox *cmbIssueClass1;
    QComboBox *cmbIssueClass2;
    QHBoxLayout *_6;
    QSpacerItem *spacer;
    QPushButton *btnClose;
    QPushButton *btnClear;
    QPushButton *btnSearch;
    QTableView *tblResult;

    void setupUi(QDialog *PeopleSearchDlg)
    {
        if (PeopleSearchDlg->objectName().isEmpty())
            PeopleSearchDlg->setObjectName(QString::fromUtf8("PeopleSearchDlg"));
        PeopleSearchDlg->resize(650, 550);
        QFont font;
        font.setPointSize(10);
        PeopleSearchDlg->setFont(font);
        verticalLayout = new QVBoxLayout(PeopleSearchDlg);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(PeopleSearchDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 130));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(10);
        edtAddress = new QLineEdit(groupBox);
        edtAddress->setObjectName(QString::fromUtf8("edtAddress"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(edtAddress->sizePolicy().hasHeightForWidth());
        edtAddress->setSizePolicy(sizePolicy);

        gridLayout->addWidget(edtAddress, 2, 1, 1, 1);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_9, 2, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(70, 0));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(10);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        cmbParty = new QComboBox(groupBox);
        cmbParty->setObjectName(QString::fromUtf8("cmbParty"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(cmbParty->sizePolicy().hasHeightForWidth());
        cmbParty->setSizePolicy(sizePolicy2);
        cmbParty->setMinimumSize(QSize(90, 0));
        cmbParty->setMaximumSize(QSize(90, 16777215));

        horizontalLayout_3->addWidget(cmbParty);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_3->addWidget(label_7);

        cmbKnowledge = new QComboBox(groupBox);
        cmbKnowledge->setObjectName(QString::fromUtf8("cmbKnowledge"));
        sizePolicy2.setHeightForWidth(cmbKnowledge->sizePolicy().hasHeightForWidth());
        cmbKnowledge->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(cmbKnowledge);


        gridLayout->addLayout(horizontalLayout_3, 1, 1, 1, 1);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        sizePolicy1.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_10, 2, 2, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMinimumSize(QSize(70, 0));

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy1.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_8, 1, 2, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_6, 1, 0, 1, 1);

        edtBirthPlace = new QLineEdit(groupBox);
        edtBirthPlace->setObjectName(QString::fromUtf8("edtBirthPlace"));
        sizePolicy2.setHeightForWidth(edtBirthPlace->sizePolicy().hasHeightForWidth());
        edtBirthPlace->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(edtBirthPlace, 1, 3, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        edtName = new QLineEdit(groupBox);
        edtName->setObjectName(QString::fromUtf8("edtName"));
        sizePolicy2.setHeightForWidth(edtName->sizePolicy().hasHeightForWidth());
        edtName->setSizePolicy(sizePolicy2);
        edtName->setMinimumSize(QSize(90, 0));
        edtName->setMaximumSize(QSize(90, 16777215));

        horizontalLayout->addWidget(edtName);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(label_2);

        cmbSex = new QComboBox(groupBox);
        cmbSex->setObjectName(QString::fromUtf8("cmbSex"));
        sizePolicy2.setHeightForWidth(cmbSex->sizePolicy().hasHeightForWidth());
        cmbSex->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(cmbSex);


        gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);

        edtPost = new QLineEdit(groupBox);
        edtPost->setObjectName(QString::fromUtf8("edtPost"));
        sizePolicy.setHeightForWidth(edtPost->sizePolicy().hasHeightForWidth());
        edtPost->setSizePolicy(sizePolicy);

        gridLayout->addWidget(edtPost, 2, 3, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        spnAgeFrom = new QSpinBox(groupBox);
        spnAgeFrom->setObjectName(QString::fromUtf8("spnAgeFrom"));
        sizePolicy2.setHeightForWidth(spnAgeFrom->sizePolicy().hasHeightForWidth());
        spnAgeFrom->setSizePolicy(sizePolicy2);
        spnAgeFrom->setMaximum(100);

        horizontalLayout_2->addWidget(spnAgeFrom);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(label_4);

        spnAgeTo = new QSpinBox(groupBox);
        spnAgeTo->setObjectName(QString::fromUtf8("spnAgeTo"));
        sizePolicy2.setHeightForWidth(spnAgeTo->sizePolicy().hasHeightForWidth());
        spnAgeTo->setSizePolicy(sizePolicy2);
        spnAgeTo->setMaximum(150);

        horizontalLayout_2->addWidget(spnAgeTo);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy1.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(label_5);


        gridLayout->addLayout(horizontalLayout_2, 0, 3, 1, 1);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy1.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_11, 3, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        cmbSocialOrg = new QComboBox(groupBox);
        cmbSocialOrg->setObjectName(QString::fromUtf8("cmbSocialOrg"));
        sizePolicy2.setHeightForWidth(cmbSocialOrg->sizePolicy().hasHeightForWidth());
        cmbSocialOrg->setSizePolicy(sizePolicy2);
        cmbSocialOrg->setMinimumSize(QSize(90, 0));
        cmbSocialOrg->setMaximumSize(QSize(90, 16777215));

        horizontalLayout_4->addWidget(cmbSocialOrg);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout_4, 3, 1, 1, 1);

        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        sizePolicy1.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_13, 3, 2, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(15);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        cmbIssueClass1 = new QComboBox(groupBox);
        cmbIssueClass1->setObjectName(QString::fromUtf8("cmbIssueClass1"));
        sizePolicy2.setHeightForWidth(cmbIssueClass1->sizePolicy().hasHeightForWidth());
        cmbIssueClass1->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(cmbIssueClass1);

        cmbIssueClass2 = new QComboBox(groupBox);
        cmbIssueClass2->setObjectName(QString::fromUtf8("cmbIssueClass2"));
        sizePolicy2.setHeightForWidth(cmbIssueClass2->sizePolicy().hasHeightForWidth());
        cmbIssueClass2->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(cmbIssueClass2);


        gridLayout->addLayout(horizontalLayout_5, 3, 3, 1, 1);


        verticalLayout_2->addLayout(gridLayout);


        verticalLayout->addWidget(groupBox);

        _6 = new QHBoxLayout();
        _6->setSpacing(10);
        _6->setObjectName(QString::fromUtf8("_6"));
        _6->setSizeConstraint(QLayout::SetNoConstraint);
        _6->setContentsMargins(10, -1, 10, -1);
        spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        _6->addItem(spacer);

        btnClose = new QPushButton(PeopleSearchDlg);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));

        _6->addWidget(btnClose);

        btnClear = new QPushButton(PeopleSearchDlg);
        btnClear->setObjectName(QString::fromUtf8("btnClear"));

        _6->addWidget(btnClear);

        btnSearch = new QPushButton(PeopleSearchDlg);
        btnSearch->setObjectName(QString::fromUtf8("btnSearch"));

        _6->addWidget(btnSearch);


        verticalLayout->addLayout(_6);

        tblResult = new QTableView(PeopleSearchDlg);
        tblResult->setObjectName(QString::fromUtf8("tblResult"));
        tblResult->setFont(font);
        tblResult->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblResult->setSelectionMode(QAbstractItemView::SingleSelection);
        tblResult->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblResult->horizontalHeader()->setCascadingSectionResizes(true);
        tblResult->verticalHeader()->setCascadingSectionResizes(true);

        verticalLayout->addWidget(tblResult);

        QWidget::setTabOrder(edtName, cmbSex);
        QWidget::setTabOrder(cmbSex, spnAgeFrom);
        QWidget::setTabOrder(spnAgeFrom, spnAgeTo);
        QWidget::setTabOrder(spnAgeTo, cmbParty);
        QWidget::setTabOrder(cmbParty, cmbKnowledge);
        QWidget::setTabOrder(cmbKnowledge, edtBirthPlace);
        QWidget::setTabOrder(edtBirthPlace, edtAddress);
        QWidget::setTabOrder(edtAddress, edtPost);
        QWidget::setTabOrder(edtPost, cmbSocialOrg);
        QWidget::setTabOrder(cmbSocialOrg, cmbIssueClass1);
        QWidget::setTabOrder(cmbIssueClass1, cmbIssueClass2);
        QWidget::setTabOrder(cmbIssueClass2, btnSearch);
        QWidget::setTabOrder(btnSearch, btnClear);
        QWidget::setTabOrder(btnClear, btnClose);
        QWidget::setTabOrder(btnClose, tblResult);

        retranslateUi(PeopleSearchDlg);

        QMetaObject::connectSlotsByName(PeopleSearchDlg);
    } // setupUi

    void retranslateUi(QDialog *PeopleSearchDlg)
    {
        PeopleSearchDlg->setWindowTitle(QCoreApplication::translate("PeopleSearchDlg", "PeopleSearchDlg", nullptr));
        groupBox->setTitle(QString());
        label_9->setText(QCoreApplication::translate("PeopleSearchDlg", "address", nullptr));
        label->setText(QCoreApplication::translate("PeopleSearchDlg", "name", nullptr));
        label_7->setText(QCoreApplication::translate("PeopleSearchDlg", "knowledge", nullptr));
        label_10->setText(QCoreApplication::translate("PeopleSearchDlg", "post", nullptr));
        label_3->setText(QCoreApplication::translate("PeopleSearchDlg", "age", nullptr));
        label_8->setText(QCoreApplication::translate("PeopleSearchDlg", "birthplace", nullptr));
        label_6->setText(QCoreApplication::translate("PeopleSearchDlg", "party", nullptr));
        label_2->setText(QCoreApplication::translate("PeopleSearchDlg", "sex", nullptr));
        label_4->setText(QCoreApplication::translate("PeopleSearchDlg", "from", nullptr));
        label_5->setText(QCoreApplication::translate("PeopleSearchDlg", "to", nullptr));
        label_11->setText(QCoreApplication::translate("PeopleSearchDlg", "socialOrg", nullptr));
        label_13->setText(QCoreApplication::translate("PeopleSearchDlg", "issue", nullptr));
        btnClose->setText(QCoreApplication::translate("PeopleSearchDlg", "Close", nullptr));
        btnClear->setText(QCoreApplication::translate("PeopleSearchDlg", "Clear", nullptr));
        btnSearch->setText(QCoreApplication::translate("PeopleSearchDlg", "Search", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PeopleSearchDlg: public Ui_PeopleSearchDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PEOPLESEARCHDLG_H
