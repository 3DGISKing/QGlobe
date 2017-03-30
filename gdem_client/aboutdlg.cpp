#include "aboutdlg.h"
#include "ui_aboutdlg.h"
#include "gdemclient.h"

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AboutDlg)
{
    m_ui->setupUi(this);

	QString fileName = g_app->GetResource("/about.png");
	QString style = QString("border-image: url(%1)").arg(fileName);
	m_ui->graphicsView->setStyleSheet(style);

	setFixedSize(size());
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
				   | Qt::WindowCloseButtonHint);

	connect(m_ui->okButton, SIGNAL(clicked()), this, SLOT(close()));
}

AboutDlg::~AboutDlg()
{
    delete m_ui;
}

void AboutDlg::changeEvent(QEvent *e)
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
