/********************************************************************************
** Form generated from reading UI file 'searchdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHDLG_H
#define UI_SEARCHDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SearchDlg
{
public:
    QFrame *fraCommon;
    QComboBox *searchCombo;
    QLabel *lblKeyWord;
    QFrame *fraStyle;
    QLabel *lblSearchStyle;
    QComboBox *cmbSearchStyle;
    QPushButton *searchButton;
    QFrame *fraAround;
    QLabel *lblDistance;
    QComboBox *cmbMetrics;
    QLineEdit *edtDistance;
    QLabel *lblCenteral;
    QLabel *lblLat;
    QLineEdit *edtLat;
    QLineEdit *edtLong;
    QLabel *lblLong;

    void setupUi(QDialog *SearchDlg)
    {
        if (SearchDlg->objectName().isEmpty())
            SearchDlg->setObjectName(QString::fromUtf8("SearchDlg"));
        SearchDlg->resize(304, 193);
        QFont font;
        font.setPointSize(10);
        SearchDlg->setFont(font);
        fraCommon = new QFrame(SearchDlg);
        fraCommon->setObjectName(QString::fromUtf8("fraCommon"));
        fraCommon->setGeometry(QRect(10, 60, 281, 41));
        fraCommon->setLineWidth(0);
        searchCombo = new QComboBox(fraCommon);
        searchCombo->setObjectName(QString::fromUtf8("searchCombo"));
        searchCombo->setGeometry(QRect(80, 10, 191, 25));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(searchCombo->sizePolicy().hasHeightForWidth());
        searchCombo->setSizePolicy(sizePolicy);
        searchCombo->setMinimumSize(QSize(0, 25));
        searchCombo->setFont(font);
        searchCombo->setEditable(true);
        searchCombo->setFrame(true);
        lblKeyWord = new QLabel(fraCommon);
        lblKeyWord->setObjectName(QString::fromUtf8("lblKeyWord"));
        lblKeyWord->setGeometry(QRect(10, 10, 61, 29));
        fraStyle = new QFrame(SearchDlg);
        fraStyle->setObjectName(QString::fromUtf8("fraStyle"));
        fraStyle->setGeometry(QRect(10, 10, 281, 41));
        fraStyle->setLineWidth(0);
        lblSearchStyle = new QLabel(fraStyle);
        lblSearchStyle->setObjectName(QString::fromUtf8("lblSearchStyle"));
        lblSearchStyle->setGeometry(QRect(10, 10, 81, 29));
        cmbSearchStyle = new QComboBox(fraStyle);
        cmbSearchStyle->setObjectName(QString::fromUtf8("cmbSearchStyle"));
        cmbSearchStyle->setGeometry(QRect(90, 10, 91, 25));
        cmbSearchStyle->setMinimumSize(QSize(0, 25));
        searchButton = new QPushButton(fraStyle);
        searchButton->setObjectName(QString::fromUtf8("searchButton"));
        searchButton->setGeometry(QRect(200, 10, 75, 25));
        searchButton->setFont(font);
        fraAround = new QFrame(SearchDlg);
        fraAround->setObjectName(QString::fromUtf8("fraAround"));
        fraAround->setGeometry(QRect(10, 110, 281, 71));
        fraAround->setLineWidth(0);
        lblDistance = new QLabel(fraAround);
        lblDistance->setObjectName(QString::fromUtf8("lblDistance"));
        lblDistance->setGeometry(QRect(10, 10, 61, 29));
        cmbMetrics = new QComboBox(fraAround);
        cmbMetrics->setObjectName(QString::fromUtf8("cmbMetrics"));
        cmbMetrics->setGeometry(QRect(190, 10, 81, 25));
        cmbMetrics->setMinimumSize(QSize(0, 25));
        cmbMetrics->setSizeIncrement(QSize(1, 1));
        cmbMetrics->setMaxVisibleItems(20);
        edtDistance = new QLineEdit(fraAround);
        edtDistance->setObjectName(QString::fromUtf8("edtDistance"));
        edtDistance->setGeometry(QRect(70, 10, 113, 25));
        edtDistance->setMinimumSize(QSize(0, 25));
        lblCenteral = new QLabel(fraAround);
        lblCenteral->setObjectName(QString::fromUtf8("lblCenteral"));
        lblCenteral->setGeometry(QRect(10, 40, 61, 31));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(3);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(lblCenteral->sizePolicy().hasHeightForWidth());
        lblCenteral->setSizePolicy(sizePolicy1);
        lblCenteral->setSizeIncrement(QSize(1, 1));
        lblLat = new QLabel(fraAround);
        lblLat->setObjectName(QString::fromUtf8("lblLat"));
        lblLat->setGeometry(QRect(70, 40, 21, 31));
        sizePolicy1.setHeightForWidth(lblLat->sizePolicy().hasHeightForWidth());
        lblLat->setSizePolicy(sizePolicy1);
        lblLat->setSizeIncrement(QSize(1, 1));
        edtLat = new QLineEdit(fraAround);
        edtLat->setObjectName(QString::fromUtf8("edtLat"));
        edtLat->setGeometry(QRect(100, 40, 71, 25));
        edtLat->setMinimumSize(QSize(0, 25));
        edtLong = new QLineEdit(fraAround);
        edtLong->setObjectName(QString::fromUtf8("edtLong"));
        edtLong->setGeometry(QRect(200, 40, 71, 25));
        edtLong->setMinimumSize(QSize(0, 25));
        lblLong = new QLabel(fraAround);
        lblLong->setObjectName(QString::fromUtf8("lblLong"));
        lblLong->setGeometry(QRect(170, 40, 31, 31));
        sizePolicy1.setHeightForWidth(lblLong->sizePolicy().hasHeightForWidth());
        lblLong->setSizePolicy(sizePolicy1);
        lblLong->setSizeIncrement(QSize(1, 1));

        retranslateUi(SearchDlg);

        QMetaObject::connectSlotsByName(SearchDlg);
    } // setupUi

    void retranslateUi(QDialog *SearchDlg)
    {
        SearchDlg->setWindowTitle(QCoreApplication::translate("SearchDlg", "Dialog", nullptr));
        lblKeyWord->setText(QCoreApplication::translate("SearchDlg", "KeyWord : ", nullptr));
        lblSearchStyle->setText(QCoreApplication::translate("SearchDlg", "SearchType : ", nullptr));
        searchButton->setText(QCoreApplication::translate("SearchDlg", "Search", nullptr));
        lblDistance->setText(QCoreApplication::translate("SearchDlg", "Distance : ", nullptr));
        lblCenteral->setText(QCoreApplication::translate("SearchDlg", "Centeral : ", nullptr));
        lblLat->setText(QCoreApplication::translate("SearchDlg", "Lat", nullptr));
        lblLong->setText(QCoreApplication::translate("SearchDlg", "Long", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SearchDlg: public Ui_SearchDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHDLG_H
