/********************************************************************************
** Form generated from reading UI file 'tilelatlong.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TILELATLONG_H
#define UI_TILELATLONG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TileLatLongClass
{
public:
    QHBoxLayout *horizontalLayout_12;
    QGroupBox *groupBox_6;
    QHBoxLayout *horizontalLayout_11;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label;
    QLineEdit *lineEdit_X;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QLineEdit *lineEdit_Y;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_3;
    QLineEdit *lineEdit_Level;
    QSpacerItem *horizontalSpacer_11;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_10;
    QLineEdit *lineEdit_ULLAT;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QLineEdit *lineEdit_ULLONG;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_12;
    QLineEdit *lineEdit_RBLAT;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_6;
    QLineEdit *lineEdit_RBLONG;
    QLabel *lbl_derived;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QLineEdit *lineEdit_Long;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QLineEdit *lineEdit_Lat;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_9;
    QLineEdit *lineEdit_LLlevel;
    QGroupBox *groupBox_4;
    QLabel *txtTileNum;

    void setupUi(QDialog *TileLatLongClass)
    {
        if (TileLatLongClass->objectName().isEmpty())
            TileLatLongClass->setObjectName(QString::fromUtf8("TileLatLongClass"));
        TileLatLongClass->resize(834, 241);
        horizontalLayout_12 = new QHBoxLayout(TileLatLongClass);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        groupBox_6 = new QGroupBox(TileLatLongClass);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        horizontalLayout_11 = new QHBoxLayout(groupBox_6);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        groupBox = new QGroupBox(groupBox_6);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_5->addWidget(label);

        lineEdit_X = new QLineEdit(groupBox);
        lineEdit_X->setObjectName(QString::fromUtf8("lineEdit_X"));
        lineEdit_X->setReadOnly(false);

        horizontalLayout_5->addWidget(lineEdit_X);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_6->addWidget(label_2);

        lineEdit_Y = new QLineEdit(groupBox);
        lineEdit_Y->setObjectName(QString::fromUtf8("lineEdit_Y"));

        horizontalLayout_6->addWidget(lineEdit_Y);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_7->addWidget(label_3);

        lineEdit_Level = new QLineEdit(groupBox);
        lineEdit_Level->setObjectName(QString::fromUtf8("lineEdit_Level"));

        horizontalLayout_7->addWidget(lineEdit_Level);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer_11);


        horizontalLayout_11->addWidget(groupBox);

        groupBox_2 = new QGroupBox(groupBox_6);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_3->addWidget(label_10);

        lineEdit_ULLAT = new QLineEdit(groupBox_2);
        lineEdit_ULLAT->setObjectName(QString::fromUtf8("lineEdit_ULLAT"));
        lineEdit_ULLAT->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEdit_ULLAT->setReadOnly(true);

        horizontalLayout_3->addWidget(lineEdit_ULLAT);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);

        lineEdit_ULLONG = new QLineEdit(groupBox_2);
        lineEdit_ULLONG->setObjectName(QString::fromUtf8("lineEdit_ULLONG"));
        lineEdit_ULLONG->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEdit_ULLONG->setReadOnly(true);

        horizontalLayout->addWidget(lineEdit_ULLONG);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_4->addWidget(label_12);

        lineEdit_RBLAT = new QLineEdit(groupBox_2);
        lineEdit_RBLAT->setObjectName(QString::fromUtf8("lineEdit_RBLAT"));
        lineEdit_RBLAT->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEdit_RBLAT->setReadOnly(true);

        horizontalLayout_4->addWidget(lineEdit_RBLAT);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_2->addWidget(label_6);

        lineEdit_RBLONG = new QLineEdit(groupBox_2);
        lineEdit_RBLONG->setObjectName(QString::fromUtf8("lineEdit_RBLONG"));
        lineEdit_RBLONG->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEdit_RBLONG->setReadOnly(true);

        horizontalLayout_2->addWidget(lineEdit_RBLONG);


        verticalLayout->addLayout(horizontalLayout_2);

        lbl_derived = new QLabel(groupBox_2);
        lbl_derived->setObjectName(QString::fromUtf8("lbl_derived"));

        verticalLayout->addWidget(lbl_derived);


        horizontalLayout_11->addWidget(groupBox_2);


        horizontalLayout_12->addWidget(groupBox_6);

        groupBox_5 = new QGroupBox(TileLatLongClass);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        verticalLayout_4 = new QVBoxLayout(groupBox_5);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        groupBox_3 = new QGroupBox(groupBox_5);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_8->addWidget(label_8);

        lineEdit_Long = new QLineEdit(groupBox_3);
        lineEdit_Long->setObjectName(QString::fromUtf8("lineEdit_Long"));
        lineEdit_Long->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_8->addWidget(lineEdit_Long);


        verticalLayout_3->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_9->addWidget(label_7);

        lineEdit_Lat = new QLineEdit(groupBox_3);
        lineEdit_Lat->setObjectName(QString::fromUtf8("lineEdit_Lat"));
        lineEdit_Lat->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_9->addWidget(lineEdit_Lat);


        verticalLayout_3->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_10->addWidget(label_9);

        lineEdit_LLlevel = new QLineEdit(groupBox_3);
        lineEdit_LLlevel->setObjectName(QString::fromUtf8("lineEdit_LLlevel"));

        horizontalLayout_10->addWidget(lineEdit_LLlevel);


        verticalLayout_3->addLayout(horizontalLayout_10);


        verticalLayout_4->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(groupBox_5);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        txtTileNum = new QLabel(groupBox_4);
        txtTileNum->setObjectName(QString::fromUtf8("txtTileNum"));
        txtTileNum->setGeometry(QRect(10, 40, 181, 20));
        QFont font;
        font.setPointSize(10);
        txtTileNum->setFont(font);
        txtTileNum->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(groupBox_4);


        horizontalLayout_12->addWidget(groupBox_5);


        retranslateUi(TileLatLongClass);
        QObject::connect(lineEdit_Lat, SIGNAL(textChanged(QString)), TileLatLongClass, SLOT(Change_Lat()));
        QObject::connect(lineEdit_Long, SIGNAL(textChanged(QString)), TileLatLongClass, SLOT(Change_Long()));
        QObject::connect(lineEdit_LLlevel, SIGNAL(textChanged(QString)), TileLatLongClass, SLOT(Change_LLlevel()));
        QObject::connect(lineEdit_Level, SIGNAL(textChanged(QString)), TileLatLongClass, SLOT(Change_Level()));
        QObject::connect(lineEdit_Y, SIGNAL(textChanged(QString)), TileLatLongClass, SLOT(Change_Y()));
        QObject::connect(lineEdit_X, SIGNAL(textChanged(QString)), TileLatLongClass, SLOT(Change_X()));

        QMetaObject::connectSlotsByName(TileLatLongClass);
    } // setupUi

    void retranslateUi(QDialog *TileLatLongClass)
    {
        TileLatLongClass->setWindowTitle(QCoreApplication::translate("TileLatLongClass", "TileLatLong", nullptr));
        groupBox_6->setTitle(QString());
        groupBox->setTitle(QCoreApplication::translate("TileLatLongClass", "Tile Number", nullptr));
        label->setText(QCoreApplication::translate("TileLatLongClass", "X      ", nullptr));
        label_2->setText(QCoreApplication::translate("TileLatLongClass", "Y      ", nullptr));
        label_3->setText(QCoreApplication::translate("TileLatLongClass", "Level", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("TileLatLongClass", "Tile Info", nullptr));
        label_10->setText(QCoreApplication::translate("TileLatLongClass", "Top     ", nullptr));
        label_4->setText(QCoreApplication::translate("TileLatLongClass", "Left     ", nullptr));
        label_12->setText(QCoreApplication::translate("TileLatLongClass", "Bottom", nullptr));
        label_6->setText(QCoreApplication::translate("TileLatLongClass", "Right   ", nullptr));
        lbl_derived->setText(QCoreApplication::translate("TileLatLongClass", "Tile Width : ", nullptr));
        groupBox_5->setTitle(QString());
        groupBox_3->setTitle(QCoreApplication::translate("TileLatLongClass", "Point Info", nullptr));
        label_8->setText(QCoreApplication::translate("TileLatLongClass", "Longitude", nullptr));
        label_7->setText(QCoreApplication::translate("TileLatLongClass", "Latitude   ", nullptr));
        label_9->setText(QCoreApplication::translate("TileLatLongClass", "Level        ", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("TileLatLongClass", "Tile Number", nullptr));
        txtTileNum->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TileLatLongClass: public Ui_TileLatLongClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TILELATLONG_H
