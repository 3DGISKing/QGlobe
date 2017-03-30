#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QtGui/QTabWidget>
#include "gdemclientbase.h"

class PlaceDlg;
class SearchDlg;
class ResultTreeView;
class LayersDlg;

class CGIS_Node;

enum ButtonState	{ BS_Disable, BS_Open, BS_Animation, BS_Close };
enum SenderType		{ ST_Search, ST_Place, ST_Layer };

#define BOTTOM_MARGIN			3
#define SIDEBAR_SEARCH_HEIGHT	200


typedef struct _StatePos
{
	int state;
	int sender;
	int height1;
	int height2;
	int height3;
} StatePos;

class SideBar : public QTabWidget
{
    Q_OBJECT
public:
    SideBar();
    ~SideBar();
	bool Init();
	void Release();
	// Menu Function
	void MoveToMyPlace();
	bool IsMoveToMyPlace();
	bool IsSaveAsPlaceMarkFile();
	void MoveStartPosition();
	void SetSelectItem(const PLACEMARK_DATA *data, bool focus = false);

	static PlaceMarkMap* GetPlaceMarkMap();
	ResultTreeView* GetActiveView();
	void ChangePanel(int sender = ST_Search);
	void AddPlaceMark(const PLACEMARK_DATA &data);
	void RemovePlaceMark(const PLACEMARK_DATA &data);
	void ModifyPlaceMark(const PLACEMARK_DATA &data);
	void ClearSearch();
	bool IsActiveSearchView();
	void UpdateToolTip();

	void setSearchRegion(CGIS_Node *region);
	void setSearchStyle(int i);

	SearchDlg *m_searchDlg;
	PlaceDlg  *m_placeDlg;
	LayersDlg *m_layersDlg;
private:
	bool CreateFrame();
	void DestroyFrame();
	StatePos *GetStatePos(int state, int sender = -1);

	int        m_activeView;
};

#endif // SIDEBAR_H
