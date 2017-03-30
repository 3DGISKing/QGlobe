#ifndef BROWSER_H
#define BROWSER_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>

#include "ui_browserwidget.h"

class ConnectionWidget;
QT_FORWARD_DECLARE_CLASS(QTableView)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QSqlError)

class Browser: public QWidget, private Ui::Browser
{
    Q_OBJECT
public:
    Browser(QWidget *parent = 0);
    virtual ~Browser();

    QSqlError addConnection(const QString &driver, const QString &dbName, const QString &host,
                  const QString &user, const QString &passwd, int port = -1);

    void insertRow();
	void deleteRow();
	void cutRow();
    void updateActions();

	QSqlDatabase db;

private:
	bool cutedFlag;
	bool nocutFlag;
	bool metaFlag;

public slots:
	bool exec();
    void showTable(const QString &table);
    void showMetaData(const QString &table);
	void runCutPaste(const QString &table);
    void addConnection();
    void currentChanged() { updateActions(); }
    void sqlMake();

    void on_insertRowAction_triggered()
    { insertRow(); }
	void on_deleteRowAction_triggered()
	{ deleteRow(); }
	void on_cutRowAction_triggered()
	{ cutRow(); }
    void on_connectionWidget_tableActivated(const QString &table)
    { showTable(table); }
    void on_connectionWidget_metaDataRequested(const QString &table)
    { showMetaData(table); }
	void on_connectionWidget_setCutPaste(const QString &table)
	{ runCutPaste(table); }
    void on_submitButton_clicked()
    {
		if(exec()) {
			QMessageBox::information(this, tr("OK"), tr("SQL Execute OK !!!"));
		} else {
			QMessageBox::critical(this, tr("FAILED"), tr("SQL Execute Failed !!!"));
		}
        sqlEdit->setFocus();
    }
    void on_clearButton_clicked()
    {
        sqlEdit->clear();
		submitButton->setEnabled(!cutedFlag);
		connectionWidget->pstAction->setEnabled(cutedFlag);
        sqlEdit->setFocus();
    }
	void on_connectButton_clicked()
	{
		addConnection();
	}

signals:
    void statusMessage(const QString &message);
};

#endif
