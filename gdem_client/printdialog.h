#ifndef PRINTDIALOG_H
#define PRINTDIALOG_H

#include <QDialog>
#include <QList>
#include "ui_printdlg.h"

class PrintDialog : public QDialog
{
    Q_OBJECT

public:
    PrintDialog(QPrinter* a_pPrinter, QWidget* a_pParent = 0);
    virtual ~PrintDialog();

public slots:
    void on_printerCombo_currentIndexChanged(int a_nIndex);
    void on_sizeCombo_currentIndexChanged(int a_nIndex);
    void on_OKButton_clicked();
    void on_cancelButton_clicked();

private:
    void    LoadPrinterInfoOnSystem();
    void    LoadPageSizeList();
    QString GetPageSizeStrFormUint(uint a_nPageSize, QSizeF &a_oPageSize);
    void    OptimizePageSizeList(QList<QPrinter::PaperSize> &a_PageSizeList);
    bool    SetPrinterInfo();

private:
    Ui::PrintDlg ui;

    QList<QPrinterInfo>             m_PrintInfoList;
    QList<QPrinter::PaperSize>      m_PageSizeList;
    QPrinterInfo                    m_CurPrintInfo;

    QPrinter*                       m_pPrinter;
};

#endif // PRINTDIALOG_H
