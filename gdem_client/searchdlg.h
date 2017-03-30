#ifndef SEARCHDLG_H
#define SEARCHDLG_H

#include <QtGui/QDialog>
#include <QtCore/QBasicTimer>
#include "GDM_CommandDlg.h"

class QFrame;
class SideBar;
class ResultTreeView;
class SearchThread;

class CGDM_MeasureAreaCmd;
class CGIS_Node;

namespace Ui {
    class SearchDlg;
}

class SearchDlg : public CGDM_CommandDlg
{
    Q_OBJECT
private:
	bool CreateFrame();
	void DestroyFrame();
	void SendTimeMessage();

	//int RequestData(QString key, int start = 0);
	int AnalyzeResponseData(QString &key, int itemkey, int res, void *resData);
	void SendDeleteHistoryMsg();

	Ui::SearchDlg  *m_ui;
	ResultTreeView *m_searchResultView;
	SideBar        *m_parent;
	QFrame         *m_frame;

	QBasicTimer		m_timer;

	SearchThread    *m_searchThread;

	bool m_visible;
	bool m_deleteHistory;

	int m_status;
	int m_oldsize;

	QMap<QString, int> m_tableNames;

	CGDM_MeasureAreaCmd *m_pCommand;

	QList<double> getMBR();

public:
    SearchDlg(QWidget *parent = 0);
    ~SearchDlg();
	bool                   Init();
	void                   Release();
	void                   Search(QString key, int start = 0, int itemkey = 0);
	inline ResultTreeView *GetSearchView() { return m_searchResultView; }
	inline bool            GetVisible() { return m_visible; }
	bool                   IsActiveView();
	void                   SetStatus(int status);
	inline int             GetStatus() { return m_status; }
	int                    GetHeight(int sender);
	inline void            SetRestoreHeight(int height) { if(height) m_oldsize = height; }
	inline int             GetRestoreHeight() { return m_oldsize; }
	void                   WaiteStop();
	void                   UpdateToolTip();

	void                   searchRangeData();
	void                   setSearchRegion(CGIS_Node *region);
	void                   setSearchStyle(int idx);

	virtual void			UpdateDialog(void);
	virtual void            hideEvent ( QHideEvent * event );
	virtual void            showEvent ( QShowEvent * event );

protected:
	virtual void timerEvent(QTimerEvent *event);
	virtual void keyPressEvent (QKeyEvent * event);

private slots:
	void SearchCommand();
	void ShowSearchContextMenu(QPoint pos);
	void SearchClick();
	int  ChangeState(bool press);
	void ClearList(bool press);
	void cmbSearchStyle_change(int style);
};

#endif // SEARCHDLG_H
