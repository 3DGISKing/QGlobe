#include "printoutputdlg.h"

PrintOutputDlg::PrintOutputDlg(QString* a_pStr, QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    m_pStr = a_pStr;
}

PrintOutputDlg::~PrintOutputDlg()
{

}

void PrintOutputDlg::on_OKButton_clicked()
{
    if (ui.lineEdit->text().isEmpty())
        return;
    else
        *m_pStr = ui.lineEdit->text();

    QDialog::accept();
}

void PrintOutputDlg::on_cancelButton_clicked()
{
    QDialog::reject();
}
