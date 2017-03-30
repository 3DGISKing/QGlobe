#ifndef SRVCONFDLG_H
#define SRVCONFDLG_H

#include <QtGui/QDialog>

namespace Ui {
    class SrvConfDlg;
}

class SrvConfDlg : public QDialog {
    Q_OBJECT
public:
    SrvConfDlg(QWidget *parent = 0);
    ~SrvConfDlg();

	virtual void accept();

	void SetServer(char *addr, unsigned int port);
	void GetServer(char *addr, int len, unsigned int *port);

protected:
    void changeEvent(QEvent *e);

    QString m_addr;
	unsigned int m_port;

private slots:
    void ChangeControl();

private:
    Ui::SrvConfDlg *m_ui;
};

#endif // SRVCONFDLG_H
