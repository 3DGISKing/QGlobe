#include "stable.h"
#include "colorrampdlg.h"
#include "GDM_DataMgr.h"
#include "ColorRamp.h"

ColorRampDlg::ColorRampDlg(QWidget *parent,IColorRamp* colorramp)
	: QDialog(parent)
{
	ui.setupUi(this);

	Q_ASSERT(colorramp!=NULL);

	QList<IColorRamp*> colorramplist=g_pGDMDataMgr->m_ColorRampList;

	int rampindex=0;
	
	for(int i=0;i<colorramplist.size();i++)
	{
		AlgorithmicColorRamp* ramp=(AlgorithmicColorRamp*)colorramplist[i];

		if(ramp==colorramp)
			rampindex=i;

		QPixmap pix=ramp->ToPixmap();

		QIcon icon; 
		icon.addPixmap(pix);

		int val=(int)ramp;
		QVariant v(val);

		ui.comboBox->addItem(icon,"",v);
	}
	ui.comboBox->setIconSize(QSize(256,10));

	_ColorRamp=colorramplist[rampindex];
	ui.comboBox->setCurrentIndex(rampindex);

	connect(ui.pushButtonOk,SIGNAL(clicked()),this,SLOT(onClickedPushButtonOk()));
	connect(ui.pushButtonCancel,SIGNAL(clicked()),this,SLOT(onClickedPushButtonCancel()));
	connect(ui.comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onComboBoxChanged(int)));
}

ColorRampDlg::~ColorRampDlg()
{

}

void ColorRampDlg::onComboBoxChanged(int index)
{
	QVariant v=ui.comboBox->itemData(index);

	_ColorRamp=(IColorRamp*)v.toInt();

}

void ColorRampDlg::onClickedPushButtonOk()
{
	accept();
}

void ColorRampDlg::onClickedPushButtonCancel()
{
	reject();
}