#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QtGui/QDialog>
#include <qsharedmemory.h>
#include <QAbstractButton>

namespace Ui
{
	class ConfigDialog;
}

class GDSConfig;
class GDEMDistributeConfig;
class DirDelegate;
class QStandardItemModel;
class QTableView;

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
	ConfigDialog(QWidget *parent = 0);
	~ConfigDialog();

	bool Init();
	void Release();

private:
	Ui::ConfigDialog *m_ui;

	QStandardItemModel	*m_pcModel;
	QStandardItemModel	*m_hardModel;
	DirDelegate			*m_itemDelegate;
	QSharedMemory sharedMemory;

	bool ReadConfig();
	void SaveConfig();
	bool CreateLayout();
	bool InitDistViews();
	void SetValues();
	int GetValues();
	int GetDistributeType();
	void UpdateServiceBtn();
	void StartService();

private slots:
	void OpenBrowse();
	void ClickButtonBox(QAbstractButton *button);
	void Service();
	void AddConfRow();
	void RemoveConfRow();
	void SetMainTable();
};

#endif // CONFIGDIALOG_H
