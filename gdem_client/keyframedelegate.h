
#include <QtGui/QItemDelegate>
#include <phonon/videowidget.h>
#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>

class	CameraKeyFrameDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	CameraKeyFrameDelegate(QObject *parent = 0);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class	VideoKeyFrameDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	VideoKeyFrameDelegate(QObject *parent = 0);
	~VideoKeyFrameDelegate();

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	Phonon::VideoWidget *m_videoWidget;
	Phonon::MediaObject m_MediaObject;
	Phonon::AudioOutput m_AudioOutput;
};
