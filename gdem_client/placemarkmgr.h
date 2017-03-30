#ifndef PLACEMARKMGR_H
#define PLACEMARKMGR_H

#include "gdemclientbase.h"

class ResultTreeView;
class PlaceFolderDlg;

enum PLACEDLG_EDIT_STATE { PLACEDLG_NORMAL, PLACEDLG_ACCEPT, PLACEDLG_REJECT, PLACEDLG_VIEW_EDIT };

class PlaceMarkMgr
{
public:
    PlaceMarkMgr();
	~PlaceMarkMgr();

	void SetResultTreeView(ResultTreeView *currentView);

	void GetNewPlaceMarkData(PLACEMARK_DATA *data, ItemType type);
	void GetPropertyPlaceData(PLACEMARK_DATA *data, bool isAdd);
	void GetEditPlaceData(PLACEMARK_DATA *data, unsigned int state);
	void GetApplyPlaceData(PLACEMARK_DATA *data, bool isAdd);
	void GetCancelPlaceData(PLACEMARK_DATA *data, bool isAdd);
	void GetSnapshotPlaceData(PLACEMARK_DATA *data);
	void GetResetPlaceData(PLACEMARK_DATA *data);
	void GetNewPlaceFolderData(PLACEMARK_DATA *data);

	bool MouseClickEvent(const CursorState *event);

protected:
	PlaceMarkMap	*m_placeMarkData;
	ResultTreeView	*m_resultTreeView;

	PLACEMARK_DATA	m_propertyOrgData;

	unsigned int	GetSelectKey();
	bool IsSelectedMark(PLACEMARK_DATA *data);
	void GetGlobalPos(const int x, const int y, QPoint *pos);
};

#endif // PLACEMARKMGR_H
