#ifndef GDSTESTSUITE_H
#define GDSTESTSUITE_H

#include <QtGui/QMainWindow>
#include "ui_gdstestsuite.h"

class GDSTestSuite : public QMainWindow
{
	Q_OBJECT

public:
	GDSTestSuite(QWidget *parent = 0, Qt::WFlags flags = 0);
	~GDSTestSuite();

private:
	Ui::GDSTestSuiteClass ui;
};

#endif // GDSTESTSUITE_H
