#ifndef PNGBROWSER_H
#define PNGBROWSER_H

#include <QtGui/QDialog>

class PngView;
class QComboBox;
class QPushButton;
class PngBrowser : public QDialog
{
	Q_OBJECT

public:
	PngBrowser(QWidget *parent=0);
	~PngBrowser();
	QString getPngFileName(QString dir);

private slots:
	void setChildDir(const QString & dir);
	void onOk();
	void onCancel();

private:
	PngView*    m_IconView;
	QComboBox*   m_CategoryBox;
	QPushButton* m_OkButton;
	QPushButton* m_CancalButton;

	QString      m_ParentPath;
private:
	
};

#endif // PNGROWSER_H
