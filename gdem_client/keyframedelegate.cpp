
#include "keyframedelegate.h"
#include "camerakeyframedlg.h"
#include "videokeyframedlg.h"
#include "keyframe.h"

CameraKeyFrameDelegate::CameraKeyFrameDelegate(QObject* parent)
:QItemDelegate(parent)
{
}

QWidget* CameraKeyFrameDelegate::createEditor(QWidget *parent,
											  const QStyleOptionViewItem & option,
											  const QModelIndex & index ) const
{
	CameraKeyFrameDlg *b=new CameraKeyFrameDlg(parent);
	return b;
}

void CameraKeyFrameDelegate::setEditorData(QWidget *editor,
										   const QModelIndex &index) const
{
	CameraKeyFrameDlg* dlg= static_cast<CameraKeyFrameDlg*>(editor);
	QVariant v=index.model()->data(index, Qt::UserRole);

	CameraKeyFrame ckf=v.value<CameraKeyFrame>();
	dlg->SetData(&ckf);
}

void CameraKeyFrameDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
										  const QModelIndex &index) const
{
	CameraKeyFrameDlg *dlg = static_cast<CameraKeyFrameDlg*>(editor);

	CameraKeyFrame* data=dlg->getData();
	QVariant v;
	v.setValue(*data);
	model->setData(index, v, Qt::UserRole);
	model->setData(index, data->label(), Qt::EditRole);
}

void CameraKeyFrameDelegate::updateEditorGeometry(QWidget *editor,
												  const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	/*int left=option.rect.left()+option.rect.width()/2;
	int top=option.rect.top()+option.rect.height()/2;

	editor->move(left,top);*/
}

VideoKeyFrameDelegate::VideoKeyFrameDelegate(QObject* parent)
:QItemDelegate(parent)
{
	m_videoWidget=new Phonon::VideoWidget();
	m_videoWidget->setVisible(false);
	Phonon::createPath(&m_MediaObject, m_videoWidget);
	Phonon::createPath(&m_MediaObject, &m_AudioOutput);
}

VideoKeyFrameDelegate::~VideoKeyFrameDelegate()
{
	delete m_videoWidget;
}

QWidget* VideoKeyFrameDelegate::createEditor(QWidget *parent,
											  const QStyleOptionViewItem & option,
											  const QModelIndex & index ) const
{
	VideoKeyFrameDlg *b=new VideoKeyFrameDlg(NULL);
	return b;
}

void VideoKeyFrameDelegate::setEditorData(QWidget *editor,
										   const QModelIndex &index) const
{
	VideoKeyFrameDlg* dlg= static_cast<VideoKeyFrameDlg*>(editor);

	QVariant v=index.model()->data(index, Qt::UserRole);
	VideoKeyFrame vkf=v.value<VideoKeyFrame>();

	dlg->setData(&vkf);
}

void VideoKeyFrameDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
										  const QModelIndex &index) const
{
	VideoKeyFrameDlg* dlg= static_cast<VideoKeyFrameDlg*>(editor);

	VideoKeyFrame* data=dlg->getData();
	QVariant v;
	v.setValue(*data);
	model->setData(index, v, Qt::UserRole);
	model->setData(index, data->label(), Qt::EditRole);
}

void VideoKeyFrameDelegate::updateEditorGeometry(QWidget *editor,
												  const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{

}

