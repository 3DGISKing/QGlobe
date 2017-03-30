#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QtGui/QDialog>

namespace Ui {
    class AboutDlg;
}

class AboutDlg : public QDialog {
    Q_OBJECT
public:
    AboutDlg(QWidget *parent = 0);
    ~AboutDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AboutDlg *m_ui;
};

#endif // ABOUTDLG_H
