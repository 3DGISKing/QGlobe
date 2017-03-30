#include "stable.h"
#include "licencedlg.h"
#include "key.h"
#include "guiuty.h"
#include "gdemclient.h"
#include "config.h"
#include "MainWindow.h"

LicenceDlg::LicenceDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	QString modelnumber=GetPhysicalDrive0ModelNumber();

	if(modelnumber.isEmpty())
	{
		gu_MessageBox(NULL,tr("GDEMClient"),tr("Excuse me. Inform me!"),MSGBOX_IDOK);
		return;
	}

	ui.lineEditModelNumber->setInputMask("HHHH-HHHH-HHHH-HHHH");

	ui.lineEditModelNumber->setText(FormatKey(modelnumber,16));
	ui.lineEditLicence->setInputMask("HHHH-HHHH-HHHH-HHHH");
	ui.lineEditLicence->setText("0000000000000000");
	connect(ui.pushButtonClose,SIGNAL(clicked()),this,SLOT(accept()));
	connect(ui.pushButtonLicence,SIGNAL(clicked()),this,SLOT(setLicence()));
	trynumber=0;
}

LicenceDlg::~LicenceDlg()
{

}


void LicenceDlg::setLicence()
{
	QString licence=ui.lineEditLicence->text();
	licence.replace("-","");

	QString formatedserial=FormatKey(GetPhysicalDrive0ModelNumber(), 16);

	if(licence==FormatKey(GetKey(formatedserial),16))
	{
		gu_MessageBox(NULL,tr("GDEMClient"),tr("Licence Ok"));
		
		g_app->GetConfig()->WriteLicence(licence);
		g_app->GetMainWindow()->DisableLicenceAction();
		accept();
	}
	else
	{
		gu_MessageBox(NULL,tr("GDEMClient"),tr("Invalid licence. Inform me"));
		trynumber++;
		ui.lineEditLicence->setText("");

		if(trynumber==3)
			accept();
	}
}