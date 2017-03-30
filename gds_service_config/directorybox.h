#ifndef DIRECTORYBOX_H
#define DIRECTORYBOX_H

#include <QWidget>
#include <QToolButton>
#include <QtGui/QLineEdit>

class DirectoryBox : public QWidget
{
	Q_OBJECT

public:
	DirectoryBox(QWidget *parent);
	~DirectoryBox();

	QLineEdit *m_edit;
	QToolButton *m_button;
	void SetValue(QString &value);
	QString GetValue();

signals:
	void setDirectory();

private:
	bool Init();
	void Release();

private slots:
	void OpenBrowse();

protected:
	void focusInEvent(QFocusEvent *event);
};

#endif // DIRECTORYBOX_H
