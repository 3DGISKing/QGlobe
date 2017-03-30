#include "layertreeitem.h"
#include "Layer.h"
#include "GDM_DataMgr.h"
#include "gdemclient.h"

#define DataRole Qt::UserRole+1

LayerTreeItem::LayerTreeItem(QString text)
:QStandardItem(text)
{
	setCheckable(true);
	setEditable(false);

	m_exclusiveChilditem=NULL;
	m_checkedItemMustBeExist=false;
}

LayerTreeItem::LayerTreeItem(QString text,QString iconfilename)
:QStandardItem(text)
{
	setCheckable(true);
	setEditable(false);
	setIcon(QIcon(iconfilename));

	m_exclusiveChilditem=NULL;
	m_checkedItemMustBeExist=false;
}

LayerTreeItem::LayerTreeItem(Layer* layer)
:QStandardItem()
{
	setCheckable(true);
	setEditable(false);
	setText(layer->GetName());

	QString iconfilename=layer->GetIconFileName();

	iconfilename = g_app->GetResource(iconfilename);

	if(iconfilename!="")
		Q_ASSERT(QFile::exists(iconfilename));

	setIcon(QIcon(iconfilename));

	QVariant var;

	var.setValue((uint)layer);

	setData(var,DataRole);
	m_exclusiveChilditem=NULL;
	m_checkedItemMustBeExist=false;

	g_pGDMDataMgr->AddLayer(layer);
}

LayerTreeItem::~LayerTreeItem()
{
	Layer* layer=GetLayer();

	//all layer must not be destroyed before datamgr is destroyed.

	/*if(layer)
		delete layer;*/
}

Layer* LayerTreeItem::GetLayer()
{
	QVariant data=this->data(DataRole);
	return (Layer*)data.toUInt();
}

LayerTreeItem* LayerTreeItem::Root()
{
	LayerTreeItem* parent=(LayerTreeItem*)this->parent();

	if(parent==NULL)
		return this;

	return parent->Root();
}

void LayerTreeItem::SetState(Qt::CheckState state)
{
	setCheckState(state);
	Layer* layer=GetLayer();

	if(layer )
	{
		if(state==Qt::Checked)
			layer->SetVisible(true);
		else
			layer->SetVisible(false);
		
		g_pGDMDataMgr->SetLayer(layer);
	}
}

//set exclusive child item to parent recursively
void LayerTreeItem::PropagateExclusiveChildToParent()
{
	LayerTreeItem* parent=GetParent();

	if(parent)
	{
		parent->SetExclusiveChildItem(this);
		parent->PropagateExclusiveChildToParent();
	}
}

//propagate state from this to child exclusive item  recursively

void LayerTreeItem::PropagateStateToExclusiveChild(Qt::CheckState state)
{
	SetState(state);

	LayerTreeItem* ex=ExclusiveChildItem();

	if(ex)
		ex->PropagateStateToExclusiveChild(state);
}

void LayerTreeItem::PropagateStateToAllChilds(Qt::CheckState state)
{
	SetState(state);

	if(hasChildren())
	{
		for (int i = 0; i < rowCount(); i++)
		{
			LayerTreeItem* child=(LayerTreeItem*)this->child(i);
			child->PropagateStateToAllChilds(state);
		}
	}
}


void LayerTreeItem::PropagateEnabledState(bool state)
{
	setEnabled(state);

	if(hasChildren())
	{
		for (int i = 0; i < rowCount(); i++)
		{
			LayerTreeItem* child=(LayerTreeItem*)this->child(i);
			child->PropagateEnabledState(state);
		}
	}
}

void LayerTreeItem::OnClicked()
{
	Qt::CheckState state=checkState();

	LayerTreeItem* root=Root();

	if(root->ExclusiveChildItem())
		SetExclusiveCheckState(state);
	else
	{
		SetCheckState(state);
	}
}

void LayerTreeItem::PropagateStateToParent()
{
	LayerTreeItem* parent=GetParent();

	if(parent)
	{
		bool allchecked=true;

		for(int i=0;i<parent->rowCount();i++)
		{
			LayerTreeItem* child=(LayerTreeItem*)parent->child(i);

			if(child->checkState()!=Qt::Checked)
			{
				allchecked=false;
				break;
			}
		}

		if(allchecked)
			parent->SetState(Qt::Checked);
		
		bool allunchecked=true;

		for(int i=0;i<parent->rowCount();i++)
		{
			LayerTreeItem* child=(LayerTreeItem*)parent->child(i);

			if(child->checkState()!=Qt::Unchecked)
				allunchecked=false;
		}

		if(allunchecked)
			parent->SetState(Qt::Unchecked);

		if(!allchecked && !allunchecked)
			parent->SetState(Qt::PartiallyChecked);

		parent->PropagateStateToParent();
	}
}

void LayerTreeItem::SetCheckState(Qt::CheckState state)
{
	PropagateStateToAllChilds(state);
	PropagateStateToParent();
}

void LayerTreeItem::SetExclusiveChildItem(LayerTreeItem* item)
{
	Q_ASSERT(item->parent()==this);

	m_exclusiveChilditem=item;
}

void LayerTreeItem::SetExclusiveCheckState(Qt::CheckState state)
{	
	LayerTreeItem* parent=GetParent();

	if(parent==NULL)
	{
		if(GetCheckedItemMustBeExist())
		{	
			if(state==Qt::Unchecked)
				PropagateStateToExclusiveChild(Qt::Checked);	
		}
		else
			PropagateStateToExclusiveChild(state);
	}
	else
	{
		LayerTreeItem* root=Root();

		root->PropagateStateToExclusiveChild(Qt::Unchecked);
		this->PropagateExclusiveChildToParent();
		root->PropagateStateToExclusiveChild(Qt::Checked);
	}
}
