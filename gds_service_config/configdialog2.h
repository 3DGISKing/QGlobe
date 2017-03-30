#ifndef CONFIGDIALOG2_H
#define CONFIGDIALOG2_H

#include <QtGui/QDialog>
#include <QAbstractButton>

namespace Ui
{
	class ConfigDialog2;
}

class ConfigDialog2 : public QDialog
{
	Q_OBJECT

public:
	ConfigDialog2(QWidget *parent = 0);
	~ConfigDialog2();
	bool Init();
	void Release();

private:
	Ui::ConfigDialog2 *m_ui;

	bool ReadConfig();
	void SaveConfig();
	bool CreateLayout();
	void SetValues();
	void GetValues();
	void StartAllService();
	bool IsChanged();
	bool IsValidValue();
	bool IsValidPath(QString& sPath);

private slots:
	void OpenBrowse();
	void OpenBrowse2();
	void ClickButtonBox(QAbstractButton *button);
	void InstallService();
	void RestartAllService();
	void ApplyService();
};

#endif // CONFIGDIALOG2_H
