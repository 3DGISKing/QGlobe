#include <QtGui/QFileDialog>
#include <QtGui/QColorDialog>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>

#include "objectplacesettingdlg.h"
#include "ui_objectplacesettingdlg.h"
#include "../gdem_client/gdemclient.h"
#include "../gdem_client/guiuty.h"
#include "../gdem_client/mainwindow.h"

#define TEXTURE_SUBPATH		"/res/texture"
#define ICON_SUBPATH		"/res/icon"
#define OBJECT_PERFIX		"obj_"

ObjectPlaceSettingDlg::ObjectPlaceSettingDlg(QWidget *parent, bool isAdd) :
	QDialog(parent),
	m_ui(new Ui::ObjectPlaceSettingDlg)
{
    m_ui->setupUi(this);

	int intVal;
	QSqlQuery qry;
	QString strSql = "SELECT name_ko, type FROM tb_type ORDER BY id";
	qry.exec(strSql);
	while (qry.next()) 
	{
		intVal = qry.record().field("type").value().toInt();
		m_ui->cmbObject->addItem(qry.record().field("name_ko").value().toString(), qry.record().field("type").value());
	/*	if(intVal > m_maxTypeNo) m_maxTypeNo = intVal;*/
	}
	//m_maxTypeNo++;
	//m_tbtype = m_maxTypeNo;
//	m_ui->cmbObject->insertItem(0, tr("New Object..."), QVariant(m_maxTypeNo));
	
	m_ui->cmbShapeType->addItem(tr("point"), QVariant(OBJECT_POINT));
	m_ui->cmbShapeType->addItem(tr("line"), QVariant(OBJECT_LINE));
	m_ui->cmbShapeType->addItem(tr("polygon"), QVariant(OBJECT_POLYGON));
	
	connect(m_ui->cmbObject, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbObject_change(int)));
	connect(m_ui->cmbShapeType, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbShapeType_change(int)));
	connect(m_ui->edtName_ko, SIGNAL(textChanged(QString)), this, SLOT(refreshPreview()));

	connect(m_ui->gruIcon, SIGNAL(clicked()), this, SLOT(refreshPreview()));
	connect(m_ui->btnIconPath, SIGNAL(clicked()), this, SLOT(btnIconPath_click()));
	connect(m_ui->spnIconSize, SIGNAL(valueChanged(int)), this, SLOT(spnIconSize_change(int)));
	connect(m_ui->sldIconSize, SIGNAL(valueChanged(int)), this, SLOT(sldIconSize_change(int)));
	connect(m_ui->edtIconPath, SIGNAL(textChanged(QString)), this, SLOT(refreshPreview()));

	connect(m_ui->gruLine, SIGNAL(clicked()), this, SLOT(refreshPreview()));
	connect(m_ui->btnLineColor, SIGNAL(clicked()), this, SLOT(btnLineColor_click()));
	connect(m_ui->spnLineThick, SIGNAL(valueChanged(double)), this, SLOT(spnLineThick_change(double)));
	connect(m_ui->sldLineThick, SIGNAL(valueChanged(int)), this, SLOT(sldLineThick_change(int)));
	connect(m_ui->spnLineOpacity, SIGNAL(valueChanged(int)), this, SLOT(refreshPreview()));

	connect(m_ui->gruFill, SIGNAL(clicked()), this, SLOT(refreshPreview()));
	connect(m_ui->btnFillColor, SIGNAL(clicked()), this, SLOT(btnFillColor_click()));
	connect(m_ui->spnFillOpacity, SIGNAL(valueChanged(QString)), this, SLOT(refreshPreview()));
	connect(m_ui->btnTexturePath, SIGNAL(clicked()), this, SLOT(btnTexturePath_click()));
	connect(m_ui->edtTexturePath, SIGNAL(clicked()), this, SLOT(refreshPreview()));

	connect(m_ui->applyButton, SIGNAL(clicked()), this, SLOT(apply()));
	connect(m_ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	InitDialog();
}

ObjectPlaceSettingDlg::~ObjectPlaceSettingDlg()
{
    delete m_ui;

	//if (m_state == PLACEDLG_ACCEPT)
	//	m_placeMarkMgr->GetApplyPlaceData(&m_placeMarkData, m_isAdd);
	//else if (m_state == PLACEDLG_REJECT)
	//	m_placeMarkMgr->GetCancelPlaceData(&m_placeMarkData, m_isAdd);
}

void ObjectPlaceSettingDlg::InitDialog()
{
	QPoint mainPos = g_app->GetMainWindow()->pos();
	move(mainPos.x()+50, mainPos.y()+80);

	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
				   | Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);
	setFixedSize(size());

	m_ui->okButton->setFocus(Qt::OtherFocusReason);

	m_ui->cmbObject->setCurrentIndex(0);
}

void ObjectPlaceSettingDlg::accept()
{
	if(m_ui->edtName_ko->text().trimmed().isEmpty()) {
		gu_MessageBox(this, tr("Wrong"), tr("Input Value to Name_ko Please !"));
		m_ui->edtName_ko->setFocus();
		return;
	}

	if(m_ui->edtName->text().trimmed().isEmpty()) {
		gu_MessageBox(this, tr("Wrong"), tr("Input Value to Name Please !"));
		m_ui->edtName->setFocus();
		return;
	}

	if(m_ui->spnEndLevel->value() > 0 && m_ui->spnStartLevel->value() > m_ui->spnEndLevel->value()) {
		gu_MessageBox(this, tr("Wrong"), tr("Start Level must smaller than End Level !"));
		m_ui->spnEndLevel->setFocus();
		return;
	}

	if(!saveData()) {
		gu_MessageBox(this, tr("Failed"), tr("Failed Save Data !"));
		return;
	}

	g_app->GetMainWindow()->UpdatePlaceDlg(true);
	QDialog::accept();
}

void ObjectPlaceSettingDlg::reject()
{
	g_app->GetMainWindow()->UpdatePlaceDlg(true);
	QDialog::reject();
}

void ObjectPlaceSettingDlg::apply()
{
	if(m_ui->edtName_ko->text().trimmed().isEmpty()) {
		gu_MessageBox(this, tr("Wrong"), tr("Input Value to Name_ko Please !"));
		m_ui->edtName_ko->setFocus();
		return;
	}

	if(m_ui->edtName->text().trimmed().isEmpty()) {
		gu_MessageBox(this, tr("Wrong"), tr("Input Value to Name Please !"));
		m_ui->edtName->setFocus();
		return;
	}

	if(m_ui->spnEndLevel->value() > 0 && m_ui->spnStartLevel->value() > m_ui->spnEndLevel->value()) {
		gu_MessageBox(this, tr("Wrong"), tr("Start Level must smaller than End Level !"));
		m_ui->spnEndLevel->setFocus();
		return;
	}

	if(!saveData()) {
		gu_MessageBox(this, tr("Failed"), tr("Failed Save Data !"));
		return;
	}
}

bool ObjectPlaceSettingDlg::saveData()
{
	QSqlQuery qry;
	QString strQry;
	QString strSet;
	QString strVal;
	int intVal;
	double doubleVal;
	QVariant bindData;
	QString tableName;

	m_shapeType = m_ui->cmbShapeType->itemData(m_ui->cmbShapeType->currentIndex()).toInt();
	if(m_ui->cmbObject->currentIndex() == 0) {
		switch(m_shapeType) {
		case OBJECT_POINT:
			strQry = "INSERT INTO tb_type (type, table_name, start_level, end_level, ";
			strQry += "name, name_ko, shape_type, icon_path, icon_size) ";
			strQry += "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
			break;
		case OBJECT_LINE:
			strQry = "INSERT INTO tb_type (type, table_name, start_level, end_level, ";
			strQry += "name, name_ko, shape_type, icon_path, icon_size, line_color, line_alpha, line_width) ";
			strQry += "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
			break;
		case OBJECT_POLYGON:
			strQry = "INSERT INTO tb_type (type, table_name, start_level, end_level, ";
			strQry += "name, name_ko, shape_type, icon_path, icon_size, ";
			strQry += "line_color, line_alpha, line_width, fill_color, fill_alpha, fill_pattern) ";
			strQry += "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
			break;
		}
		qry.prepare(strQry);

		bindData.setValue(m_tbtype);
		qry.bindValue(0, bindData);

		tableName = QString(OBJECT_PERFIX) + m_ui->edtName->text().trimmed();
		bindData.setValue(tableName);
		qry.bindValue(1, bindData);

		intVal = m_ui->spnStartLevel->value();
		bindData.setValue(intVal);
		qry.bindValue(2, bindData);

		intVal = m_ui->spnEndLevel->value();
		bindData.setValue(intVal);
		qry.bindValue(3, bindData);

		strVal = m_ui->edtName->text().trimmed();
		bindData.setValue(strVal);
		qry.bindValue(4, bindData);

		strVal = m_ui->edtName_ko->text().trimmed();
		bindData.setValue(strVal);
		qry.bindValue(5, bindData);

		bindData.setValue(m_shapeType);
		qry.bindValue(6, bindData);

		strVal = m_ui->edtIconPath->text().trimmed();
		if(m_ui->gruIcon->isChecked() && !strVal.isEmpty()) {
			bindData.setValue(strVal);
			qry.bindValue(7, bindData);

			intVal = m_ui->spnIconSize->value();
			bindData.setValue(intVal);
			qry.bindValue(8, bindData);
		} else {
			qry.bindValue(7, QVariant());
			qry.bindValue(8, QVariant());
		}

		if(m_shapeType > OBJECT_POINT) {
			doubleVal = m_ui->spnLineThick->value();
			if(m_ui->gruLine->isChecked() && doubleVal > 0) {
				bindData.setValue(m_lineColor);
				qry.bindValue(9, bindData);

				intVal = m_ui->spnLineOpacity->value();
				bindData.setValue(intVal);
				qry.bindValue(10, bindData);

				bindData.setValue(doubleVal);
				qry.bindValue(11, bindData);
			} else {
				qry.bindValue(9, QVariant());
				qry.bindValue(10, QVariant());
				qry.bindValue(11, QVariant());
			}
		}

		if(m_shapeType > OBJECT_LINE) {
			strVal = m_ui->edtTexturePath->text().trimmed();
			if(m_ui->gruFill->isChecked() && (!m_fillColor.isEmpty() || !strVal.isEmpty())) {
				if(strVal.isEmpty()) {
					bindData.setValue(m_fillColor);
					qry.bindValue(12, bindData);
					qry.bindValue(14, QVariant());
					intVal = m_ui->spnFillOpacity->value();
				} else {
					bindData.setValue(strVal);
					qry.bindValue(14, bindData);
					qry.bindValue(12, QVariant());
					intVal = 0;
				}

				bindData.setValue(intVal);
				qry.bindValue(13, bindData);
			} else {
				qry.bindValue(12, QVariant());
				qry.bindValue(13, QVariant());
				qry.bindValue(14, QVariant());
			}
		}

		if(!qry.exec()) return false;
		m_savedData = qry.boundValues().values();

		strQry = "CREATE TABLE " + tableName + " (id integer PRIMARY KEY NOT NULL, ";
		strQry += "type smallint NOT NULL DEFAULT " + QString("%1").arg(m_tbtype) + ", ";
		strQry += "name varchar(50) NULL, name_ko varchar(50) NULL, latitude real NULL, longitude real NULL, ";
		strQry += "height real NULL, view_height real NULL, html_path varchar(100) NULL, ";
		strQry += "video_path varchar(100) NULL, points BLOB NULL, minlatitude real NULL, ";
		strQry += "minlongitude real NULL, maxlatitude real NULL, maxlongitude real NULL)";
		if(!qry.exec(strQry)) return false;
		strQry = "CREATE INDEX idx_" + tableName + "_latitude ON " + tableName + "(latitude ASC)";
		qry.exec(strQry);
		strQry = "CREATE INDEX idx_" + tableName + "_longitude ON " + tableName + "(longitude ASC)";
		qry.exec(strQry);
		strQry = "CREATE INDEX idx_" + tableName + "_name_ko ON " + tableName + "(name_ko ASC)";
		qry.exec(strQry);

//		m_maxTypeNo++;
		strVal = m_ui->edtName_ko->text().trimmed();
		m_ui->cmbObject->addItem(strVal, QVariant(m_tbtype));
		m_ui->cmbObject->setCurrentIndex(m_ui->cmbObject->count() - 1);
	} else {
		tableName = QString(OBJECT_PERFIX) + m_ui->edtName->text().trimmed();
		QString org_tableName = m_savedData[1].toString();
		if(org_tableName != tableName) {
			strSet = "table_name='" + tableName + "'";

			strQry = "ALTER TABLE " + org_tableName + " RENAME TO " + tableName;
			if(!qry.exec(strQry)) return false;
			strQry = "DROP INDEX idx_" + org_tableName + "_latitude";
			qry.exec(strQry);
			strQry = "DROP INDEX idx_" + org_tableName + "_longitude";
			qry.exec(strQry);
			strQry = "DROP INDEX idx_" + org_tableName + "_name_ko";
			qry.exec(strQry);
			strQry = "CREATE INDEX idx_" + tableName + "_latitude ON " + tableName + "(latitude ASC)";
			qry.exec(strQry);
			strQry = "CREATE INDEX idx_" + tableName + "_longitude ON " + tableName + "(longitude ASC)";
			qry.exec(strQry);
			strQry = "CREATE INDEX idx_" + tableName + "_name_ko ON " + tableName + "(name_ko ASC)";
			qry.exec(strQry);
		}

		intVal = m_ui->spnStartLevel->value();
		if(m_savedData[2].toInt() != intVal) {
			if(strSet.isEmpty())
				strSet = "start_level=" + QString("%1").arg(intVal);
			else
				strSet += ", start_level=" + QString("%1").arg(intVal);
		}

		intVal = m_ui->spnEndLevel->value();
		if(m_savedData[3].toInt() != intVal) {
			if(strSet.isEmpty())
				strSet = "end_level=" + QString("%1").arg(intVal);
			else
				strSet += ", end_level=" + QString("%1").arg(intVal);
		}

		strVal = m_ui->edtName->text().trimmed();
		if(m_savedData[4].toString() != strVal) {
			if(strSet.isEmpty())
				strSet = "name='" + strVal + "'";
			else
				strSet += ", name='" + strVal + "'";
		}

		strVal = m_ui->edtName_ko->text().trimmed();
		if(m_savedData[5].toString() != strVal) {
			if(strSet.isEmpty())
				strSet = "name_ko='" + strVal + "'";
			else
				strSet += ", name_ko='" + strVal + "'";
		}

		if(m_savedData[6].toInt() != m_shapeType) {
			if(strSet.isEmpty())
				strSet = "shape_type=" + QString("%1").arg(m_shapeType);
			else
				strSet += ", shape_type=" + QString("%1").arg(m_shapeType);
		}

		strVal = m_ui->edtIconPath->text().trimmed();
		if(m_ui->gruIcon->isChecked() == false) strVal = "";
		if(m_savedData[7].toString() != strVal) {
			if(strSet.isEmpty())
				strSet = "icon_path='" + strVal + "'";
			else
				strSet += ", icon_path='" + strVal + "'";
		}

		intVal = m_ui->spnIconSize->value();
		if(m_ui->gruIcon->isChecked() == false || strVal.isEmpty()) intVal = 0;
		if(m_savedData[8].toInt() != intVal) {
			if(strSet.isEmpty())
				strSet = "icon_size=" + QString("%1").arg(intVal);
			else
				strSet += ", icon_size=" + QString("%1").arg(intVal);
		}

		if(m_shapeType > OBJECT_POINT) {
			doubleVal = m_ui->spnLineThick->value();
			if(m_ui->gruLine->isChecked() == false) doubleVal = 0.0;
			if(m_savedData[11].toDouble() != doubleVal) {
				if(strSet.isEmpty())
					strSet = "line_width=" + QString::number(doubleVal, 'g', 2);
				else
					strSet += ", line_width=" + QString::number(doubleVal, 'g', 2);
			}

			if(m_ui->gruLine->isChecked() == false || doubleVal == 0.0) m_lineColor = "";
			if(m_savedData[9].toString() != m_lineColor) {
				if(strSet.isEmpty())
					strSet = "line_color='" + m_lineColor + "'";
				else
					strSet += ", line_color='" + m_lineColor + "'";
			}

			intVal = m_ui->spnLineOpacity->value();
			if(m_ui->gruLine->isChecked() == false || doubleVal == 0.0) intVal = 0;
			if(m_savedData[10].toInt() != intVal) {
				if(strSet.isEmpty())
					strSet = "line_alpha=" + QString("%1").arg(intVal);
				else
					strSet += ", line_alpha=" + QString("%1").arg(intVal);
			}
		} else {
			if(m_savedData[11].toDouble() > 0)
				if(strSet.isEmpty())
					strSet = "line_color='', line_alpha=0, line_width=0";
				else
					strSet += ", line_color='', line_alpha=0, line_width=0";
		}

		if(m_shapeType > OBJECT_LINE) {
			QString texturePath = m_ui->edtTexturePath->text().trimmed();
			if(m_ui->gruFill->isChecked() == false) texturePath = "";
			if(m_savedData[14].toString() != texturePath) {
				if(strSet.isEmpty())
					strSet = "fill_pattern='" + texturePath + "'";
				else
					strSet += ", fill_pattern='" + texturePath + "'";
			}

			if(m_ui->gruFill->isChecked() == false || !texturePath.isEmpty()) m_fillColor = "";
			if(m_savedData[12].toString() != m_fillColor) {
				if(strSet.isEmpty())
					strSet = "fill_color='" + m_fillColor + "'";
				else
					strSet += ", fill_color='" + m_fillColor + "'";
			}

			intVal = m_ui->spnFillOpacity->value();
			if(m_savedData[13].toInt() != intVal) {
				if(strSet.isEmpty())
					strSet = "fill_alpha=" + QString("%1").arg(intVal);
				else
					strSet += ", fill_alpha=" + QString("%1").arg(intVal);
			}
		} else {
			if(!m_savedData[14].toString().isEmpty() || !m_savedData[12].toString().isEmpty())
				if(strSet.isEmpty())
					strSet = "fill_color='', fill_alpha=0, fill_pattern=''";
				else
					strSet += ", fill_color='', fill_alpha=0, fill_pattern=''";
		}

		if(!strSet.isEmpty()) {
			strQry = "UPDATE tb_type SET " + strSet + " WHERE type=" + QString("%1").arg(m_tbtype);
			if(!qry.exec(strQry)) return false;
			cmbObject_change(m_ui->cmbObject->currentIndex());
		}
	}
	return true;
}

void ObjectPlaceSettingDlg::cmbObject_change(int id)
{
	m_ui->edtName_ko->blockSignals(true);
	m_ui->edtIconPath->blockSignals(true);
	m_ui->spnIconSize->blockSignals(true);
	m_ui->sldIconSize->blockSignals(true);
	m_ui->btnLineColor->blockSignals(true);
	m_ui->spnLineOpacity->blockSignals(true);
	m_ui->spnLineThick->blockSignals(true);
	m_ui->sldLineThick->blockSignals(true);
	m_ui->btnFillColor->blockSignals(true);
	m_ui->spnFillOpacity->blockSignals(true);
	m_ui->edtTexturePath->blockSignals(true);
	if(id == 0) 
	{
//		m_tbtype = m_maxTypeNo;
		m_shapeType = m_ui->cmbShapeType->itemData(0).toInt();

		m_ui->gruIcon->setChecked(true);
		m_ui->cmbShapeType->setCurrentIndex(0);
		m_ui->edtName_ko->setText("");
		m_ui->edtName->setText("");
		m_ui->spnStartLevel->setValue(10);
		m_ui->spnEndLevel->setValue(0);
		m_ui->edtIconPath->setText("");
		m_ui->spnIconSize->setValue(64);
		m_ui->sldIconSize->setValue(64);
		m_lineColor = "#00ffff";
		m_ui->btnLineColor->setStyleSheet("background-color: " + m_lineColor + ";");
		m_ui->spnLineOpacity->setValue(100);
		m_ui->spnLineThick->setValue(2.0);
		m_ui->sldLineThick->setValue(20);
		m_fillColor = "#0000ff";
		m_ui->btnFillColor->setStyleSheet("background-color: " + m_fillColor + ";");
		m_ui->spnFillOpacity->setValue(50);
		m_ui->edtTexturePath->setText("");

		m_savedData.clear();
	}
	else
	{
		QSqlQuery qry;
		m_tbtype = m_ui->cmbObject->itemData(id).toInt();
		QString strSql = "SELECT type, table_name, start_level, end_level, name, name_ko, shape_type, icon_path, icon_size, ";
		strSql += "line_color, line_width, fill_color, fill_pattern ";
		strSql += "FROM tb_type WHERE type=" + QString("%1").arg(m_tbtype);
		
		bool bSuccess=qry.exec(strSql);
		Q_ASSERT(bSuccess);
		
		if(qry.first())
		{
			m_shapeType = qry.record().field("shape_type").value().toInt();
			m_ui->cmbShapeType->setCurrentIndex(0);
			for(int ii = 0; ii < m_ui->cmbShapeType->count(); ii++) 
			{
				if(m_ui->cmbShapeType->itemData(ii).toInt() == m_shapeType) 
				{
					m_ui->cmbShapeType->setCurrentIndex(ii);
					break;
				}
			}

			m_ui->edtName_ko->setText(qry.record().field("name_ko").value().toString());
			m_ui->edtName->setText(qry.record().field("name").value().toString());
			m_ui->spnStartLevel->setValue(qry.record().field("start_level").value().toInt());
			m_ui->spnEndLevel->setValue(qry.record().field("end_level").value().toInt());

			QString iconPath = qry.record().field("icon_path").value().toString();
			if(iconPath.isEmpty())
				m_ui->gruIcon->setChecked(false);
			else
				m_ui->gruIcon->setChecked(true);
			m_ui->edtIconPath->setText(iconPath);
			int iconSize = qry.record().field("icon_size").value().toInt();
			m_ui->spnIconSize->setValue(iconSize);
			m_ui->sldIconSize->setValue(iconSize);

			double lineThick = qry.record().field("line_width").value().toDouble();
			if(lineThick == 0.0)
				m_ui->gruLine->setChecked(false);
			else
				m_ui->gruLine->setChecked(true);

			m_lineColor = qry.record().field("line_color").value().toString();
			m_ui->btnLineColor->setStyleSheet("background-color: " + m_lineColor + ";");
			
			//m_ui->spnLineOpacity->setValue(qry.value(10).toInt());
			m_ui->spnLineThick->setValue(lineThick);
			m_ui->sldLineThick->setValue((int)(lineThick * 10));

			m_fillColor = qry.record().field("fill_color").value().toString();

			QString texturePath = qry.record().field("fill_pattern").value().toString();
			if(m_fillColor.isEmpty() && texturePath.isEmpty())
				m_ui->gruFill->setChecked(false);
			else
				m_ui->gruFill->setChecked(true);
			m_ui->btnFillColor->setStyleSheet("background-color: " + m_fillColor + ";");
			
			//m_ui->spnFillOpacity->setValue(qry.value(13).toInt());
			m_ui->edtTexturePath->setText(texturePath);

			m_savedData.clear();
			for(int ii = 0; ii < 15; ii++) 
			{
				m_savedData << qry.value(ii);
			}
		}
	}
	m_ui->edtName_ko->blockSignals(false);
	m_ui->edtIconPath->blockSignals(false);
	m_ui->spnIconSize->blockSignals(false);
	m_ui->sldIconSize->blockSignals(false);
	m_ui->btnLineColor->blockSignals(false);
	m_ui->spnLineOpacity->blockSignals(false);
	m_ui->spnLineThick->blockSignals(false);
	m_ui->sldLineThick->blockSignals(false);
	m_ui->btnFillColor->blockSignals(false);
	m_ui->spnFillOpacity->blockSignals(false);
	m_ui->edtTexturePath->blockSignals(false);
	refreshPreview();
}

void ObjectPlaceSettingDlg::cmbShapeType_change(int id)
{
	switch(id) {
	case 0:
		m_ui->gruLine->setChecked(false);
		m_ui->gruLine->setEnabled(false);
		m_ui->gruFill->setChecked(false);
		m_ui->gruFill->setEnabled(false);
		break;
	case 1:
		m_ui->gruLine->setChecked(true);
		m_ui->gruLine->setEnabled(true);
		m_ui->gruFill->setChecked(false);
		m_ui->gruFill->setEnabled(false);
		break;
	case 2:
		m_ui->gruLine->setChecked(true);
		m_ui->gruLine->setEnabled(true);
		m_ui->gruFill->setChecked(true);
		m_ui->gruFill->setEnabled(true);
		break;
	}
}

void ObjectPlaceSettingDlg::refreshPreview()
{
	//m_lineChecked = m_ui->gruLine->isChecked();
}

void ObjectPlaceSettingDlg::btnIconPath_click()
{
	QString iconPath = gdm_GetWorkPath() + ICON_SUBPATH;
	//QString iconPath = QString("E:/Work/_HaeSa/2.Sources/bin") + ICON_SUBPATH;

	QString fileName = QFileDialog::getOpenFileName( this, tr("Select Icon"),
		iconPath,
		tr("Images (*.png)"));

	if (fileName.isEmpty())
		return;

	if (!fileName.contains(iconPath)) {
		if (!QFile::copy(fileName, iconPath)) {
			gu_MessageBox(this, tr("File Copy Error!"), 
				tr("Cannot copy icon file to resource file! \n Same file name already exists or source file not found!"));
			return;
		}
	}

	fileName.remove(iconPath);
	m_ui->edtIconPath->setText(ICON_SUBPATH + fileName);
}

void ObjectPlaceSettingDlg::spnIconSize_change(int val)
{
	m_ui->sldIconSize->blockSignals(true);
	m_ui->sldIconSize->setValue(val);
	refreshPreview();
	m_ui->sldIconSize->blockSignals(false);
}

void ObjectPlaceSettingDlg::sldIconSize_change(int val)
{
	m_ui->spnIconSize->blockSignals(true);
	m_ui->spnIconSize->setValue(val);
	refreshPreview();
	m_ui->spnIconSize->blockSignals(false);
}

void ObjectPlaceSettingDlg::btnLineColor_click()
{
	QColor color = QColorDialog::getColor();
	if (color.isValid()) {
		m_lineColor = color.name();
		QString sheet = QString("background-color: %1").arg(m_lineColor);
		m_ui->btnLineColor->setStyleSheet(sheet);
		refreshPreview();
	}
}

void ObjectPlaceSettingDlg::spnLineThick_change(double val)
{
	m_ui->sldLineThick->blockSignals(true);
	m_ui->sldLineThick->setValue((int)(val * 10));
	refreshPreview();
	m_ui->sldLineThick->blockSignals(false);
}

void ObjectPlaceSettingDlg::sldLineThick_change(int val)
{
	m_ui->spnLineThick->blockSignals(true);
	m_ui->spnLineThick->setValue((double)val / 10);
	refreshPreview();
	m_ui->spnLineThick->blockSignals(false);
}

void ObjectPlaceSettingDlg::btnFillColor_click()
{
	QColor color = QColorDialog::getColor();
	if (color.isValid()) {
		m_fillColor = color.name();
		QString sheet = QString("background-color: %1").arg(m_fillColor);
		m_ui->btnFillColor->setStyleSheet(sheet);
		refreshPreview();
	}
}

void ObjectPlaceSettingDlg::btnTexturePath_click()
{
	QString texturePath = gdm_GetWorkPath() + TEXTURE_SUBPATH;
	//QString texturePath = QString("E:/Work/_HaeSa/2.Sources/bin") + TEXTURE_SUBPATH;

	QString fileName = QFileDialog::getOpenFileName( this, tr("Select Texture"),
		texturePath,
		tr("Images (*.png *.gif *.jpg)"));

	if (fileName.isEmpty())
		return;

	if (!fileName.contains(texturePath)) {
		if (!QFile::copy(fileName, texturePath)) {
			gu_MessageBox(this, tr("File Copy Error!"), 
				tr("Cannot copy texture file to resource file! \n Same file name already exists or source file not found!"));
			return;
		}
	}

	fileName.remove(texturePath);
	m_ui->edtTexturePath->setText(TEXTURE_SUBPATH + fileName);
}
