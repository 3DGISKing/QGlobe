#include "browser.h"
#include "qsqlconnectiondialog.h"

#include <QtGui>

Browser::Browser(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
	connect(connectButton,SIGNAL(clicked()),this,SLOT(on_connectButton_clicked()));

    table->addAction(insertRowAction);
	table->addAction(deleteRowAction);
	//table->addAction(cutRowAction);

	cutedFlag = false;
	nocutFlag = false;
	metaFlag = false;
	on_clearButton_clicked();

    if (QSqlDatabase::drivers().isEmpty())
        QMessageBox::critical(this, tr("Faild"), tr("No database drivers found"));

    emit statusMessage(tr("Ready."));
}

Browser::~Browser()
{
}

//////////////void Browser::clear()
//////////////{
//////////////	QSqlQuery qry("", db);
//////////////	QList<QString> dropSQL;
//////////////	dropSQL <<
//////////////
//////////////	for (int i = 0; i < list.size(); ++i) {
//////////////		if (list.at(i) == "Jane")
//////////////			cout << "Found Jane at position " << i << endl;
//////////////	}
//////////////	//QFile file("./clear.sql");
//////////////	//if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//////////////	//	return;
//////////////	//QTextStream in(&file);
//////////////	//int ii = 0;
//////////////	//QString LineNum;
//////////////	//while (!in.atEnd()) {
//////////////	//	ii++;
//////////////	//	QString line = in.readLine();
//////////////		if(!qry.exec(line)) {
//////////////			QMessageBox::critical(this, tr("FAILED"), tr("FAILED ! ! !"));
//////////////			file.close();	
//////////////			return;
//////////////		}
//////////////	//}     
//////////////	//file.close();	
//////////////}


bool Browser::exec()
{
	bool ret = false;
	QSqlQuery qry("", db);
	QString sql;
	QStringList sql_list = sqlEdit->toPlainText().split(";", QString::SkipEmptyParts);
	if(!sql_list.empty()) ret = true;
	foreach (sql, sql_list) {
		if(sql.trimmed() != "") 
			if(!qry.exec(sql)) {
				emit statusMessage(qry.lastError().text() + "  :  " + sql);
				ret = false;
			} else emit statusMessage(tr("Executing Query . . ."));
	}
	return ret;
}

QSqlError Browser::addConnection(const QString &driver, const QString &dbName, const QString &host,
                            const QString &user, const QString &passwd, int port)
{
    static int cCount = 0;

    QSqlError err;
	db = QSqlDatabase::addDatabase(driver, QString("Browser%1").arg(++cCount));
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port);
    if (!db.open(user, passwd)) {
        err = db.lastError();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(QString("Browser%1").arg(cCount));
    }
    connectionWidget->refresh();

    return err;
}

void Browser::addConnection()
{
    QSqlConnectionDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
        return;

        QSqlError err = addConnection(dialog.driverName(), dialog.databaseName(), dialog.hostName(),
                           dialog.userName(), dialog.password(), dialog.port());
        if (err.type() != QSqlError::NoError)
            QMessageBox::warning(this, tr("Unable to open database"), tr("An error occurred while "
                                       "opening the connection: ") + err.text());
}

void Browser::showTable(const QString &t)
{
    QSqlTableModel *model = new QSqlTableModel(table, connectionWidget->currentDatabase());
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->setTable(t);
    model->select();
    if (model->lastError().type() != QSqlError::NoError)
        emit statusMessage(model->lastError().text());
    table->setModel(model);
    table->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);

    connect(table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(currentChanged()));
    updateActions();

	cutedFlag = false;
	on_clearButton_clicked();
	QSqlQuery qry("select type from tb_type where table_name='" + t + "';", db);
	if(qry.first()) nocutFlag = false; else nocutFlag = true;
	metaFlag = false;
}

void Browser::showMetaData(const QString &t)
{
    QSqlRecord rec = connectionWidget->currentDatabase().record(t);
    QStandardItemModel *model = new QStandardItemModel(table);

    model->insertRows(0, rec.count());
    model->insertColumns(0, 7);

    model->setHeaderData(0, Qt::Horizontal, "Fieldname");
    model->setHeaderData(1, Qt::Horizontal, "Type");
    model->setHeaderData(2, Qt::Horizontal, "Length");
    model->setHeaderData(3, Qt::Horizontal, "Precision");
    model->setHeaderData(4, Qt::Horizontal, "Required");
    model->setHeaderData(5, Qt::Horizontal, "AutoValue");
    model->setHeaderData(6, Qt::Horizontal, "DefaultValue");


    for (int i = 0; i < rec.count(); ++i) {
        QSqlField fld = rec.field(i);

		//if(i==3 || i==4 || i==5 || i==6) fld.setType(QVariant::Double);

        model->setData(model->index(i, 0), fld.name());
        model->setData(model->index(i, 1), fld.typeID() == -1
                ? QString(QVariant::typeToName(fld.type()))
                : QString("%1 (%2)").arg(QVariant::typeToName(fld.type())).arg(fld.typeID()));
        model->setData(model->index(i, 2), fld.length());
        model->setData(model->index(i, 3), fld.precision());
        model->setData(model->index(i, 4), fld.requiredStatus() == -1 ? QVariant("?")
                : QVariant(bool(fld.requiredStatus())));
        model->setData(model->index(i, 5), fld.isAutoValue());
        model->setData(model->index(i, 6), fld.defaultValue());
    }

    table->setModel(model);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    updateActions();
	cutedFlag = false;
	on_clearButton_clicked();
	metaFlag = true;
}

void Browser::insertRow()
{
	QSqlTableModel *model = qobject_cast<QSqlTableModel *>(table->model());
	if (!model)
		return;

	QModelIndex insertIndex = table->currentIndex();
	int row = insertIndex.row() == -1 ? 0 : insertIndex.row();
	model->insertRow(row);
	insertIndex = model->index(row, 0);
	table->setCurrentIndex(insertIndex);
	table->edit(insertIndex);
	emit statusMessage(tr("Insert OK"));
}

void Browser::deleteRow()
{
    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(table->model());
    if (!model)
        return;

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    QModelIndexList currentSelection = table->selectionModel()->selectedIndexes();
    for (int i = 0; i < currentSelection.count(); ++i) {
        if (currentSelection.at(i).column() != 0)
            continue;
        model->removeRow(currentSelection.at(i).row());
    }

    model->submitAll();
    model->setEditStrategy(QSqlTableModel::OnRowChange);

    updateActions();
	emit statusMessage(tr("Delete OK"));
}

void Browser::updateActions()
{
    bool enableIns = qobject_cast<QSqlTableModel *>(table->model());
    bool enableDel = enableIns && table->currentIndex().isValid();

    insertRowAction->setEnabled(enableIns);
	deleteRowAction->setEnabled(enableDel);
	//cutRowAction->setEnabled(enableDel && !nocutFlag);
}

void Browser::cutRow()
{
	cutedFlag = true;
	on_clearButton_clicked();
	QSqlTableModel *model = qobject_cast<QSqlTableModel *>(table->model());
	if (!model)
		return;
	QString tb_name = model->tableName();

	QModelIndexList currentSelection = table->selectionModel()->selectedIndexes();
	for (int i = 0; i < currentSelection.count(); ++i) {
		if (currentSelection.at(i).column() != 0)
			continue;
		QString rid = model->record(currentSelection.at(i).row()).value(0).toString();
		sqlEdit->setPlainText(sqlEdit->toPlainText() + "delete from " + tb_name + " where id=" + rid + ";\n");
	}
	emit statusMessage(tr("Data Moving Ready"));
}

void Browser::runCutPaste(const QString &t)
{
	QString paste_type;
	QSqlQuery qry("select type from tb_type where table_name='" + t + "';", db);
	if(qry.first()) {
		paste_type = qry.value(0).toString();
	} else {
		QMessageBox::critical(this, tr("FAILED"), tr("Invalid table !!!\n\nRetry Select Table !!!"));
		return;
	}

	QSqlTableModel *model = qobject_cast<QSqlTableModel *>(table->model());
	if (!model)
		return;
	QString iid, name, name_ko, latitude, longitude, height, view_height, type, detailURL, GM, AREA_ID, AGE;
	QModelIndexList currentSelection = table->selectionModel()->selectedIndexes();
	for (int i = 0; i < currentSelection.count(); ++i) {
		if (currentSelection.at(i).column() != 0)
			continue;
		int rowid = currentSelection.at(i).row();
		iid = model->record(rowid).value("id").toString();
		name = model->record(rowid).value("name").toString();
		name_ko = model->record(rowid).value("name_ko").toString();
		latitude = model->record(rowid).value("latitude").toString();
		longitude = model->record(rowid).value("longitude").toString();
		height = model->record(rowid).value("height").toString();
		view_height = model->record(rowid).value("view_height").toString();
		type = model->record(rowid).value("type").toString();
		detailURL = model->record(rowid).value("detailURL").toString();
		GM = model->record(rowid).value("GM").toString();
		AREA_ID = model->record(rowid).value("AREA_ID").toString();
		AGE = model->record(rowid).value("AGE").toString();

		sqlEdit->setPlainText(sqlEdit->toPlainText() + 
			"insert into " + t + "(id, name, name_ko, latitude, longitude, height, view_height, type, detailURL, GM, AREA_ID, AGE)"
			" values(" + iid + ", '" + name + "', '" + name_ko + "', " + latitude + ", " + longitude + ","
			" " + height + ", " + view_height + ", " + paste_type + ", '" + detailURL + "', " + GM + ", " + AREA_ID + ", " + AGE + ");\n");
	}
	int ret = QMessageBox::information(this, tr("Data Move"), tr("Moving Data !!!\n\nDo you want to move Data ?"), QMessageBox::Yes|QMessageBox::No);
	if(ret == QMessageBox::No) {
		cutedFlag = false;
		on_clearButton_clicked();
		return;
	} else {
		if(exec()) {
			QMessageBox::information(this, tr("OK"), tr("Data Moving OK !!!"));
			emit statusMessage(tr("Data Moving OK"));
		} else {
			QMessageBox::critical(this, tr("FAILED"), tr("Data Moving Failed !!!"));
			emit statusMessage(tr("Data Moving Failed"));
		}
		cutedFlag = false;
		on_clearButton_clicked();
	}
}

void Browser::sqlMake()
{
	if(metaFlag) {
//CREATE TABLE tb_pl_water (id int primary key, name varchar(50), name_ko varchar(50), 
//latitude double, longitude double, height double, view_height double, type int, code varchar(10), 
//detailURL varchar(45), AREA_ID int, GM int, AGE int);
		emit statusMessage("Cannot sqlMake !");
	} else {
		QSqlTableModel *model = qobject_cast<QSqlTableModel *>(table->model());
		if (!model)
			return;
		QList<bool> isString;
		for (int l = 0; l < model->columnCount(); ++l) {
			if(model->record().field(l).type() == 10) isString << true; else isString << false;
		}
		QString sqlstr;
		QString sqlHeader = "INSERT INTO " + model->tableName() + " (" + model->headerData(0, Qt::Horizontal).toString();
		for (int i = 1; i < model->columnCount(); ++i) {
			sqlHeader += ", " + model->headerData(i, Qt::Horizontal).toString();
		}
		sqlHeader += ")";
		int j;
		for (j= 0; j < model->rowCount(); ++j) {
				if(isString[0])
					sqlstr = sqlHeader + " VALUES ('" + model->record(j).value(0).toString() + "'";
				else
					sqlstr = sqlHeader + " VALUES (" + model->record(j).value(0).toString();
			for (int k = 1; k < model->columnCount(); ++k) {
				if(isString[k])
					sqlstr += ", '" + model->record(j).value(k).toString() + "'";
				else
					sqlstr += ", " + model->record(j).value(k).toString();
			}
			sqlstr += ");\n";
			sqlEdit->setPlainText(sqlEdit->toPlainText() + sqlstr);
		}
		QString str;
		str.setNum(j);
		emit statusMessage(str + "rows !");
	}
}