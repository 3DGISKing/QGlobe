/********************************************************************************
** Form generated from reading UI file 'placemarkdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLACEMARKDLG_H
#define UI_PLACEMARKDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlaceMarkDlg
{
public:
    QTabWidget *propertyTab;
    QWidget *tabView;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QLabel *label_5;
    QLineEdit *lineViewLatitude;
    QLabel *label_4;
    QLineEdit *lineViewLongitude;
    QLabel *label_7;
    QLineEdit *lineViewRange;
    QLabel *label_6;
    QLineEdit *lineViewHeading;
    QLabel *label_9;
    QLineEdit *lineViewTilt;
    QPushButton *snapshotViewButton;
    QPushButton *resetButton;
    QCheckBox *viewerCenterCheck;
    QWidget *tabDescription;
    QPlainTextEdit *textDescription;
    QLabel *label_10;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLabel *label;
    QLineEdit *lineName;
    QLineEdit *lineIconName;
    QLabel *label_11;
    QPushButton *iconnameChangeButton;
    QLabel *label_12;
    QSlider *horizontalSliderIconScale;
    QPushButton *videoPathChangeButton;
    QLabel *label_13;
    QLineEdit *lineVideoPath;
    QLabel *label_14;
    QPushButton *htmlPathChangeButton;
    QLineEdit *lineHtmlPath;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *lineLatitude;
    QLabel *label_3;
    QLineEdit *lineLongitude;
    QPushButton *btnGo;

    void setupUi(QDialog *PlaceMarkDlg)
    {
        if (PlaceMarkDlg->objectName().isEmpty())
            PlaceMarkDlg->setObjectName(QString::fromUtf8("PlaceMarkDlg"));
        PlaceMarkDlg->resize(450, 510);
        PlaceMarkDlg->setMinimumSize(QSize(450, 510));
        PlaceMarkDlg->setMaximumSize(QSize(450, 510));
        propertyTab = new QTabWidget(PlaceMarkDlg);
        propertyTab->setObjectName(QString::fromUtf8("propertyTab"));
        propertyTab->setGeometry(QRect(15, 210, 421, 261));
        tabView = new QWidget();
        tabView->setObjectName(QString::fromUtf8("tabView"));
        layoutWidget = new QWidget(tabView);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(60, 50, 301, 136));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 0, 0, 1, 1);

        lineViewLatitude = new QLineEdit(layoutWidget);
        lineViewLatitude->setObjectName(QString::fromUtf8("lineViewLatitude"));

        gridLayout_2->addWidget(lineViewLatitude, 0, 1, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        lineViewLongitude = new QLineEdit(layoutWidget);
        lineViewLongitude->setObjectName(QString::fromUtf8("lineViewLongitude"));

        gridLayout_2->addWidget(lineViewLongitude, 1, 1, 1, 1);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 2, 0, 1, 1);

        lineViewRange = new QLineEdit(layoutWidget);
        lineViewRange->setObjectName(QString::fromUtf8("lineViewRange"));

        gridLayout_2->addWidget(lineViewRange, 2, 1, 1, 1);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 3, 0, 1, 1);

        lineViewHeading = new QLineEdit(layoutWidget);
        lineViewHeading->setObjectName(QString::fromUtf8("lineViewHeading"));

        gridLayout_2->addWidget(lineViewHeading, 3, 1, 1, 1);

        label_9 = new QLabel(layoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_2->addWidget(label_9, 4, 0, 1, 1);

        lineViewTilt = new QLineEdit(layoutWidget);
        lineViewTilt->setObjectName(QString::fromUtf8("lineViewTilt"));

        gridLayout_2->addWidget(lineViewTilt, 4, 1, 1, 1);

        snapshotViewButton = new QPushButton(tabView);
        snapshotViewButton->setObjectName(QString::fromUtf8("snapshotViewButton"));
        snapshotViewButton->setGeometry(QRect(170, 200, 150, 24));
        snapshotViewButton->setFocusPolicy(Qt::StrongFocus);
        snapshotViewButton->setAutoDefault(false);
        resetButton = new QPushButton(tabView);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));
        resetButton->setGeometry(QRect(330, 200, 75, 24));
        resetButton->setFocusPolicy(Qt::StrongFocus);
        resetButton->setAutoDefault(false);
        viewerCenterCheck = new QCheckBox(tabView);
        viewerCenterCheck->setObjectName(QString::fromUtf8("viewerCenterCheck"));
        viewerCenterCheck->setGeometry(QRect(290, 20, 120, 18));
        viewerCenterCheck->setLayoutDirection(Qt::LeftToRight);
        propertyTab->addTab(tabView, QString());
        tabDescription = new QWidget();
        tabDescription->setObjectName(QString::fromUtf8("tabDescription"));
        textDescription = new QPlainTextEdit(tabDescription);
        textDescription->setObjectName(QString::fromUtf8("textDescription"));
        textDescription->setGeometry(QRect(10, 30, 405, 215));
        label_10 = new QLabel(tabDescription);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 10, 100, 22));
        propertyTab->addTab(tabDescription, QString());
        okButton = new QPushButton(PlaceMarkDlg);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(250, 480, 75, 24));
        okButton->setFocusPolicy(Qt::StrongFocus);
        okButton->setAutoDefault(false);
        cancelButton = new QPushButton(PlaceMarkDlg);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(340, 480, 75, 24));
        cancelButton->setFocusPolicy(Qt::StrongFocus);
        cancelButton->setAutoDefault(false);
        label = new QLabel(PlaceMarkDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 45, 24));
        lineName = new QLineEdit(PlaceMarkDlg);
        lineName->setObjectName(QString::fromUtf8("lineName"));
        lineName->setGeometry(QRect(89, 20, 351, 22));
        lineIconName = new QLineEdit(PlaceMarkDlg);
        lineIconName->setObjectName(QString::fromUtf8("lineIconName"));
        lineIconName->setEnabled(true);
        lineIconName->setGeometry(QRect(91, 52, 321, 22));
        label_11 = new QLabel(PlaceMarkDlg);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 55, 81, 16));
        iconnameChangeButton = new QPushButton(PlaceMarkDlg);
        iconnameChangeButton->setObjectName(QString::fromUtf8("iconnameChangeButton"));
        iconnameChangeButton->setGeometry(QRect(417, 50, 25, 25));
        label_12 = new QLabel(PlaceMarkDlg);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 90, 81, 16));
        horizontalSliderIconScale = new QSlider(PlaceMarkDlg);
        horizontalSliderIconScale->setObjectName(QString::fromUtf8("horizontalSliderIconScale"));
        horizontalSliderIconScale->setGeometry(QRect(90, 90, 349, 20));
        horizontalSliderIconScale->setMinimum(1);
        horizontalSliderIconScale->setMaximum(100);
        horizontalSliderIconScale->setValue(1);
        horizontalSliderIconScale->setOrientation(Qt::Horizontal);
        horizontalSliderIconScale->setTickPosition(QSlider::NoTicks);
        videoPathChangeButton = new QPushButton(PlaceMarkDlg);
        videoPathChangeButton->setObjectName(QString::fromUtf8("videoPathChangeButton"));
        videoPathChangeButton->setGeometry(QRect(417, 118, 25, 25));
        label_13 = new QLabel(PlaceMarkDlg);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 123, 81, 16));
        lineVideoPath = new QLineEdit(PlaceMarkDlg);
        lineVideoPath->setObjectName(QString::fromUtf8("lineVideoPath"));
        lineVideoPath->setEnabled(true);
        lineVideoPath->setGeometry(QRect(91, 120, 321, 22));
        label_14 = new QLabel(PlaceMarkDlg);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(9, 153, 81, 16));
        htmlPathChangeButton = new QPushButton(PlaceMarkDlg);
        htmlPathChangeButton->setObjectName(QString::fromUtf8("htmlPathChangeButton"));
        htmlPathChangeButton->setGeometry(QRect(416, 148, 25, 25));
        lineHtmlPath = new QLineEdit(PlaceMarkDlg);
        lineHtmlPath->setObjectName(QString::fromUtf8("lineHtmlPath"));
        lineHtmlPath->setEnabled(true);
        lineHtmlPath->setGeometry(QRect(90, 150, 321, 22));
        horizontalLayoutWidget = new QWidget(PlaceMarkDlg);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(90, 180, 351, 25));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(horizontalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        lineLatitude = new QLineEdit(horizontalLayoutWidget);
        lineLatitude->setObjectName(QString::fromUtf8("lineLatitude"));

        horizontalLayout->addWidget(lineLatitude);

        label_3 = new QLabel(horizontalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        lineLongitude = new QLineEdit(horizontalLayoutWidget);
        lineLongitude->setObjectName(QString::fromUtf8("lineLongitude"));

        horizontalLayout->addWidget(lineLongitude);

        btnGo = new QPushButton(horizontalLayoutWidget);
        btnGo->setObjectName(QString::fromUtf8("btnGo"));
        btnGo->setMaximumSize(QSize(50, 16777215));
        btnGo->setFocusPolicy(Qt::StrongFocus);
        btnGo->setAutoDefault(false);

        horizontalLayout->addWidget(btnGo);

        QWidget::setTabOrder(lineName, propertyTab);
        QWidget::setTabOrder(propertyTab, textDescription);
        QWidget::setTabOrder(textDescription, lineViewLatitude);
        QWidget::setTabOrder(lineViewLatitude, lineViewLongitude);
        QWidget::setTabOrder(lineViewLongitude, lineViewRange);
        QWidget::setTabOrder(lineViewRange, lineViewHeading);
        QWidget::setTabOrder(lineViewHeading, lineViewTilt);
        QWidget::setTabOrder(lineViewTilt, viewerCenterCheck);
        QWidget::setTabOrder(viewerCenterCheck, snapshotViewButton);
        QWidget::setTabOrder(snapshotViewButton, resetButton);
        QWidget::setTabOrder(resetButton, okButton);
        QWidget::setTabOrder(okButton, cancelButton);

        retranslateUi(PlaceMarkDlg);

        propertyTab->setCurrentIndex(0);
        okButton->setDefault(true);
        btnGo->setDefault(true);


        QMetaObject::connectSlotsByName(PlaceMarkDlg);
    } // setupUi

    void retranslateUi(QDialog *PlaceMarkDlg)
    {
        PlaceMarkDlg->setWindowTitle(QCoreApplication::translate("PlaceMarkDlg", "New PlaceMark", nullptr));
        label_5->setText(QCoreApplication::translate("PlaceMarkDlg", "Latitude:", nullptr));
        label_4->setText(QCoreApplication::translate("PlaceMarkDlg", "Longitude:", nullptr));
        label_7->setText(QCoreApplication::translate("PlaceMarkDlg", "Range:", nullptr));
        label_6->setText(QCoreApplication::translate("PlaceMarkDlg", "Heading:", nullptr));
        label_9->setText(QCoreApplication::translate("PlaceMarkDlg", "Tilt:", nullptr));
        snapshotViewButton->setText(QCoreApplication::translate("PlaceMarkDlg", "Snapshot current view", nullptr));
        resetButton->setText(QCoreApplication::translate("PlaceMarkDlg", "Reset", nullptr));
        viewerCenterCheck->setText(QCoreApplication::translate("PlaceMarkDlg", "Center in view", nullptr));
        propertyTab->setTabText(propertyTab->indexOf(tabView), QCoreApplication::translate("PlaceMarkDlg", "View", nullptr));
        label_10->setText(QCoreApplication::translate("PlaceMarkDlg", "Description:", nullptr));
        propertyTab->setTabText(propertyTab->indexOf(tabDescription), QCoreApplication::translate("PlaceMarkDlg", "Description", nullptr));
        okButton->setText(QCoreApplication::translate("PlaceMarkDlg", "Ok", nullptr));
        cancelButton->setText(QCoreApplication::translate("PlaceMarkDlg", "Cancel", nullptr));
        label->setText(QCoreApplication::translate("PlaceMarkDlg", "Name: ", nullptr));
        label_11->setText(QCoreApplication::translate("PlaceMarkDlg", "Icon Name", nullptr));
        iconnameChangeButton->setText(QCoreApplication::translate("PlaceMarkDlg", "...", nullptr));
        label_12->setText(QCoreApplication::translate("PlaceMarkDlg", "IconScale", nullptr));
        videoPathChangeButton->setText(QCoreApplication::translate("PlaceMarkDlg", "...", nullptr));
        label_13->setText(QCoreApplication::translate("PlaceMarkDlg", "Video Path", nullptr));
        label_14->setText(QCoreApplication::translate("PlaceMarkDlg", "Html Path", nullptr));
        htmlPathChangeButton->setText(QCoreApplication::translate("PlaceMarkDlg", "...", nullptr));
        label_2->setText(QCoreApplication::translate("PlaceMarkDlg", "Latitude:", nullptr));
        label_3->setText(QCoreApplication::translate("PlaceMarkDlg", "Longitude:", nullptr));
        btnGo->setText(QCoreApplication::translate("PlaceMarkDlg", "Go", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlaceMarkDlg: public Ui_PlaceMarkDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLACEMARKDLG_H
