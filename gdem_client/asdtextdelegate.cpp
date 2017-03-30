#include "asdtextdelegate.h"
#include "gdemclient.h"
#include "config.h"
#include "guiuty.h"
#include <QLineEdit>

asdTextDelegate::asdTextDelegate(QObject *parent)
	: QItemDelegate(parent)
{

}

asdTextDelegate::~asdTextDelegate()
{

}

QWidget *asdTextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QLineEdit *editor = new QLineEdit(parent);
	return editor;
}

void asdTextDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString value = index.data().toString();
	QLineEdit *textEdit = static_cast<QLineEdit*>(editor);
	textEdit->setText(value);
}

void asdTextDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QString oldVal = index.data().toString();
	QLineEdit *textEdit = static_cast<QLineEdit*>(editor);
	QString newVal = textEdit->text();

	double clonglat = 0.0;
	if(!newVal.isEmpty()) {
		clonglat = StringToValue(newVal);
		newVal = ValueToString(clonglat);
	}

	if(oldVal == newVal) return;
	model->setData(index, clonglat, Qt::EditRole);
}

void asdTextDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QRect rect = option.rect;
	editor->setGeometry(rect);
}

QString asdTextDelegate::ValueToString(double value) const
{
	QString str;
	switch(g_app->GetConfig()->m_llShowMode)
	{
	case OPTION_LL_10NOTATION:
		str = QString("%1%2").arg(value, 10, 'f', 6).arg(QChar(0x00B0));
		break;
	case OPTION_LL_DMS:
		str = gu_Degree2DMS(value, GEO_TYPE_LATITUDE);
		break;
	case OPTION_LL_10NOTATION_DM:
		str = gu_Degree2DM(value, GEO_TYPE_LATITUDE);
		break;
	default:
		break;
	}
	return str;
}

double asdTextDelegate::StringToValue(QString &str) const
{
	double value;
	switch(g_app->GetConfig()->m_llShowMode)
	{
	case OPTION_LL_10NOTATION:
		value = gu_Degree2Degree(str);
		break;
	case OPTION_LL_DMS:
		value = gu_DMS2Degree(str, GEO_TYPE_LATITUDE);
		break;
	case OPTION_LL_10NOTATION_DM:
		value = gu_DM2Degree(str, GEO_TYPE_LATITUDE);
		break;
	default:
		break;
	}
	return value;
}
