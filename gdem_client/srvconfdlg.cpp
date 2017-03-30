#include "mainwindow.h"
#include "srvconfdlg.h"
#include "ui_srvconfdlg.h"

SrvConfDlg::SrvConfDlg(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SrvConfDlg)
{
    m_ui->setupUi(this);
	setFixedSize(size());
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
				   | Qt::WindowCloseButtonHint);
    m_addr.clear();
    m_port = 0;

    connect(m_ui->ipEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(ChangeControl()));
    connect(m_ui->portEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(ChangeControl()));
	connect(m_ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_ui->closeButton, SIGNAL(clicked()), this, SLOT(reject()));
}

SrvConfDlg::~SrvConfDlg()
{
    delete m_ui;
}

void SrvConfDlg::accept()
{
	bool res=false;
	m_addr = m_ui->ipEdit->text();
	m_port = (unsigned int)m_ui->portEdit->text().toInt(&res);

	QDialog::accept();
}

void SrvConfDlg::SetServer(char *addr, unsigned int port)
{
	m_addr = QString(addr);
	m_port = port;

	m_ui->ipEdit->setText(m_addr);
	if (port > 0)
	{
		QString portText;
		portText.setNum(port);
		m_ui->portEdit->setText(portText);
	}
	ChangeControl();
}

void SrvConfDlg::GetServer(char *addr, int len, unsigned int *port)
{
	QByteArray ba = m_addr.toAscii();
	const char *data = ba.constData();

	strncpy(addr, data, len);
	*port = m_port;
}

void SrvConfDlg::changeEvent(QEvent *e)
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

static bool CheckAddress(const QString &addr)
{
	bool res = false;
	if (addr.isEmpty())
		return false;

	static char validCodes[] = {'a', 'z', '0', '9', '.', '.', 0};
	int i = 0;
	for (i = 0; i < addr.length(); i ++)
	{
		ushort code = addr.at(i).toLower().unicode();
		if (code > 0x0ff)
			continue;

		int cn = 0;
		for (cn = 0; validCodes[cn] != 0; cn += 2)
		{
			char code1 = validCodes[cn];
			char code2 = validCodes[cn+1];
			if (code1 <= (char)code && (char)code <= code2)
				break;
		}
		if (validCodes[cn] == 0)
			break;
	}

	if (i == addr.length())
		res = true;

	return res;
}

void SrvConfDlg::ChangeControl()
{
	bool res = false;
	QString addr = m_ui->ipEdit->text();
	int port = m_ui->portEdit->text().toInt(&res);

	bool addrCheck = false;
	bool portCheck = false;

	//check address
	addrCheck = CheckAddress(addr);

	//check port
	if (0<port && port<=0x0ffff)
		portCheck = true;

	m_ui->okButton->setEnabled(addrCheck && res && portCheck);
}
