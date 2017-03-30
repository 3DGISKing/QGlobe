#ifndef CAMERAKEYFRAMEDLG_H
#define CAMERAKEYFRAMEDLG_H

#include <QDialog>
#include "ui_camerakeyframedlg.h"

class CameraKeyFrame;

class CameraKeyFrameDlg : public QDialog
{
	Q_OBJECT

public:
	CameraKeyFrameDlg(QWidget *parent = 0);
	~CameraKeyFrameDlg();

	void SetData(CameraKeyFrame* ckf);
	CameraKeyFrame* getData(){return m_CameraKeyFrame;}
private slots:
	void onOk();
	void onCancel(){reject();}
	void onCapture();
private:
	Ui::CameraKeyFrameDlg ui;
	CameraKeyFrame* m_CameraKeyFrame;
};

#endif // CAMERAKEYFRAMEDLG_H
