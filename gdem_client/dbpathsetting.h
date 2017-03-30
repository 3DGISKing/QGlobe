#ifndef DBPATHSETTING_H
#define DBPATHSETTING_H

#include "../gdem_common/gdem_config.h"

#include <QtGui/QDialog>

#ifdef STANDARD_ALONE

namespace Ui {
    class SetDBPathDlg;
}

class DBPathSetting : public QDialog
{
    Q_OBJECT
public:
    DBPathSetting(QWidget *parent = 0);
    virtual ~DBPathSetting();

public slots:
    void on_toolButton_clicked();
    void on_OKButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::SetDBPathDlg *m_ui;
};

#endif

#endif // DBPATHSETTING_H
