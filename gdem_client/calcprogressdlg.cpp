#include "calcprogressdlg.h"
#include "ui_calcprogressdlg.h"
#include <QTime>

CalcProgressDlg::CalcProgressDlg(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CalcProgressDlg)
{
	m_ui->setupUi(this);

	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	setAttribute(Qt::WA_DeleteOnClose);
	setFixedSize(size());

	QString format = QString("%p%1").arg(QChar(0xFF05));
	m_ui->calcProgressBar->setFormat(format);

	m_succecced = -1;

	setProgress(0);

	connect(m_ui->stopButton, SIGNAL(clicked()), this, SLOT(StopCalc()));
}

CalcProgressDlg::~CalcProgressDlg()
{
	delete m_ui;
}

bool CalcProgressDlg::isStop()
{
	if(m_succecced == 0)
		return true;
	return false;
}

static void artisticSleep(int sleepTime)
{
    QTime time;
    time.restart();
    while (time.elapsed() < sleepTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
}

void CalcProgressDlg::setProgress(double percent)
{
	if(m_succecced == 0)
		return;

	m_ui->calcProgressBar->setValue((int)percent);

	artisticSleep(5);

	if (percent >= 100)
	{
		m_succecced = 1;
		accept();
	}
}

void CalcProgressDlg::StopCalc()
{
	m_succecced = 0;
	reject();
}
