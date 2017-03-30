#include "configdialog.h"
#include "ui_configdialog.h"
#include "directorydialog.h"
#include "../gds_service_common/gdsservice_common.h"
#include "../gdem_common/gdem_distributeconfig.h"
#include "../gds_common/GDSConfigMgr.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QDialogButtonBox>
#include "dirdelegate.h"
#include <QMessageBox>

#define MONITOR_SERVICE_NAME	"GDS Service Monitoring"

#define TAB3_BUTTONS_SPACING	20
#define LABEL_RIGHT_SPACE		5
#define TAB3_NUM_LAYOUT_SPACE	50

enum GetResult	{ GR_Ok, GR_NumFail};

void gc_wchar2ushort(wchar_t *src, ushort *dst, int len)
{
	int i = 0;
	while (src[i] > 0)
	{
		if (len > 0 && i >= len)
			break;
		dst[i] = (ushort)src[i];
		i ++;
	}
	dst[i] = 0;
}

ConfigDialog::ConfigDialog(QWidget *parent)
	: QDialog(parent), m_ui(new Ui::ConfigDialog)
{
	m_ui->setupUi(this);
	m_pcModel		= NULL;
	m_hardModel		= NULL;
	m_itemDelegate	= NULL;
}

ConfigDialog::~ConfigDialog()
{
	Release();
	delete m_ui;
}

bool ConfigDialog::Init()
{
	if (!ReadConfig())
		return false;

	if (!CreateLayout())
		return false;

	if (!InitDistViews())
		return false;

	connect(m_ui->browseButton, SIGNAL(clicked()), this, SLOT(OpenBrowse()));
	connect(m_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this,
			SLOT(ClickButtonBox(QAbstractButton*)));
	connect(m_ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(m_ui->serviceButton, SIGNAL(clicked()), this, SLOT(Service()));
	connect(m_ui->addDistItemBtn, SIGNAL(clicked()), this, SLOT(AddConfRow()));
	connect(m_ui->rmvDistItemBtn, SIGNAL(clicked()), this, SLOT(RemoveConfRow()));
	connect(m_ui->pcRadioBtn, SIGNAL(toggled(bool)), this, SLOT(SetMainTable()));
	connect(m_ui->hardRadioBtn, SIGNAL(toggled(bool)), this, SLOT(SetMainTable()));
	connect(m_ui->noneRadioBtn, SIGNAL(toggled(bool)), this, SLOT(SetMainTable()));
	connect(m_ui->pcTable, SIGNAL(activated(QModelIndex)), this, SLOT(ActiveWindow(QModelIndex)));

	SetValues();
	StartService();
	UpdateServiceBtn();

	return true;
}

void ConfigDialog::Release()
{
	if (m_pcModel)
	{
		delete m_pcModel;
	}
	if (m_hardModel)
	{
		delete m_hardModel;
	}
	if (m_itemDelegate)
	{
		delete m_itemDelegate;
	}
}

bool ConfigDialog::ReadConfig()
{
	g_ServerConfig.ReadServerConfig();
	g_DistributeConfig.ReadFromRegistry();
	return true;
}

void ConfigDialog::SaveConfig()
{
	if (GetValues() == GR_NumFail)
	{
		QMessageBox::warning(this, tr("Service Options"),
							 QObject::tr("Distribute Original Number is less than Distribute Number!"));
		m_ui->spinDistOrgNum->setFocus();
		return;
	}

	g_ServerConfig.WriteServerConfig();
	g_DistributeConfig.WriteToRegistry();
}

bool ConfigDialog::CreateLayout()
{
	QHBoxLayout *subLayout = new QHBoxLayout;
	if (!subLayout)
		return false;
	subLayout->addWidget(m_ui->serviceButton);
	subLayout->addWidget(m_ui->buttonBox);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	if (!mainLayout)
		return false;

	mainLayout->addWidget(m_ui->tabWidget);
	mainLayout->addItem(subLayout);
	setLayout(mainLayout);

	// Distribute Tab
	QHBoxLayout *distTypeGrpLayt = new QHBoxLayout;
	if (!distTypeGrpLayt)
		return false;
	distTypeGrpLayt->addWidget(m_ui->pcRadioBtn);
	distTypeGrpLayt->addWidget(m_ui->hardRadioBtn);
	distTypeGrpLayt->addWidget(m_ui->noneRadioBtn);
	m_ui->noneRadioBtn->setChecked(true);
	m_ui->disttypeGroup->setLayout(distTypeGrpLayt);

	QHBoxLayout *distNumGrpLayt = new QHBoxLayout;
	if (!distNumGrpLayt)
		return false;
	QHBoxLayout *distSubNum1Layt = new QHBoxLayout;
	if (!distSubNum1Layt)
		return false;
	distSubNum1Layt->addWidget(m_ui->labelDistOrgNum);
	distSubNum1Layt->addWidget(m_ui->spinDistOrgNum);
	distSubNum1Layt->setSpacing(LABEL_RIGHT_SPACE);

	QHBoxLayout *distSubNum2Layt = new QHBoxLayout;
	if (!distSubNum2Layt)
		return false;
	distSubNum2Layt->addWidget(m_ui->labeDistNum);
	distSubNum2Layt->addWidget(m_ui->spinDistNum);
	distSubNum2Layt->setSpacing(LABEL_RIGHT_SPACE);

	distNumGrpLayt->addItem(distSubNum1Layt);
	distNumGrpLayt->addItem(distSubNum2Layt);
	distNumGrpLayt->setSpacing(TAB3_NUM_LAYOUT_SPACE);
	m_ui->groupDisNum->setLayout(distNumGrpLayt);

	QHBoxLayout *ctrlBtnsLayt = new QHBoxLayout;
	if (!ctrlBtnsLayt)
		return false;
	ctrlBtnsLayt->addWidget(m_ui->addDistItemBtn);
	ctrlBtnsLayt->addWidget(m_ui->rmvDistItemBtn);
	ctrlBtnsLayt->setSpacing(TAB3_BUTTONS_SPACING);
	ctrlBtnsLayt->setAlignment(Qt::AlignHCenter);

	QVBoxLayout *page3Layout = new QVBoxLayout;
	if (!page3Layout)
		return false;
	page3Layout->addWidget(m_ui->disttypeGroup);
	page3Layout->addWidget(m_ui->groupDisNum);
	page3Layout->addWidget(m_ui->pcTable);
	page3Layout->addWidget(m_ui->hardTable);
	page3Layout->addItem(ctrlBtnsLayt);
	int count = m_ui->tabWidget->count();
	m_ui->tabWidget->widget(count-1)->setLayout(page3Layout);

#ifndef USE_MYSQL
	m_ui->tabWidget->removeTab(1);
#endif
	return true;
}

bool ConfigDialog::InitDistViews()
{
	m_pcModel = new QStandardItemModel(0,4, this);
	if (!m_pcModel)
		return false;
	m_pcModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Start Index"));
	m_pcModel->setHeaderData(1, Qt::Horizontal, QObject::tr("End Index"));
	m_pcModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Server IP"));
	m_pcModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Server Port"));
	m_ui->pcTable->setAlternatingRowColors(true);
	m_ui->pcTable->horizontalHeader()->setStretchLastSection(true);
	m_ui->pcTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_ui->pcTable->setModel(m_pcModel);

	m_hardModel = new QStandardItemModel(0,3, this);
	if (!m_hardModel)
		return false;
	m_hardModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Start Index"));
	m_hardModel->setHeaderData(1, Qt::Horizontal, QObject::tr("End Index"));
	m_hardModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Database directory"));
	m_ui->hardTable->setAlternatingRowColors(true);
	m_ui->hardTable->horizontalHeader()->setStretchLastSection(true);
	m_ui->hardTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_ui->hardTable->setModel(m_hardModel);

	m_itemDelegate = new DirDelegate;
	if (!m_itemDelegate)
		return false;

	m_ui->hardTable->setItemDelegate(m_itemDelegate);


	return true;
}

void ConfigDialog::SetValues()
{
	// configure
	m_ui->srvportSpin->setValue(g_ServerConfig.GetPortNo());
	m_ui->srvconnectSpin->setValue(g_ServerConfig.GetMaxConnectNum());

#ifdef USE_MYSQL
	m_ui->srvaddrEdit->setText(g_ServerConfig.GetDBServerIP());
	m_ui->dbusrEdit->setText(g_ServerConfig.GetDBUserID());
	m_ui->dbpasswdEdit->setText(g_ServerConfig.GetDBPassword());
	m_ui->dbnameEdit->setText(g_ServerConfig.GetDBName());
#endif

	m_ui->dbpathEdit->setText(g_ServerConfig.GetFileDBPath());

	// distribute configure
	E_DISTRIBUTE_TYPE tDistType = g_DistributeConfig.GetDistributableType();
	int nTargetCount = g_DistributeConfig.GetDistributeTargetNum();
	m_ui->spinDistOrgNum->setValue(g_DistributeConfig.GetDistributeNum());
	m_ui->spinDistNum->setValue(nTargetCount);
	if (tDistType == E_DISTRIBUTE_TYPE_PC)
	{
		m_ui->pcRadioBtn->setChecked(true);

		ST_DISTRIBUTE_PC tPCInfo;
		for(int i = 0; i < nTargetCount; ++i)
		{
			g_DistributeConfig.GetPCInfo(i, tPCInfo);
			m_pcModel->setRowCount(i+1);
			m_pcModel->setData(m_pcModel->index(i, 0), tPCInfo.start_index);
			m_pcModel->setData(m_pcModel->index(i, 1), tPCInfo.end_index);
			m_pcModel->setData(m_pcModel->index(i, 2), tPCInfo.ip_address);
			m_pcModel->setData(m_pcModel->index(i, 3), tPCInfo.port);
		}
	}
	else if (tDistType == E_DISTRIBUTE_TYPE_DIRECTORY)
	{
		m_ui->hardRadioBtn->setChecked(true);
		ST_DISTRIBUTE_FOLDER tFolderInfo;
		for(int i = 0; i < nTargetCount; ++i)
		{
			g_DistributeConfig.GetFolderInfo(i, tFolderInfo);
			m_hardModel->setRowCount(i+1);
			m_hardModel->setData(m_hardModel->index(i, 0), tFolderInfo.start_index);
			m_hardModel->setData(m_hardModel->index(i, 1), tFolderInfo.end_index);
			m_hardModel->setData(m_hardModel->index(i, 2),
								 QString::fromUtf16(tFolderInfo.path));
		}
	}
	else
	{
		m_ui->noneRadioBtn->setChecked(true);
	}

	SetMainTable();
}

int ConfigDialog::GetValues()
{
	g_ServerConfig.SetPortNo(m_ui->srvportSpin->value());
	g_ServerConfig.SetMaxConnectNum(m_ui->srvconnectSpin->value());

#ifdef USE_MYSQL
	g_ServerConfig.SetDBServerIP(m_ui->srvaddrEdit->text());
	g_ServerConfig.SetDBUserID(m_ui->dbusrEdit->text());
	g_ServerConfig.SetDBPassword(m_ui->dbpasswdEdit->text());
	g_ServerConfig.SetDBName(m_ui->dbnameEdit->text());
#endif
        QString strdbPath = m_ui->dbpathEdit->text();
        g_ServerConfig.SetFileDBPath(strdbPath);

	if (m_ui->spinDistOrgNum->value() < m_ui->spinDistNum->value())
		return GR_NumFail;

	g_DistributeConfig.Initialize();

	E_DISTRIBUTE_TYPE tDistType = (E_DISTRIBUTE_TYPE)GetDistributeType();
	g_DistributeConfig.SetDistributableType(tDistType);
	g_DistributeConfig.SetDistributeNum(m_ui->spinDistOrgNum->value());
	g_DistributeConfig.SetDistributeTargetNum(m_ui->spinDistNum->value());

	QStandardItem * item = NULL;
	if (tDistType == E_DISTRIBUTE_TYPE_PC)
	{
		ST_DISTRIBUTE_PC pc;
		QString strIP = "";
		for(int i = 0; i < m_pcModel->rowCount(); ++i)
		{
			item = m_pcModel->item(i, 0);
			if (!item)
				continue;
			if (item->text().isEmpty())
				continue;
			pc.start_index = item->text().toUInt();

			item = m_pcModel->item(i, 1);
			if (!item)
				continue;
			if (item->text().isEmpty())
				continue;
			pc.end_index = item->text().toUInt();

			item = m_pcModel->item(i, 2);
			if (!item)
				continue;
			strIP = item->text();
			if (strIP.isEmpty())
				continue;
			strcpy(pc.ip_address, strIP.toAscii());

			item = m_pcModel->item(i, 3);
			if (!item)
				continue;
			if (item->text().isEmpty())
				continue;
			pc.port = item->text().toUInt();
			g_DistributeConfig.AddPCInfo(pc);
		}
	}
	else if (tDistType == E_DISTRIBUTE_TYPE_DIRECTORY)
	{
		ST_DISTRIBUTE_FOLDER dir;
		QString strDir = "";
		for(int i = 0; i < m_hardModel->rowCount(); ++i)
		{
			item = m_hardModel->item(i, 0);
			if (!item)
				continue;
			if (item->text().isEmpty())
				continue;
			dir.start_index = item->text().toUInt();

			item = m_hardModel->item(i, 1);
			if (!item)
				continue;
			if (item->text().isEmpty())
				continue;
			dir.end_index = item->text().toUInt();

			item = m_hardModel->item(i, 2);
			if (!item)
				continue;
			strDir = item->text();
			if (strDir.isEmpty())
				continue;
			wchar_t temppath[MAX_DIST_PATH];
			strDir.toWCharArray(temppath);
			gc_wchar2ushort(temppath, dir.path, strDir.count());
			g_DistributeConfig.AddFolderInfo(dir);
		}
	}
	else
	{
		g_DistributeConfig.SetDistributeNum(0);
		g_DistributeConfig.SetDistributeTargetNum(0);
	}
	return GR_Ok;
}

int ConfigDialog::GetDistributeType()
{
	if (m_ui->pcRadioBtn->isChecked())
		return E_DISTRIBUTE_TYPE_PC;
	else if (m_ui->hardRadioBtn->isChecked())
		return E_DISTRIBUTE_TYPE_DIRECTORY;

	return E_DISTRIBUTE_TYPE_NONE;
}

void ConfigDialog::UpdateServiceBtn()
{
	GDSServiceController controller(MONITOR_SERVICE_NAME);
	bool bService = controller.isRunning();
	if (bService)
		m_ui->serviceButton->setText(tr("Stop Service"));
	else
		m_ui->serviceButton->setText(tr("Start Service"));
}

void ConfigDialog::StartService()
{
	GDSServiceController controller(MONITOR_SERVICE_NAME);
	bool bService = controller.isRunning();
	if (!bService)
		controller.start();
}

void ConfigDialog::OpenBrowse()
{
	QString dir = m_ui->dbpathEdit->text();
	m_ui->dbpathEdit->setText(DirectoryDialog::GetDirectory(this, dir));
}

void ConfigDialog::ClickButtonBox(QAbstractButton *button)
{
	QDialogButtonBox::ButtonRole role = m_ui->buttonBox->buttonRole(button);
	switch (role)
	{
		case QDialogButtonBox::AcceptRole:
			SaveConfig();
			accept();
			break;
		case QDialogButtonBox::ApplyRole:
			SaveConfig();
			break;
		default:
			break;
	}
}

void ConfigDialog::Service()
{
	GDSServiceController controller(MONITOR_SERVICE_NAME);
	bool bService = controller.isRunning();
	if (bService)
		controller.stop();
	else
		controller.start();

	UpdateServiceBtn();
}

void ConfigDialog::AddConfRow()
{
	int row = 0;

	if (GetDistributeType() == E_DISTRIBUTE_TYPE_PC)
	{
		row = m_pcModel->rowCount();
		m_pcModel->setRowCount(row + 1);
		m_pcModel->setData(m_pcModel->index(row, 0), 0);
		m_pcModel->setData(m_pcModel->index(row, 1), 0);
		m_pcModel->setData(m_pcModel->index(row, 2), QString());
		m_pcModel->setData(m_pcModel->index(row, 3), 0);
	}
	else if (GetDistributeType() == E_DISTRIBUTE_TYPE_DIRECTORY)
	{
		row = m_hardModel->rowCount();
		m_hardModel->setRowCount(row + 1);
		m_hardModel->setData(m_hardModel->index(row, 0), 0);
		m_hardModel->setData(m_hardModel->index(row, 1), 0);
		m_hardModel->setData(m_hardModel->index(row, 2), QString());
	}
}

void ConfigDialog::RemoveConfRow()
{
	QModelIndex index;
	if (GetDistributeType() == E_DISTRIBUTE_TYPE_PC)
	{
		index = m_ui->pcTable->selectionModel()->currentIndex();
		if (index.isValid())
			m_pcModel->removeRow(index.row());
	}
	else if (GetDistributeType() == E_DISTRIBUTE_TYPE_DIRECTORY)
	{
		index = m_ui->hardTable->selectionModel()->currentIndex();
		if (index.isValid())
			m_hardModel->removeRow(index.row());
	}
}

void ConfigDialog::SetMainTable()
{
	m_ui->spinDistOrgNum->setEnabled(true);
	m_ui->spinDistNum->setEnabled(true);
	m_ui->pcTable->setEnabled(true);
	m_ui->hardTable->setEnabled(true);
	m_ui->addDistItemBtn->setEnabled(true);
	m_ui->rmvDistItemBtn->setEnabled(true);

	int type = GetDistributeType();
	if (type == E_DISTRIBUTE_TYPE_PC)
	{
		m_ui->hardTable->hide();
		m_ui->pcTable->show();
	}
	else if (type == E_DISTRIBUTE_TYPE_DIRECTORY)
	{
		m_ui->pcTable->hide();
		m_ui->hardTable->show();
	}
	else
	{
		m_ui->spinDistOrgNum->setDisabled(true);
		m_ui->spinDistNum->setDisabled(true);
		m_ui->hardTable->hide();
		m_ui->pcTable->show();
		m_ui->pcTable->setDisabled(true);
		m_ui->hardTable->setDisabled(true);
		m_ui->addDistItemBtn->setDisabled(true);
		m_ui->rmvDistItemBtn->setDisabled(true);		
	}
}
