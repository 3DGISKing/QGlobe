#ifndef GDM_MEASURE_AREA_CMD_H

#define GDM_MEASURE_AREA_CMD_H

#include "GDM_Command.h"

class CGIS_Area;
class GGIS_Node;

class CGDM_MeasureAreaCmd :	public CGDM_Command
{
public:
	CGDM_MeasureAreaCmd(CGDM_CommandDlg *pCmdDlg = NULL);
	~CGDM_MeasureAreaCmd(void);

public:

	// inherited function
	virtual bool OnMousePress(CursorState* mouseEvent);
	virtual bool OnMouseMove(CursorState* mouseEvent);
	virtual bool OnMouseRelease(CursorState* mouseEvent);
	virtual void OnKeyPressEvent(KeyState keyState);

	virtual CGIS_Node*	GetNode()	{return (CGIS_Node*)m_pAreaNode;}
	virtual void		OnEnd();
	virtual void		OnCancel();
	void		       Clear ();

	double  GetArea(); 
	double  GetMoveVelocity(){return m_MoveVelocity;}
	double  GetMoveDirection(){return m_MoveDirection;}
	double GetGrowVelocity(){return m_GrowVelocity;}

	void   SetMoveVelocity(double d){m_MoveVelocity=d;}
	void   SetMoveDirection(double d){m_MoveDirection=d;}
	void   SetGrowVelocity(double d) {m_GrowVelocity=d;}
	void   OnSimulation();
	void		Init(void);
private:

	int GetSelectTrackIndex(CursorState* mouseEvent);

	CGIS_Area			*m_pAreaNode;
	CGIS_Area			*m_pPrevAreaNode;

	double              m_MoveDirection;
	double              m_MoveVelocity;
	double              m_GrowVelocity;

	bool				m_blPress;
	bool				m_blActive;
	bool				m_blTrack;
	int					m_nCurTrackIndex;
	CursorState			m_cPressCursor;
	double				m_dTopHeight;

};


#endif