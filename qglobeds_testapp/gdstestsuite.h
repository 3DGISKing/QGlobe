#ifndef QGlobeDSTESTSUITE_H
#define QGlobeDSTESTSUITE_H

#include <QMainWindow>
#include "ui_gdstestsuite.h"

class QGlobeDSTestSuite : public QMainWindow
{
	Q_OBJECT

public:
	QGlobeDSTestSuite(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QGlobeDSTestSuite();

private:
	Ui::QGlobeDSTestSuiteClass ui;
};

#endif // QGlobeDSTESTSUITE_H
