#pragma once

#include <QDialog>
#include "qglobeclientbase.h"

namespace Ui {
    class CQGlobe_CommandDlg;
}

class CQGlobe_CommandDlg : public QDialog {
    Q_OBJECT
public:
    CQGlobe_CommandDlg(QWidget *parent = 0);
    ~CQGlobe_CommandDlg();
public:

	// update daialog data event
	virtual void UpdateDialog(void) = 0;
};

