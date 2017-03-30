#include "contourdlg.h"
#include "ui_contourdlg.h"
#include "gdemclient.h"
#include "msgdata.h"
#include "config.h"
#include "mainwindow.h"


#define METER_FEET_RATE	100/30.48

ContourDlg::ContourDlg(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ContourDlg)
{
    m_ui->setupUi(this);

	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
				   | Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);

	Init();
	GetParameter();
	UpdateDialog();

	connect(m_ui->intervalEdit, SIGNAL(editingFinished()), this, SLOT(SetContourInterval()));
	connect(m_ui->ravineColorButton, SIGNAL(clicked()), this, SLOT(SetRavineColor()));
	connect(m_ui->ravineThickSpin, SIGNAL(valueChanged(int)), this, SLOT(SetRavineThick(int)));
	connect(m_ui->mainColorButton, SIGNAL(clicked()), this, SLOT(SetMainCurveColor()));
	connect(m_ui->mainThickSpin, SIGNAL(valueChanged(int)), this, SLOT(SetMainCurveThick(int)));
	connect(m_ui->defaultValueButton, SIGNAL(clicked()), this, SLOT(SetDefaultValue()));
	connect(m_ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

ContourDlg::~ContourDlg()
{
    delete m_ui;
}

void ContourDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ContourDlg::Init()
{
	m_ui->ravineThickSpin->setRange(CONTOUR_THICK_MIN, CONTOUR_THICK_MAX);
	m_ui->mainThickSpin->setRange(CONTOUR_THICK_MIN, CONTOUR_THICK_MAX);
}

void ContourDlg::GetParameter()
{
	Config *config = g_app->GetConfig();

	m_contourInterval = config->m_contourInterval;
	m_ravineColor = QColor(config->m_ravineColor);
	m_mainCurveColor = QColor(config->m_mainCurveColor);
	m_ravineThick = config->m_ravineThick;
	m_mainCurveThick = config->m_mainCurveThick;
}

void ContourDlg::SetParameter()
{
	Config *config = g_app->GetConfig();

	config->m_contourInterval = m_contourInterval;
	config->m_ravineColor = m_ravineColor.rgb();
	config->m_mainCurveColor = m_mainCurveColor.rgb();
	config->m_ravineThick = m_ravineThick;
	config->m_mainCurveThick = m_mainCurveThick;
}

void ContourDlg::UpdateDialog()
{
	if (g_app->GetConfig()->m_distShowMode == OPTION_DISTANCE_METER_KILO)
	{
		m_ui->intervalEdit->setText(QString("%1").arg((int)m_contourInterval));
		m_ui->intervalUnitLabel->setText(MSG_CONTOUR_METER_DESC);
	}
	else
	{
		double feet = m_contourInterval * METER_FEET_RATE;
		m_ui->intervalEdit->setText(QString("%1").arg(feet, 0, 'f', 2, QLatin1Char('0')));
		m_ui->intervalUnitLabel->setText(MSG_CONTOUR_FEET_DESC);
	}

	QString sheet = QString("background-color: %1").arg(m_ravineColor.name());
	m_ui->ravineColorButton->setStyleSheet(sheet);
	m_ui->ravineThickSpin->setValue(m_ravineThick);

	sheet = QString("background-color: %1").arg(m_mainCurveColor.name());
	m_ui->mainColorButton->setStyleSheet(sheet);
	m_ui->mainThickSpin->setValue(m_mainCurveThick);
}

void ContourDlg::SetContourInterval()
{
	if (g_app->GetConfig()->m_distShowMode == OPTION_DISTANCE_METER_KILO)
	{
		unsigned int mValue = m_ui->intervalEdit->text().toUInt();

		if (mValue >= CONTOUR_INTERVAL_MIN && mValue <= CONTOUR_INTERVAL_MAX)
			m_contourInterval = (double) mValue;
	}
	else
	{
		double fValue = m_ui->intervalEdit->text().toDouble();
		fValue /= METER_FEET_RATE;

		if (fValue >= CONTOUR_INTERVAL_MIN && fValue <= CONTOUR_INTERVAL_MAX)
			m_contourInterval = fValue;
	}

	SetParameter();
	UpdateDialog();
}

void ContourDlg::SetRavineColor()
{
#ifdef Q_WS_MAC
	QColor color = QColorDialog::getColor(m_ravineColor, this, QString(),
										  QColorDialog::DontUseNativeDialog);
#else
	QColor color = QColorDialog::getColor(m_ravineColor);
#endif
	if (color.isValid())
		m_ravineColor = color;

	UpdateDialog();
}

void ContourDlg::SetRavineThick(int thick)
{
	m_ravineThick = thick;
}

void ContourDlg::SetMainCurveColor()
{
#ifdef Q_WS_MAC
	QColor color = QColorDialog::getColor(m_mainCurveColor, this, QString(),
										  QColorDialog::DontUseNativeDialog);
#else
	QColor color = QColorDialog::getColor(m_mainCurveColor);
#endif
	if (color.isValid())
		m_mainCurveColor = color;

	UpdateDialog();
}

void ContourDlg::SetMainCurveThick(int thick)
{
	m_mainCurveThick = thick;
}

void ContourDlg::SetDefaultValue()
{
	m_contourInterval = 100;
	m_ravineColor = QColor(0x0000FF);
	m_mainCurveColor=  QColor(0xBEBEBE);
	m_ravineThick = 2;
	m_mainCurveThick = 1;

	UpdateDialog();
}

void ContourDlg::accept()
{
	SetParameter();
	g_app->GetMainWindow()->UpdateContourState();

	QDialog::accept();
}

void ContourDlg::reject()
{
	QDialog::reject();
}
