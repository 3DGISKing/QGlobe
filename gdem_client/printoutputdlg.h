#ifndef PRINTOUTPUTDLG_H
#define PRINTOUTPUTDLG_H

#include <QtGui/QDialog>
#include "ui_printoutputdlg.h"

class PrintOutputDlg : public QDialog {
    Q_OBJECT

public:
    PrintOutputDlg(QString *a_pStr, QWidget *parent = 0);
    ~PrintOutputDlg();

public slots:
    void on_OKButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::PrintOutputDlg ui;

    QString*         m_pStr;
};

#endif // PRINTOUTPUTDLG_H
