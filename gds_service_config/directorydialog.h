#ifndef DIRECTORYDIALOG_H
#define DIRECTORYDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class DirectoryDialog;
}

class QDirModel;

class DirectoryDialog : public QDialog {
    Q_OBJECT
public:
    DirectoryDialog(QWidget *parent = 0);
    ~DirectoryDialog();

	static QString GetDirectory(QWidget * parent = 0, const QString & dir = QString());
	bool Init();
	void Release();
	QString SelectedDir() const;
	void SelectDir(const QString &dirname);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DirectoryDialog *m_ui;
	QDirModel *m_model;

	bool CreateLayout();
	bool InitDirView();
};

#endif // DIRECTORYDIALOG_H
