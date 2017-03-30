#ifndef PLACEDLG_H
#define PLACEDLG_H

#include <QtGui>
#include "treeitem.h"

class SideBar;
class PlaceMarkList;
class ResultTreeView;

namespace Ui {
    class PlaceDlg;
}

class PlaceDlg : public QDialog {
    Q_OBJECT
public:
    PlaceDlg(QWidget *parent = 0);
    ~PlaceDlg();
	bool Init();
	void Release();

	ResultTreeView *GetPlaceView()
	{ return m_placeView; }
	bool IsActiveView();
	void SetStatus(int status);
	int GetStatus()
	{ return m_status; }
    bool GetVisible()
	{ return m_visible; }
	int GetHeight(int sender);
	void SetRestoreHeight(int height)
	{ if (height)
		m_oldsize = height; }
	int GetRestoreHeight()
	{ return m_oldsize; }
	bool TempToMyPlaces();
	void SaveMyPlaces();

	void SaveStartPosition();
	void UpdateToolTip();

protected:
	void keyPressEvent ( QKeyEvent * event );
	bool CreateFrame();
	void DestroyFrame();

private:
    Ui::PlaceDlg *m_ui;

	ResultTreeView *m_placeView;
    SideBar *m_parent;
    QFrame *m_frame;

	bool m_visible;
    int m_status;
	int m_oldsize;


private slots:
	int ChangeState(bool press);
	void DeleteItem(bool press);
};

#endif // PLACEDLG_H
