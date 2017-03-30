#include "pngbrowser.h"
#include "pngview.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtCore/QDir>

PngBrowser::PngBrowser(QWidget *parent)
	: QDialog(parent)
{
	m_IconView=new PngView();

	QHBoxLayout* layout1=new QHBoxLayout();
	QLabel   *label=new QLabel;
	label->setText(tr("Icon Category"));

	layout1->addWidget(label);
	m_CategoryBox=new QComboBox();
	layout1->addWidget(m_CategoryBox);

	QHBoxLayout*layout2=new QHBoxLayout();
	m_OkButton=new QPushButton(tr("Ok"));
	m_CancalButton=new QPushButton(tr("Cancel"));


	QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	layout2->addItem(horizontalSpacer);

	layout2->addWidget(m_OkButton);
	layout2->addWidget(m_CancalButton);
	QVBoxLayout* totallayout=new QVBoxLayout;
	totallayout->addLayout(layout1);
	totallayout->addWidget(m_IconView);
	totallayout->addLayout(layout2);

	this->setLayout(totallayout);

	m_ParentPath="";

	connect(m_CategoryBox,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setChildDir(const QString&)));
	connect(m_OkButton,SIGNAL(clicked()),this,SLOT(onOk()));
	connect(m_CancalButton,SIGNAL(clicked()),this,SLOT(onCancel()));
}

PngBrowser::~PngBrowser()
{

}

QString PngBrowser::getPngFileName(QString path)
{

	m_ParentPath=path;

	QDir dir(path);
	dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks);

	QFileInfoList list = dir.entryInfoList();
	
	for (int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);
		QString label=fileInfo.completeBaseName();

		if(label.isEmpty())
			continue;
		if(label==".")
			continue;
		m_CategoryBox->addItem(label);
	}
	int result=exec();

	if(result==Accepted)
	{
		QList<QTableWidgetItem*> itemlist=m_IconView->selectedItems();

		if(itemlist.size()==0)
			return "";
		QString name=itemlist[0]->text();

		QString res;

		res="/res/icon/"+m_CategoryBox->currentText()+"/"+name+".png";
		return res;
		
	}
	else if(result==Rejected)
		return "";

	return "";
}


void PngBrowser::setChildDir(const QString &path)
{
	QDir dir(m_ParentPath+"/"+path);
	
	QString extension="*.png";
	QStringList pngfiles;
	pngfiles = dir.entryList(QStringList(extension),QDir::Files | QDir::NoSymLinks);

	int columcount=m_IconView->columnCount();
	int row=pngfiles.size()/columcount+1;
	m_IconView->setRowCount(row);
	
	m_IconView->clear();
	for(int i=0;i<pngfiles.size();i=i+columcount)
	{
		for(int j=0;j<columcount;j++)
		{
			if(i+j>=pngfiles.size())
				continue;

			QTableWidgetItem *newItem = new QTableWidgetItem(pngfiles[i+j].left(pngfiles[i+j].indexOf(".")));
			newItem->setIcon(QIcon(QPixmap(m_ParentPath+"/"+path+"/"+pngfiles[i+j])));
			m_IconView->setItem(i/columcount, j, newItem);
		}
	}

	m_IconView->resizeColumnsToContents();
	m_IconView->resizeRowsToContents();

	int w=0;
	for(int i=0;i<columcount;i++)
		w+=m_IconView->columnWidth(i);

	resize(w,height());
	m_IconView->setCurrentCell(0,0);
}

void PngBrowser::onOk()
{
	accept();
}

void PngBrowser::onCancel()
{
	reject();
}