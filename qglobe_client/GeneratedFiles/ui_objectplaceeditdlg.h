/********************************************************************************
** Form generated from reading UI file 'objectplaceeditdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBJECTPLACEEDITDLG_H
#define UI_OBJECTPLACEEDITDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ObjectPlaceEditDlg
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QComboBox *cmbCategory;
    QLabel *lblObjectPlace;
    QComboBox *cmbObjectPlace;
    QTabWidget *tabs_ShapeProperty;
    QWidget *tabPlaceMark;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout_3;
    QLabel *label_8;
    QLineEdit *edtName_ko;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *btnCenterPoint;
    QGridLayout *gridLayout_4;
    QLabel *label_10;
    QLineEdit *edtLongitude;
    QLabel *label_9;
    QLineEdit *edtLatitude;
    QLabel *label_6;
    QLineEdit *edtName;
    QLabel *label_15;
    QHBoxLayout *horizontalLayout_8;
    QLineEdit *edtHtmlPath;
    QPushButton *btnHtmlPath;
    QHBoxLayout *horizontalLayout_9;
    QLineEdit *edtVideoPath;
    QPushButton *btnVideoPath;
    QLabel *label_16;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnModify;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *btnNewAdd;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *btnDelete;
    QWidget *tabPointList;
    QVBoxLayout *verticalLayout_3;
    QTableView *pListTableView;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *addButton;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *editButton;
    QSpacerItem *horizontalSpacer_11;
    QPushButton *deleteButton;
    QWidget *tabProperty;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *lblLat;
    QLabel *lblLong;
    QLabel *lblName;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnSave;
    QPushButton *btnCancel;
    QPushButton *btnExit;

    void setupUi(QDialog *ObjectPlaceEditDlg)
    {
        if (ObjectPlaceEditDlg->objectName().isEmpty())
            ObjectPlaceEditDlg->setObjectName(QString::fromUtf8("ObjectPlaceEditDlg"));
        ObjectPlaceEditDlg->resize(379, 352);
        QFont font;
        font.setPointSize(10);
        ObjectPlaceEditDlg->setFont(font);
        verticalLayout = new QVBoxLayout(ObjectPlaceEditDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_2 = new QLabel(ObjectPlaceEditDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setScaledContents(true);

        horizontalLayout_4->addWidget(label_2);

        cmbCategory = new QComboBox(ObjectPlaceEditDlg);
        cmbCategory->setObjectName(QString::fromUtf8("cmbCategory"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cmbCategory->sizePolicy().hasHeightForWidth());
        cmbCategory->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(cmbCategory);

        lblObjectPlace = new QLabel(ObjectPlaceEditDlg);
        lblObjectPlace->setObjectName(QString::fromUtf8("lblObjectPlace"));
        sizePolicy.setHeightForWidth(lblObjectPlace->sizePolicy().hasHeightForWidth());
        lblObjectPlace->setSizePolicy(sizePolicy);
        lblObjectPlace->setScaledContents(true);

        horizontalLayout_4->addWidget(lblObjectPlace);

        cmbObjectPlace = new QComboBox(ObjectPlaceEditDlg);
        cmbObjectPlace->setObjectName(QString::fromUtf8("cmbObjectPlace"));

        horizontalLayout_4->addWidget(cmbObjectPlace);


        verticalLayout->addLayout(horizontalLayout_4);

        tabs_ShapeProperty = new QTabWidget(ObjectPlaceEditDlg);
        tabs_ShapeProperty->setObjectName(QString::fromUtf8("tabs_ShapeProperty"));
        tabPlaceMark = new QWidget();
        tabPlaceMark->setObjectName(QString::fromUtf8("tabPlaceMark"));
        verticalLayout_2 = new QVBoxLayout(tabPlaceMark);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(10);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_8 = new QLabel(tabPlaceMark);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_3->addWidget(label_8, 1, 0, 1, 1);

        edtName_ko = new QLineEdit(tabPlaceMark);
        edtName_ko->setObjectName(QString::fromUtf8("edtName_ko"));

        gridLayout_3->addWidget(edtName_ko, 0, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        btnCenterPoint = new QPushButton(tabPlaceMark);
        btnCenterPoint->setObjectName(QString::fromUtf8("btnCenterPoint"));
        btnCenterPoint->setFocusPolicy(Qt::StrongFocus);
        btnCenterPoint->setCheckable(true);
        btnCenterPoint->setAutoDefault(false);

        horizontalLayout_5->addWidget(btnCenterPoint);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setHorizontalSpacing(5);
        gridLayout_4->setVerticalSpacing(10);
        label_10 = new QLabel(tabPlaceMark);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_4->addWidget(label_10, 1, 1, 1, 1);

        edtLongitude = new QLineEdit(tabPlaceMark);
        edtLongitude->setObjectName(QString::fromUtf8("edtLongitude"));

        gridLayout_4->addWidget(edtLongitude, 1, 2, 1, 1);

        label_9 = new QLabel(tabPlaceMark);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_4->addWidget(label_9, 0, 1, 1, 1);

        edtLatitude = new QLineEdit(tabPlaceMark);
        edtLatitude->setObjectName(QString::fromUtf8("edtLatitude"));

        gridLayout_4->addWidget(edtLatitude, 0, 2, 1, 1);


        horizontalLayout_5->addLayout(gridLayout_4);


        gridLayout_3->addLayout(horizontalLayout_5, 2, 1, 1, 1);

        label_6 = new QLabel(tabPlaceMark);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_3->addWidget(label_6, 0, 0, 1, 1);

        edtName = new QLineEdit(tabPlaceMark);
        edtName->setObjectName(QString::fromUtf8("edtName"));

        gridLayout_3->addWidget(edtName, 1, 1, 1, 1);

        label_15 = new QLabel(tabPlaceMark);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_3->addWidget(label_15, 3, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        edtHtmlPath = new QLineEdit(tabPlaceMark);
        edtHtmlPath->setObjectName(QString::fromUtf8("edtHtmlPath"));
        edtHtmlPath->setReadOnly(true);

        horizontalLayout_8->addWidget(edtHtmlPath);

        btnHtmlPath = new QPushButton(tabPlaceMark);
        btnHtmlPath->setObjectName(QString::fromUtf8("btnHtmlPath"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(btnHtmlPath->sizePolicy().hasHeightForWidth());
        btnHtmlPath->setSizePolicy(sizePolicy2);
        btnHtmlPath->setMaximumSize(QSize(25, 22));

        horizontalLayout_8->addWidget(btnHtmlPath);


        gridLayout_3->addLayout(horizontalLayout_8, 3, 1, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        edtVideoPath = new QLineEdit(tabPlaceMark);
        edtVideoPath->setObjectName(QString::fromUtf8("edtVideoPath"));
        edtVideoPath->setReadOnly(true);

        horizontalLayout_9->addWidget(edtVideoPath);

        btnVideoPath = new QPushButton(tabPlaceMark);
        btnVideoPath->setObjectName(QString::fromUtf8("btnVideoPath"));
        sizePolicy2.setHeightForWidth(btnVideoPath->sizePolicy().hasHeightForWidth());
        btnVideoPath->setSizePolicy(sizePolicy2);
        btnVideoPath->setMaximumSize(QSize(25, 22));

        horizontalLayout_9->addWidget(btnVideoPath);


        gridLayout_3->addLayout(horizontalLayout_9, 4, 1, 1, 1);

        label_16 = new QLabel(tabPlaceMark);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_3->addWidget(label_16, 4, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        btnModify = new QPushButton(tabPlaceMark);
        btnModify->setObjectName(QString::fromUtf8("btnModify"));
        btnModify->setFocusPolicy(Qt::StrongFocus);
        btnModify->setAutoDefault(false);

        horizontalLayout_3->addWidget(btnModify);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        btnNewAdd = new QPushButton(tabPlaceMark);
        btnNewAdd->setObjectName(QString::fromUtf8("btnNewAdd"));
        btnNewAdd->setFocusPolicy(Qt::StrongFocus);
        btnNewAdd->setAutoDefault(false);

        horizontalLayout_3->addWidget(btnNewAdd);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_8);

        btnDelete = new QPushButton(tabPlaceMark);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));
        btnDelete->setFocusPolicy(Qt::StrongFocus);
        btnDelete->setAutoDefault(false);

        horizontalLayout_3->addWidget(btnDelete);


        verticalLayout_2->addLayout(horizontalLayout_3);

        tabs_ShapeProperty->addTab(tabPlaceMark, QString());
        tabPointList = new QWidget();
        tabPointList->setObjectName(QString::fromUtf8("tabPointList"));
        verticalLayout_3 = new QVBoxLayout(tabPointList);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        pListTableView = new QTableView(tabPointList);
        pListTableView->setObjectName(QString::fromUtf8("pListTableView"));
        pListTableView->setEditTriggers(QAbstractItemView::DoubleClicked);
        pListTableView->setSelectionMode(QAbstractItemView::SingleSelection);
        pListTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        pListTableView->horizontalHeader()->setStretchLastSection(true);
        pListTableView->verticalHeader()->setDefaultSectionSize(18);

        verticalLayout_3->addWidget(pListTableView);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        addButton = new QPushButton(tabPointList);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        addButton->setFocusPolicy(Qt::StrongFocus);
        addButton->setAutoDefault(false);

        horizontalLayout_10->addWidget(addButton);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_10);

        editButton = new QPushButton(tabPointList);
        editButton->setObjectName(QString::fromUtf8("editButton"));
        editButton->setFocusPolicy(Qt::StrongFocus);
        editButton->setAutoDefault(false);

        horizontalLayout_10->addWidget(editButton);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_11);

        deleteButton = new QPushButton(tabPointList);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));
        deleteButton->setFocusPolicy(Qt::StrongFocus);
        deleteButton->setAutoDefault(false);

        horizontalLayout_10->addWidget(deleteButton);


        verticalLayout_3->addLayout(horizontalLayout_10);

        tabs_ShapeProperty->addTab(tabPointList, QString());
        tabProperty = new QWidget();
        tabProperty->setObjectName(QString::fromUtf8("tabProperty"));
        label = new QLabel(tabProperty);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(9, 9, 111, 21));
        label_3 = new QLabel(tabProperty);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 40, 41, 21));
        label_4 = new QLabel(tabProperty);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(190, 40, 41, 21));
        lblLat = new QLabel(tabProperty);
        lblLat->setObjectName(QString::fromUtf8("lblLat"));
        lblLat->setGeometry(QRect(70, 40, 91, 21));
        lblLat->setFrameShape(QFrame::Panel);
        lblLat->setFrameShadow(QFrame::Sunken);
        lblLong = new QLabel(tabProperty);
        lblLong->setObjectName(QString::fromUtf8("lblLong"));
        lblLong->setGeometry(QRect(240, 40, 91, 21));
        lblLong->setFrameShape(QFrame::Panel);
        lblLong->setFrameShadow(QFrame::Sunken);
        lblName = new QLabel(tabProperty);
        lblName->setObjectName(QString::fromUtf8("lblName"));
        lblName->setGeometry(QRect(190, 10, 141, 21));
        lblName->setFrameShape(QFrame::Panel);
        lblName->setFrameShadow(QFrame::Sunken);
        label_5 = new QLabel(tabProperty);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(130, 10, 41, 21));
        tabs_ShapeProperty->addTab(tabProperty, QString());

        verticalLayout->addWidget(tabs_ShapeProperty);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnSave = new QPushButton(ObjectPlaceEditDlg);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setAutoDefault(false);

        horizontalLayout_2->addWidget(btnSave);

        btnCancel = new QPushButton(ObjectPlaceEditDlg);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setFocusPolicy(Qt::StrongFocus);
        btnCancel->setAutoDefault(false);

        horizontalLayout_2->addWidget(btnCancel);

        btnExit = new QPushButton(ObjectPlaceEditDlg);
        btnExit->setObjectName(QString::fromUtf8("btnExit"));
        btnExit->setFocusPolicy(Qt::StrongFocus);
        btnExit->setAutoDefault(false);

        horizontalLayout_2->addWidget(btnExit);


        verticalLayout->addLayout(horizontalLayout_2);

        QWidget::setTabOrder(cmbCategory, cmbObjectPlace);
        QWidget::setTabOrder(cmbObjectPlace, btnSave);
        QWidget::setTabOrder(btnSave, btnExit);

        retranslateUi(ObjectPlaceEditDlg);

        tabs_ShapeProperty->setCurrentIndex(1);
        btnSave->setDefault(true);


        QMetaObject::connectSlotsByName(ObjectPlaceEditDlg);
    } // setupUi

    void retranslateUi(QDialog *ObjectPlaceEditDlg)
    {
        ObjectPlaceEditDlg->setWindowTitle(QCoreApplication::translate("ObjectPlaceEditDlg", "ObjectPlaceEdit", nullptr));
        label_2->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Category", nullptr));
        lblObjectPlace->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Object_Place", nullptr));
        label_8->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Name", nullptr));
        btnCenterPoint->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "CenterPoint", nullptr));
        label_10->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Longitude", nullptr));
        label_9->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Latitude", nullptr));
        label_6->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Name_ko", nullptr));
        label_15->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "HtmlPath", nullptr));
        btnHtmlPath->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "...", nullptr));
        btnVideoPath->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "...", nullptr));
        label_16->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "VideoPath", nullptr));
        btnModify->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Modify", nullptr));
        btnNewAdd->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "NewAdd", nullptr));
        btnDelete->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Delete", nullptr));
        tabs_ShapeProperty->setTabText(tabs_ShapeProperty->indexOf(tabPlaceMark), QCoreApplication::translate("ObjectPlaceEditDlg", "PlaceMark", nullptr));
        addButton->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Add", nullptr));
        editButton->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Edit", nullptr));
        deleteButton->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Del", nullptr));
        tabs_ShapeProperty->setTabText(tabs_ShapeProperty->indexOf(tabPointList), QCoreApplication::translate("ObjectPlaceEditDlg", "PointList", nullptr));
        label->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Accident Location :", nullptr));
        label_3->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Lat", nullptr));
        label_4->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Long", nullptr));
        lblLat->setText(QString());
        lblLong->setText(QString());
        lblName->setText(QString());
        label_5->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "aName", nullptr));
        tabs_ShapeProperty->setTabText(tabs_ShapeProperty->indexOf(tabProperty), QCoreApplication::translate("ObjectPlaceEditDlg", "Property", nullptr));
        btnSave->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Save", nullptr));
        btnCancel->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Cancel", nullptr));
        btnExit->setText(QCoreApplication::translate("ObjectPlaceEditDlg", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ObjectPlaceEditDlg: public Ui_ObjectPlaceEditDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBJECTPLACEEDITDLG_H
