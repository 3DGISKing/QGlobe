/********************************************************************************
** Form generated from reading UI file 'keygenerator.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYGENERATOR_H
#define UI_KEYGENERATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KeyGeneratorClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEditModelNumber;
    QLineEdit *lineEditKey;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonKeyGen;

    void setupUi(QMainWindow *KeyGeneratorClass)
    {
        if (KeyGeneratorClass->objectName().isEmpty())
            KeyGeneratorClass->setObjectName(QString::fromUtf8("KeyGeneratorClass"));
        KeyGeneratorClass->resize(350, 98);
        centralWidget = new QWidget(KeyGeneratorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lineEditModelNumber = new QLineEdit(centralWidget);
        lineEditModelNumber->setObjectName(QString::fromUtf8("lineEditModelNumber"));
        lineEditModelNumber->setReadOnly(false);

        verticalLayout->addWidget(lineEditModelNumber);

        lineEditKey = new QLineEdit(centralWidget);
        lineEditKey->setObjectName(QString::fromUtf8("lineEditKey"));
        lineEditKey->setReadOnly(true);

        verticalLayout->addWidget(lineEditKey);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonKeyGen = new QPushButton(centralWidget);
        pushButtonKeyGen->setObjectName(QString::fromUtf8("pushButtonKeyGen"));

        horizontalLayout->addWidget(pushButtonKeyGen);


        verticalLayout->addLayout(horizontalLayout);

        KeyGeneratorClass->setCentralWidget(centralWidget);

        retranslateUi(KeyGeneratorClass);

        QMetaObject::connectSlotsByName(KeyGeneratorClass);
    } // setupUi

    void retranslateUi(QMainWindow *KeyGeneratorClass)
    {
        KeyGeneratorClass->setWindowTitle(QCoreApplication::translate("KeyGeneratorClass", "KeyGenerator", nullptr));
        pushButtonKeyGen->setText(QCoreApplication::translate("KeyGeneratorClass", "Generate", nullptr));
    } // retranslateUi

};

namespace Ui {
    class KeyGeneratorClass: public Ui_KeyGeneratorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYGENERATOR_H
