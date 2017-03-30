#ifndef PLACEFOLDERDLG_H
#define PLACEFOLDERDLG_H

#include <QtGui/QDialog>

#include "gdemclientbase.h"

namespace Ui {
    class PlaceFolderDlg;
}

class PlaceMarkMgr;

class PlaceFolderDlg : public QDialog {
    Q_OBJECT
public:
	PlaceFolderDlg(QWidget *parent = 0, bool isAdd = true);
    ~PlaceFolderDlg();

	virtual void accept();
	virtual void reject();

protected:
    void changeEvent(QEvent *e);
	virtual void showEvent(QShowEvent *event);

	void EnableOutputInfo();
	void DisableOutputInfo();
	void UpdateOutputInfo();

	PlaceMarkMgr	*m_placeMarkMgr;
	PLACEMARK_DATA	m_placeMarkData;
	bool			m_snapview;
	bool			m_isAdd;
	unsigned int	m_state;

private:
    Ui::PlaceFolderDlg *m_ui;

private slots:
	void SetFocusOkButton();
	void SetFolderName();
	void SetDescription();
	void SetViewLatitude();
	void SetViewLongitude();
	void SetViewRange();
	void SetViewHeading();
	void SetViewTilt();
	void SnapshotView();
	void ResetView();
};

#endif // PLACEFOLDERDLG_H
