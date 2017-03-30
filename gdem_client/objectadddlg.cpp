#include "ui_objectadddlg.h"
#include "objectadddlg.h"

#include "GDM_ObjectAddCmd.h"
#include "GDM_CommandMgr.h"
#include "gdemclient.h"
#include "renderfrm.h"
#include "proxy.h"
#include "msgdata.h"
#include "guiuty.h"
#include "mainwindow.h"
#include "config.h"
#include "../gds_common/GDSDefine.h"
#include "GDM_DataMgr.h"
#include "GDM_PyramidMgr.h"
#include "GDM_RequestMgr.h"

#include <QSqlQuery>

ObjectAddDlg::ObjectAddDlg(QWidget *parent)
	: CGDM_CommandDlg(parent),
	ui(new Ui::ObjectAddDlg)

{
	ui->setupUi(this);

	// create command
	CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
	m_pCommand = (CGDM_ObjectAddCmd*)pCmdMgr->CreateCmd(eGDM_CMD_OBJECT_ADD, this);

	ui->materialComboBox->addItem(("Solid"));
	ui->materialComboBox->addItem(("Solid 2Layer"));
	ui->materialComboBox->addItem(( "LightMap"));
	ui->materialComboBox->addItem(( "LightMapAdd"));
	ui->materialComboBox->addItem(( "LightMap M2"));
	ui->materialComboBox->addItem(("LightMap M4"));
	ui->materialComboBox->addItem(("LightMap Lighting"));
	ui->materialComboBox->addItem(("LightMap Lighting M2"));
	ui->materialComboBox->addItem(("LightMap Lighting M4"));
	ui->materialComboBox->addItem(( "Detail Map"));
	ui->materialComboBox->addItem(("Sphere Map"));
	ui->materialComboBox->addItem(( "Reflection 2 Layer"));
	ui->materialComboBox->addItem(("Transparent Add Color"));
	ui->materialComboBox->addItem(( "Transparent Alpha Channel"));
	ui->materialComboBox->addItem(( "Transparent Alpha Channel Ref"));
	ui->materialComboBox->addItem(( "Transparent Vertex Alpha"));
	ui->materialComboBox->addItem(("Transparent Reflection 2 Layer"));
	ui->materialComboBox->addItem(("Normal Map Solid"));
	ui->materialComboBox->addItem(("Normal Map Transparent Add Color"));
	ui->materialComboBox->addItem(( "Normal Map Transparent Vertex Alpha"));
	ui->materialComboBox->addItem(("Parallax Map Solid"));
	ui->materialComboBox->addItem(("Parallax Map Transparent Add Color"));
	ui->materialComboBox->addItem(( "Parallax Map Transparent Vertex Alpha"));
	ui->materialComboBox->addItem(( "One Texture Blend"));
	ui->materialComboBox->addItem(("Overlay Texture Blend"));

	connect(ui->openButton,SIGNAL(clicked()),this,SLOT(Open()));
	connect(ui->flipButton,SIGNAL(clicked()),this,SLOT(Flip()));

	connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(Close()));
	connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(Save()));

	connect(ui->setPositionButton,SIGNAL(clicked()),this,SLOT(SetPositionState()));
	connect(ui->setDirectionButton,SIGNAL(clicked()),this,SLOT(SetDirectionState()));
	connect(ui->setHeightButton,SIGNAL(clicked()),this,SLOT(SetHeightState()));

	connect(ui->azimuthDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(SetAzimuth()));
	connect(ui->heightDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(SetHeight()));
	connect(ui->longitudeDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(SetLongitude(double)));
	connect(ui->latitudeDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(SetLatitude(double)));

	connect(ui->scalingAllAxisButton,SIGNAL(clicked()),this,SLOT(SetScalingAllAxis()));
	connect(ui->scalingZAxisButton,SIGNAL(clicked()),this,SLOT(SetScalingZAxis()));
	connect(ui->scalingXAxisButton,SIGNAL(clicked()),this,SLOT(SetScalingXAxis()));
	connect(ui->scalingYAxisButton,SIGNAL(clicked()),this,SLOT(SetScalingYAxis()));
	connect(ui->scalingXYAxisButton,SIGNAL(clicked()),this,SLOT(SetScalingXYAxis()));

	connect(ui->RotateXAxisButton,SIGNAL(clicked()),this,SLOT(SetRotateXAxis()));
	connect(ui->RotateYAxisButton,SIGNAL(clicked()),this,SLOT(SetRotateYAxis()));

	connect(ui->backgroundMoveButton,SIGNAL(clicked()),this,SLOT(SetBackGroundMove()));
	connect(ui->selectButton,SIGNAL(clicked()),this,SLOT(SetSelect()));

	connect(ui->lengthDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(SetLength(double)));
	connect(ui->widthDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(SetWidth(double)));
	connect(ui->heightDoubleSpinBox1,SIGNAL(valueChanged(double)),this,SLOT(SetHeight1(double)));

	connect(ui->materialComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(SetMaterialType(int)));
	connect(ui->pushButtonDiffuseColor,SIGNAL(clicked()),this,SLOT(SetDiffuseColor()));
	connect(ui->pushButtonAmbientColor,SIGNAL(clicked()),this,SLOT(SetAmbientColor()));
	connect(ui->pushButtonSpecularColor,SIGNAL(clicked()),this,SLOT(SetSpecularColor()));
	connect(ui->pushButtonEmissiveColor,SIGNAL(clicked()),this,SLOT(SetEmissiveColor()));
	connect(ui->checkBoxLighting,SIGNAL(stateChanged(int)),this,SLOT(SetEnableLighting(int)));
	connect(ui->checkBoxFog,SIGNAL(stateChanged(int)),this,SLOT(SetEnableFog(int)));
	connect(ui->horizontalSliderShiness,SIGNAL(valueChanged(int)),this,SLOT(SetShininess(int)));

	connect(ui->Rotate90AroundXButton,SIGNAL(clicked()),this,SLOT(Rotate90AroundXAxis()));
	connect(ui->Rotate90AroundYButton,SIGNAL(clicked()),this,SLOT(Rotate90AroundYAxis()));
	connect(ui->InvertXAxisButton,SIGNAL(clicked()),this,SLOT(InvertXAxis()));

	connect(ui->pushButtonDelete,SIGNAL(clicked()),this,SLOT(DeleteObj()));

	QPoint mainPos = g_app->GetMainWindow()->pos();
	move(mainPos.x()+20, mainPos.y()+50);

	//Building Property

	ui->spinBoxFloor->setRange(0,BUILDING_MAX_FLOOR);

	QSqlQuery qry;

	qry.exec("SELECT loc_id, part1 FROM lst_location_name where level<=1 ORDER BY loc_id");
	while (qry.next())
		ui->comboBoxProvince->addItem(qry.value(1).toString(), qry.value(0).toInt());

	connect(ui->lineEditBuildingName,SIGNAL(textChanged(const QString&)),this,SLOT(SetBuildingName(const QString&)));
	connect(ui->lineEditPostName,SIGNAL(textChanged(const QString&)),this,SLOT(SetPostName(const QString&)));
	connect(ui->lineEditBan,SIGNAL(textChanged(const QString&)),this,SLOT(SetBan()));
	connect(ui->textEditMisc,SIGNAL(textChanged()),this,SLOT(SetMisc()));

	connect(ui->spinBoxFloor,SIGNAL(valueChanged(int)),this,SLOT(SetFloor()));

	if(g_app->GetConfig()->m_recentSelectedProvinceFor3DObjectAdding!=-1)
	{
		OnProvinceChange(g_app->GetConfig()->m_recentSelectedProvinceFor3DObjectAdding);
		ui->comboBoxProvince->setCurrentIndex(g_app->GetConfig()->m_recentSelectedProvinceFor3DObjectAdding);
	}
	if(g_app->GetConfig()->m_recentSelectedCountyFor3DObjectAdding!=-1)
	{
		OnCountyChange(g_app->GetConfig()->m_recentSelectedCountyFor3DObjectAdding);
		ui->comboBoxCounty->setCurrentIndex(g_app->GetConfig()->m_recentSelectedCountyFor3DObjectAdding);
	}

	if(g_app->GetConfig()->m_recentSelectedVilliageFor3DObjectAdding!=-1)
	{
		OnVilliageChange(g_app->GetConfig()->m_recentSelectedVilliageFor3DObjectAdding);
		ui->comboBoxVilliage->setCurrentIndex(g_app->GetConfig()->m_recentSelectedVilliageFor3DObjectAdding);
	}

	connect(ui->comboBoxProvince,SIGNAL(currentIndexChanged(int)),this,SLOT(OnProvinceChange(int)));
	connect(ui->comboBoxCounty,SIGNAL(currentIndexChanged(int)),this,SLOT(OnCountyChange(int)));
	connect(ui->comboBoxVilliage,SIGNAL(currentIndexChanged(int)),this,SLOT(OnVilliageChange(int)));

	ui->comboBoxObjectType->addItem(tr("Building"));
	ui->comboBoxObjectType->addItem(tr("Other"));

	connect(ui->comboBoxObjectType,SIGNAL(currentIndexChanged(int)),this,SLOT(SetObjectType(int)));

	ui->tabWidget->setCurrentIndex(0);
	
	connect(ui->checkBoxPositionSave,SIGNAL(stateChanged(int)),this,SLOT(SetImmediatelySave(int)));
	connect(ui->checkBoxMultiSelect,SIGNAL(stateChanged(int)),this,SLOT(SetMultiSelect(int)));

}


ObjectAddDlg::~ObjectAddDlg()
{
	delete ui; 
}

void ObjectAddDlg::OnProvinceChange(int id)
{
	ui->comboBoxCounty->clear();
	int fromId = ui->comboBoxProvince->itemData(id).toInt();
	int toId = fromId + 9999;

	QSqlQuery qry;
	qry.exec("SELECT loc_id, part2 FROM lst_location_name where level<=2 and loc_id>=" + QString("%1").arg(fromId) + " and loc_id<=" + QString("%1").arg(toId) + " ORDER BY loc_id");
	while (qry.next())
		ui->comboBoxCounty->addItem(qry.value(1).toString(), qry.value(0).toInt());

	ui->comboBoxCounty->setCurrentIndex(0);

	g_app->GetConfig()->m_recentSelectedProvinceFor3DObjectAdding=id;
	if(m_pCommand->_selectedGeoMesh)
		m_pCommand->_selectedGeoMeshUpdated=true;
}

void ObjectAddDlg::OnCountyChange(int id)
{
	ui->comboBoxVilliage->clear();
	int fromId = ui->comboBoxCounty->itemData(id).toInt();
	int toId = fromId + 99;

	QSqlQuery qry;
	qry.exec("SELECT loc_id, part3 FROM lst_location_name where loc_id>=" + QString("%1").arg(fromId) + " and loc_id<=" + QString("%1").arg(toId) + " ORDER BY loc_id");
	while (qry.next())
		ui->comboBoxVilliage->addItem(qry.value(1).toString(), qry.value(0).toInt());

	ui->comboBoxVilliage->setCurrentIndex(0);

	g_app->GetConfig()->m_recentSelectedCountyFor3DObjectAdding=id;
	if(m_pCommand->_selectedGeoMesh)
		m_pCommand->_selectedGeoMeshUpdated=true;
}

void ObjectAddDlg::OnVilliageChange(int id)
{
	if(id!=-1 && id!=0)
		m_pCommand->_buildingProperty._Loc_ID=ui->comboBoxVilliage->itemData(id).toInt();

	g_app->GetConfig()->m_recentSelectedVilliageFor3DObjectAdding=id;
	if(m_pCommand->_selectedGeoMesh)
		m_pCommand->_selectedGeoMeshUpdated=true;
}

void ObjectAddDlg::Open()
{
	if(m_pCommand->_toolState==CGDM_ObjectAddCmd::Select)
	{
		gu_MessageBox(NULL, tr("Warning"), tr("Can not open object in select state!"), MSGBOX_ICONINFO);
		return;
	}

	QString file_name = QFileDialog::getOpenFileName(this, tr("Select 3D Studio And Image file !"),
		g_app->GetConfig()->m_recentOpendPathFor3DObjectAdding,
		tr("3d studio And Image file  ( *.3ds *.bmp *.jpg *.png)"));

	if(file_name!="")
	{
		int pos=file_name.lastIndexOf("/");
	
		g_app->GetConfig()->m_recentOpendPathFor3DObjectAdding=file_name.left(pos);
		

		if(!m_pCommand->Open(file_name))
		{
		    gu_MessageBox(NULL, tr("Error"), tr("Failed to open 3ds file"), MSGBOX_ICONCRITICAL);
			return;
		}	
		else
		{
			ui->setDirectionButton->setEnabled(true);
			ui->setHeightButton->setEnabled(true);
			ui->backgroundMoveButton->setEnabled(true);
			ui->scalingAllAxisButton->setEnabled(true);
			ui->scalingXAxisButton->setEnabled(true);
			ui->scalingYAxisButton->setEnabled(true);
			ui->scalingZAxisButton->setEnabled(true);

			ui->longitudeDoubleSpinBox->setEnabled(true);
			ui->latitudeDoubleSpinBox->setEnabled(true);
			ui->longitudeDoubleSpinBox->setEnabled(true);
			ui->heightDoubleSpinBox->setEnabled(true);
			ui->azimuthDoubleSpinBox->setEnabled(true);
			ui->lengthDoubleSpinBox->setEnabled(true);
			ui->widthDoubleSpinBox->setEnabled(true);
			ui->heightDoubleSpinBox1->setEnabled(true);

			ui->Rotate90AroundXButton->setEnabled(true);
			ui->flipButton->setEnabled(true);
		}
	}

	ui->filenameEdit->setText(file_name);
	ui->checkBoxPositionSave->setCheckState(Qt::Unchecked);
	m_pCommand->_SetPositionImmediatelySave=false;
}

void ObjectAddDlg::Flip()
{
	m_pCommand->Flip();
}

void ObjectAddDlg::Close()
{
	this->m_pCommand->OnEnd();
	g_app->GetConfig()->m_showObjectAdd = false;
	this->accept();
}

void ObjectAddDlg::reject()
{
	this->m_pCommand->OnCancel();
	g_app->GetConfig()->m_showObjectAdd = false;

	QDialog::reject();
}

void ObjectAddDlg::UpdateDialog()
{
	ui->longitudeDoubleSpinBox->blockSignals(true);
	ui->latitudeDoubleSpinBox->blockSignals(true);
	ui->heightDoubleSpinBox->blockSignals(true);
	ui->azimuthDoubleSpinBox->blockSignals(true);

	ui->widthDoubleSpinBox->blockSignals(true);
	ui->heightDoubleSpinBox1->blockSignals(true);
	ui->lengthDoubleSpinBox->blockSignals(true);

	ui->materialComboBox->blockSignals(true);
	ui->checkBoxFog->blockSignals(true);
	ui->checkBoxLighting->blockSignals(true);
	ui->horizontalSliderShiness->blockSignals(true);

	ui->lineEditPostName->blockSignals(true);
	ui->lineEditBuildingName->blockSignals(true);
	ui->comboBoxProvince->blockSignals(true);
	ui->comboBoxCounty->blockSignals(true);
	ui->comboBoxVilliage->blockSignals(true);
	ui->lineEditBan->blockSignals(true);
	ui->comboBoxObjectType->blockSignals(true);
	ui->spinBoxFloor->blockSignals(true);


	copyDataFromNode();
	UpdateOutputInfo();

	ui->longitudeDoubleSpinBox->blockSignals(false);
	ui->latitudeDoubleSpinBox->blockSignals(false);
	ui->heightDoubleSpinBox->blockSignals(false);
	ui->azimuthDoubleSpinBox->blockSignals(false);

	ui->widthDoubleSpinBox->blockSignals(false);
	ui->heightDoubleSpinBox1->blockSignals(false);
	ui->lengthDoubleSpinBox->blockSignals(false);

	ui->materialComboBox->blockSignals(false);
	ui->checkBoxFog->blockSignals(false);
	ui->checkBoxLighting->blockSignals(false);
	ui->horizontalSliderShiness->blockSignals(false);

	ui->lineEditPostName->blockSignals(false);
	ui->lineEditBuildingName->blockSignals(false);
	ui->comboBoxProvince->blockSignals(false);
	ui->comboBoxCounty->blockSignals(false);
	ui->comboBoxVilliage->blockSignals(false);
	ui->lineEditBan->blockSignals(false);
	ui->comboBoxObjectType->blockSignals(false);
	ui->spinBoxFloor->blockSignals(false);
}

void ObjectAddDlg::copyDataFromNode()
{
   

}

void ObjectAddDlg::UpdateOutputInfo()
{
	ui->longitudeDoubleSpinBox->setValue(m_pCommand->_GeoInfo.Longitude*gdm_RADTODEG);
	ui->latitudeDoubleSpinBox->setValue(m_pCommand->_GeoInfo.Latitude*gdm_RADTODEG);
	ui->heightDoubleSpinBox->setValue(m_pCommand->_GeoInfo.Height);
	ui->azimuthDoubleSpinBox->setValue(m_pCommand->_GeoInfo.Azimuth);


	ui->lineEditBuildingName->setText(m_pCommand->_buildingProperty._Name);
	ui->lineEditPostName->setText(m_pCommand->_buildingProperty._PostName);
	ui->lineEditBan->setText(m_pCommand->_buildingProperty._Ban);
	ui->spinBoxFloor->setValue(m_pCommand->_buildingProperty._Floor);

	int locid=m_pCommand->_buildingProperty._Loc_ID;

	int provinceid=(locid/10000)*10000;

	QVariant province(provinceid);

	int index=ui->comboBoxProvince->findData(province);

	if(index!=-1)
		ui->comboBoxProvince->setCurrentIndex(index);

	int countyid=(locid/100)*100;

	QVariant county(countyid);

	index=ui->comboBoxCounty->findData(county);

	if(index!=-1)
		ui->comboBoxCounty->setCurrentIndex(index);

	QVariant villiage(locid);

	index=ui->comboBoxVilliage->findData(villiage);

	if(index!=-1)
		ui->comboBoxVilliage->setCurrentIndex(index);

	if(m_pCommand->_sampleMesh || m_pCommand->_selectedGeoMesh)
	{
		core::aabbox3df	box ;

		if(m_pCommand->_sampleMesh)		
			box=m_pCommand->_sampleMesh->getBoundingBox();
		else if(m_pCommand->_selectedGeoMesh)
			box=m_pCommand->_selectedGeoMesh->getBoundingBox();

		int materialtype=-1;

		if(m_pCommand->_sampleMesh)
			materialtype=m_pCommand->_sampleMesh->getMeshBuffer(0)->getMaterial().MaterialType;
		else if(m_pCommand->_selectedGeoMesh)
			materialtype=m_pCommand->_selectedGeoMesh->getMeshBuffer(0)->getMaterial().MaterialType;
		
		if(materialtype!=-1)
			ui->materialComboBox->setCurrentIndex(materialtype);

		SMesh * mesh=NULL;

		if(m_pCommand->_sampleMesh)
			mesh=m_pCommand->_sampleMesh;
		else if(m_pCommand->_selectedGeoMesh)
			mesh=m_pCommand->_selectedGeoMesh;

		if(mesh)
		{	
			SColor diffusecolor,ambientcolor,specularcolor,emissivecolor;

			diffusecolor=mesh->getMeshBuffer(0)->getMaterial().DiffuseColor;
			ambientcolor=mesh->getMeshBuffer(0)->getMaterial().AmbientColor;
			specularcolor=mesh->getMeshBuffer(0)->getMaterial().SpecularColor;
			emissivecolor=mesh->getMeshBuffer(0)->getMaterial().EmissiveColor;

			m_DiffuseColor.setRgb(diffusecolor.getRed(),diffusecolor.getGreen(),diffusecolor.getBlue(),diffusecolor.getAlpha());
			m_AmbientColor.setRgb(ambientcolor.getRed(),ambientcolor.getGreen(),ambientcolor.getBlue(),ambientcolor.getAlpha());
			m_SpecularColor.setRgb(specularcolor.getRed(),specularcolor.getGreen(),specularcolor.getBlue(),specularcolor.getAlpha());
			m_EmissiveColor.setRgb(emissivecolor.getRed(),emissivecolor.getGreen(),emissivecolor.getBlue(),emissivecolor.getAlpha());

			QString sheet = QString("background-color: %1").arg(m_DiffuseColor.name());
			ui->pushButtonDiffuseColor->setStyleSheet(sheet);

			sheet = QString("background-color: %1").arg(m_AmbientColor.name());
			ui->pushButtonAmbientColor->setStyleSheet(sheet);

			sheet = QString("background-color: %1").arg(m_SpecularColor.name());
			ui->pushButtonSpecularColor->setStyleSheet(sheet);

			sheet = QString("background-color: %1").arg(m_EmissiveColor.name());
			ui->pushButtonEmissiveColor->setStyleSheet(sheet);
		
			if(mesh->getMeshBuffer(0)->getMaterial().Lighting)
				ui->checkBoxLighting->setCheckState(Qt::Checked);
			else
				ui->checkBoxLighting->setCheckState(Qt::Unchecked);

			if(mesh->getMeshBuffer(0)->getMaterial().FogEnable)
				ui->checkBoxFog->setCheckState(Qt::Checked);
			else
				ui->checkBoxFog->setCheckState(Qt::Unchecked);

			ui->horizontalSliderShiness->setValue(mesh->getMeshBuffer(0)->getMaterial().Shininess);
		}

		ui->lengthDoubleSpinBox->setValue(box.MaxEdge.X-box.MinEdge.X);
		ui->widthDoubleSpinBox->setValue(box.MaxEdge.Y-box.MinEdge.Y);
		ui->heightDoubleSpinBox1->setValue(box.MaxEdge.Z-box.MinEdge.Z);
	}
}

void ObjectAddDlg::copyDataToNode()
{

}

void ObjectAddDlg::SetAzimuth()
{
	this->m_pCommand->_GeoInfo.Azimuth=ui->azimuthDoubleSpinBox->value();
	
	if(this->m_pCommand->_sampleMesh)
		this->m_pCommand->SetSampleMeshLocation();
	else if(this->m_pCommand->_selectedGeoMesh)
		this->m_pCommand->SetSelectedGeoMeshLocation();
	
}

void ObjectAddDlg::SetHeight()
{
	this->m_pCommand->_GeoInfo.Height=ui->heightDoubleSpinBox->value();
	
	if(this->m_pCommand->_sampleMesh)
		this->m_pCommand->SetSampleMeshLocation();
	else if(this->m_pCommand->_selectedGeoMesh)
		this->m_pCommand->SetSelectedGeoMeshLocation();
}

void ObjectAddDlg::SetLongitude(double newval)
{
	this->m_pCommand->_GeoInfo.Longitude=newval*gdm_DEGTORAD;
	if(this->m_pCommand->_sampleMesh)
		this->m_pCommand->SetSampleMeshLocation();
	else if(this->m_pCommand->_selectedGeoMesh)
		this->m_pCommand->SetSelectedGeoMeshLocation();
}

void ObjectAddDlg::SetLatitude(double newval)
{
	this->m_pCommand->_GeoInfo.Latitude=newval*gdm_DEGTORAD;
	if(this->m_pCommand->_sampleMesh)
		this->m_pCommand->SetSampleMeshLocation();
	else if(this->m_pCommand->_selectedGeoMesh)
		this->m_pCommand->SetSelectedGeoMeshLocation();
}

void ObjectAddDlg::SetHeightState()
{
	m_pCommand->SetToolState(CGDM_ObjectAddCmd::SetAltitude);
}

void ObjectAddDlg::SetDirectionState()
{
	m_pCommand->SetToolState(CGDM_ObjectAddCmd::SetDirection);
}

void ObjectAddDlg::SetPositionState()
{
	m_pCommand->SetToolState(CGDM_ObjectAddCmd::SetPosition);
}

void ObjectAddDlg::SetScalingAllAxis()
{
	m_pCommand->SetToolState(CGDM_ObjectAddCmd::ScaleAllAxis);
}

void ObjectAddDlg::SetScalingXAxis()
{
    m_pCommand->SetToolState(CGDM_ObjectAddCmd::ScaleXAxis);
}

void ObjectAddDlg::SetScalingYAxis()
{
     m_pCommand->SetToolState(CGDM_ObjectAddCmd::ScaleYAxis);
}

void ObjectAddDlg::SetRotateXAxis()
{
	m_pCommand->SetToolState(CGDM_ObjectAddCmd::RoateXAxis);
}

void ObjectAddDlg::SetRotateYAxis()
{
	m_pCommand->SetToolState(CGDM_ObjectAddCmd::RoateYAxis);
}

void ObjectAddDlg::SetScalingZAxis()
{
	 m_pCommand->SetToolState(CGDM_ObjectAddCmd::ScaleZAxis);
}


void ObjectAddDlg::SetScalingXYAxis()
{
	m_pCommand->SetToolState(CGDM_ObjectAddCmd::ScaleXYAxis);
}

void ObjectAddDlg::SetBackGroundMove()
{
	m_pCommand->SetToolState(CGDM_ObjectAddCmd::BackgroundMove);
}

#include "GDM_SceneManager.h"
#include "Geo3dObjectSceneNode.h"

void ObjectAddDlg::SetSelect()
{
	m_pCommand->SetToolState(CGDM_ObjectAddCmd::Select);
	if(m_pCommand->_sampleMesh)
		m_pCommand->InvalidateSampleMesh();
}

void ObjectAddDlg::SetLength(double newval)
{
   if(m_pCommand->_sampleMesh==NULL && m_pCommand->_selectedGeoMesh==NULL)
   {
	   return;
   }
   m_pCommand->SetLength(newval);
}

void ObjectAddDlg::SetWidth(double newval)
{
	if(m_pCommand->_sampleMesh==NULL && m_pCommand->_selectedGeoMesh==NULL)
	{
		return;
	}
	m_pCommand->SetWidth(newval);
}

void ObjectAddDlg::SetHeight1(double newval)
{
	if(m_pCommand->_sampleMesh==NULL && m_pCommand->_selectedGeoMesh==NULL)
	{
		return;
	}
	m_pCommand->SetHeight(newval);	
}	

void ObjectAddDlg::Save()
{
	if(m_pCommand->_sampleMesh)
	{
		int ret = gu_MessageBox((QWidget*)g_app->GetMainWindow(), tr("GDEM Client"), tr("Are you sure to save? Please check object type!"), MSGBOX_IDOKCANCEL|MSGBOX_ICONQUESTION);
		if(ret != MSGBOX_IDOK) return;	

		//check validity of building property.
		if(m_pCommand->_objectType==CGDM_ObjectAddCmd::Building)
			if(m_pCommand->_buildingProperty._Name.isEmpty() ||  m_pCommand->_buildingProperty._PostName.isEmpty() ||
				m_pCommand->_buildingProperty._Loc_ID==-1)
			{
				int ret=gu_MessageBox(NULL,	tr("Info"),	tr("There exists empty field! Do you want to continue?"),
					MSGBOX_IDYESNO|MSGBOX_ICONQUESTION);
				if(ret!=MSGBOX_IDYES) 	return ;
			}

		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

		m_pCommand->Save();
		g_pGDMDataMgr->m_pRequestMgr->ClearRequest(E_GDM_MAINDATA_TYPE_3D);
		g_pGDMDataMgr->m_pRequestMgr->UpdateRequest(E_GDM_MAINDATA_TYPE_3D);

		QApplication::restoreOverrideCursor();
		return;
	}


	if(m_pCommand->_selectedGeoMesh && m_pCommand->_selectedGeoMeshUpdated)
	{  
		if(!m_pCommand->_MultiSelect && m_pCommand->_SelectedGeoMeshs.size()>1)
		{
			gu_MessageBox(NULL,tr("Warning"),tr("one more object is selected!"),MSGBOX_ICONINFO);
		}
		int ret = gu_MessageBox((QWidget*)g_app->GetMainWindow(), tr("GDEM Client"), tr("Are you sure to revised result?"), MSGBOX_IDOKCANCEL|MSGBOX_ICONQUESTION);
		if(ret == MSGBOX_IDOK)	
		{
			QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
			m_pCommand->SetSelectState(false);

			foreach(SGeoMesh* mesh,m_pCommand->_SelectedGeoMeshs)
				m_pCommand->SaveRevisedGeoMesh(mesh);
		
			m_pCommand->_SelectedGeoMeshs.clear();
			m_pCommand->_selectedGeoMeshUpdated=false;
			m_pCommand->_selectedGeoMesh=NULL;

			g_pGDMDataMgr->Clear3DCache();
			g_pGDMDataMgr->m_pRequestMgr->ClearRequest(E_GDM_MAINDATA_TYPE_3D);
			g_pGDMDataMgr->m_pRequestMgr->UpdateRequest(E_GDM_MAINDATA_TYPE_3D);

			QApplication::restoreOverrideCursor();
		}
		else
		{
			QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
			g_pGDMDataMgr->m_pRequestMgr->ClearRequest(E_GDM_MAINDATA_TYPE_3D);
			g_pGDMDataMgr->m_pRequestMgr->UpdateRequest(E_GDM_MAINDATA_TYPE_3D);
			QApplication::restoreOverrideCursor();
		}
		return;
	}

	gu_MessageBox((QWidget*)g_app->GetMainWindow(), tr("GDEM Client"), tr("No save data!"), MSGBOX_ICONCRITICAL);
}

void ObjectAddDlg::DeleteObj()
{
	if(!m_pCommand->_selectedGeoMesh)
	{
		gu_MessageBox(NULL, tr("GDEM Client"), tr("No Mesh selected!"), MSGBOX_ICONCRITICAL);
		return;
	}

	int ret = gu_MessageBox(NULL, tr("GDEM Client"), tr("Are you sure to delete ?"), MSGBOX_IDOKCANCEL|MSGBOX_ICONQUESTION);
	if(ret != MSGBOX_IDOK) return;	

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	m_pCommand->DeleteSelectedGeoMeshs();
	g_pGDMDataMgr->Clear3DCache();
	g_pGDMDataMgr->m_pRequestMgr->ClearRequest(E_GDM_MAINDATA_TYPE_3D);
	g_pGDMDataMgr->m_pRequestMgr->UpdateRequest(E_GDM_MAINDATA_TYPE_3D);

	QApplication::restoreOverrideCursor();

	m_pCommand->_selectedGeoMesh=NULL;
}

void ObjectAddDlg::keyPressEvent(QKeyEvent * event )
{
	switch(event->key())
	{
#ifdef DEBUG
	case Qt::Key_F1	:
		g_app->GetConfig()->m_isShowTerrainWireframe=!(g_app->GetConfig()->m_isShowTerrainWireframe);
		break;
	case Qt::Key_F2	:
		g_app->GetConfig()->m_isShowRenderTileName=!(g_app->GetConfig()->m_isShowRenderTileName);
		break;
	case Qt::Key_F3	:
		g_app->GetConfig()->m_isShowRenderTileBoundry=!(g_app->GetConfig()->m_isShowRenderTileBoundry);
		break;
#endif
	case Qt::Key_F4:
		m_pCommand->SetTopViewState();
		break;
	case Qt::Key_F5:
		m_pCommand->Save();
		break;
	case Qt::Key_O:
		this->Open();
	}
}

void ObjectAddDlg::SetMaterialType(int val)
{
	m_pCommand->SetMaterialType(val);
}

void ObjectAddDlg::SetDiffuseColor()
{
	m_DiffuseColor = QColorDialog::getColor(m_DiffuseColor);

	m_pCommand->SetDiffuseColor(m_DiffuseColor);

	QString sheet = QString("background-color: %1").arg(m_DiffuseColor.name());
	ui->pushButtonDiffuseColor->setStyleSheet(sheet);
}

void ObjectAddDlg::SetAmbientColor()
{
	m_AmbientColor = QColorDialog::getColor(m_AmbientColor);

	m_pCommand->SetAmbientColor(m_AmbientColor);

	QString sheet = QString("background-color: %1").arg(m_AmbientColor.name());
	ui->pushButtonAmbientColor->setStyleSheet(sheet);
}

void ObjectAddDlg::SetSpecularColor()
{
	m_SpecularColor = QColorDialog::getColor(m_SpecularColor);

	m_pCommand->SetSpecularColor(m_SpecularColor);

	QString sheet = QString("background-color: %1").arg(m_SpecularColor.name());
	ui->pushButtonSpecularColor->setStyleSheet(sheet);
}

void ObjectAddDlg::SetEmissiveColor()
{
	m_EmissiveColor = QColorDialog::getColor(m_EmissiveColor);

	m_pCommand->SetEmissiveColor(m_EmissiveColor);

	QString sheet = QString("background-color: %1").arg(m_EmissiveColor.name());
	ui->pushButtonEmissiveColor->setStyleSheet(sheet);
}

void ObjectAddDlg::SetEnableLighting(int state)
{
	if(state==Qt::Checked)
		m_pCommand->SetEnableLighting(true);
	else
		m_pCommand->SetEnableLighting(false);
}

void ObjectAddDlg::SetEnableFog(int state)
{
	if(state==Qt::Checked)
		m_pCommand->SetEnableFog(true);
	else
		m_pCommand->SetEnableFog(false);
}

void ObjectAddDlg::SetShininess(int val)
{
	m_pCommand->SetShininess((float)val);
}


void ObjectAddDlg::Rotate90AroundXAxis()
{
	m_pCommand->Rotate90AroundXAxis();
}

void ObjectAddDlg::Rotate90AroundYAxis()
{
	m_pCommand->Rotate90AroundYAxis();
}

void ObjectAddDlg::InvertXAxis()
{
	m_pCommand->InvertXAxis();
}


void ObjectAddDlg::Normalize()
{
	if(m_pCommand->_sampleMesh)
		m_pCommand->NormalizeMesh(m_pCommand->_sampleMesh);
	else if(m_pCommand->_selectedGeoMesh)
		m_pCommand->NormalizeMesh(m_pCommand->_selectedGeoMesh);

}

void ObjectAddDlg::SetBuildingName(const QString& newtext)
{
	m_pCommand->_buildingProperty._Name=newtext;
	if(m_pCommand->_selectedGeoMesh)
		m_pCommand->_selectedGeoMeshUpdated=true;
}
void ObjectAddDlg::SetPostName(const QString& newtext)
{
	m_pCommand->_buildingProperty._PostName=newtext;
	if(m_pCommand->_selectedGeoMesh)
		m_pCommand->_selectedGeoMeshUpdated=true;
}

void ObjectAddDlg::SetBan()
{
	m_pCommand->_buildingProperty._Ban=ui->lineEditBan->text();
	if(m_pCommand->_selectedGeoMesh)
		m_pCommand->_selectedGeoMeshUpdated=true;
}

void ObjectAddDlg::SetFloor()
{
	m_pCommand->_buildingProperty._Floor=ui->spinBoxFloor->value();
	if(m_pCommand->_selectedGeoMesh)
		m_pCommand->_selectedGeoMeshUpdated=true;
}

void ObjectAddDlg::SetMisc()
{
	m_pCommand->_buildingProperty._Misc=ui->textEditMisc->toPlainText();
	if(m_pCommand->_selectedGeoMesh)
		m_pCommand->_selectedGeoMeshUpdated=true;
}

void ObjectAddDlg::SetObjectType(int type)
{
	m_pCommand->SetObjectType((CGDM_ObjectAddCmd::ObjectType)type);

	if(ui->comboBoxObjectType->currentIndex()==(int)CGDM_ObjectAddCmd::Building)
		ui->tabWidget->addTab(ui->buildingPropertyTab,tr("Building Property"));
	else
	{
		int index=ui->tabWidget->indexOf(ui->buildingPropertyTab);
		ui->tabWidget->removeTab(index);
	}
	if(m_pCommand->_selectedGeoMesh)
		m_pCommand->_selectedGeoMeshUpdated=true;
}



void ObjectAddDlg::closeEvent(QCloseEvent *e)
{
	reject();
}

void ObjectAddDlg::SetImmediatelySave(int state)
{
	if(state==Qt::Checked)
		m_pCommand->_SetPositionImmediatelySave=true;
	else
		m_pCommand->_SetPositionImmediatelySave=false;
}

void ObjectAddDlg::SetMultiSelect(int state)
{
	if(state==Qt::Checked)
		m_pCommand->_MultiSelect=true;
	else
		m_pCommand->_MultiSelect=false;
}

