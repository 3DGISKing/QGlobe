#pragma once

#include <QtGui/QDialog>
#include "gdemclientbase.h"

namespace Ui {
    class CGDM_CommandDlg;
}

class CGDM_CommandDlg : public QDialog {
    Q_OBJECT
public:
    CGDM_CommandDlg(QWidget *parent = 0);
    ~CGDM_CommandDlg();
public:

	// update daialog data event
	virtual void UpdateDialog(void) = 0;
};

