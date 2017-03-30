#include "directorydialog.h"
#include "ui_directorydialog.h"
#include <QDirModel>
#include <QItemSelectionModel>

DirectoryDialog::DirectoryDialog(QWidget *parent) :
	QDialog(parent),m_ui(new Ui::DirectoryDialog)
{
    m_ui->setupUi(this);
}

DirectoryDialog::~DirectoryDialog()
{
    delete m_ui;
	Release();
}

QString DirectoryDialog::GetDirectory(QWidget * parent , const QString & dir )
{
	QString dirName = dir;

	DirectoryDialog dlg(parent);
	dlg.Init();
	dlg.SelectDir(dirName);

	if (QDialog::Accepted == dlg.exec())
	{
		dirName = dlg.SelectedDir();
	}
	return dirName;
}

bool DirectoryDialog::Init()
{
	if (!CreateLayout())
		return false;

	if (!InitDirView())
		return false;

	return true;
}

void DirectoryDialog::Release()
{
	if(m_model)
		delete m_model;
	m_model = NULL;
}

QString DirectoryDialog::SelectedDir() const
{
	QString dir;
	QModelIndex index = m_ui->directoryView->selectionModel()->currentIndex();
	if (index.isValid())
		dir = m_model->filePath(index);

	return dir;
}

void DirectoryDialog::SelectDir(const QString &dirname)
{
	QString dir = dirname;
	if (dir.isEmpty())
		dir = QDir::currentPath();

	QModelIndex index = m_model->index(dir);
	if (index.isValid())
		m_ui->directoryView->setCurrentIndex(index);
}

void DirectoryDialog::changeEvent(QEvent *e)
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

bool DirectoryDialog::CreateLayout()
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	if (!mainLayout)
		return false;

	mainLayout->addWidget(m_ui->directorylabel);
	mainLayout->addWidget(m_ui->directoryView);
	mainLayout->addWidget(m_ui->buttonBox);
	setLayout(mainLayout);

	return true;

}

bool DirectoryDialog::InitDirView()
{
	m_model = new QDirModel;
	if (!m_model)
		return false;

	m_model->setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Drives);

	m_ui->directoryView->setModel(m_model);
	m_ui->directoryView->setColumnHidden(1, true);
	m_ui->directoryView->setColumnHidden(2, true);
	m_ui->directoryView->setColumnHidden(3, true);
	m_ui->directoryView->setHeaderHidden(true);

	// Demonstrating look and feel features
	m_ui->directoryView->setAnimated(false);
	m_ui->directoryView->setIndentation(20);
	m_ui->directoryView->setSortingEnabled(true);

	return true;
}
