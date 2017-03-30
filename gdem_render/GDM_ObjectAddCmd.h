#pragma once

#include <QtCore/QMap>

#include "gdm_command.h"

#include "../gdem_libmain/Building.h"
#include "../gdem_scene/SampleMeshSceneNode.h"
#include "../gdem_terrainengine/IVideoDriver.h"
#include "../gdem_terrainengine/SAnimatedMesh.h"
#include "../gdem_terrainengine/SGeoMesh.h"

using namespace geo;
using namespace scene;
using namespace  video;

class CGDM_ObjectAddCmd :public CGDM_Command
{
	Q_DECLARE_TR_FUNCTIONS(CGDM_ObjectAddCmd)

	typedef enum ToolState
	{   	
		SetPosition,
		SetDirection,
		SetAltitude,
		ScaleAllAxis,
		ScaleXYAxis,
		ScaleXAxis,
		ScaleYAxis,
		ScaleZAxis,
		RoateXAxis,
		RoateYAxis,
		BackgroundMove,
		Select,
	};

	typedef enum ObjectType
	{
		Building,
		Other
	};

	public:		  
		CGDM_ObjectAddCmd(CGDM_CommandDlg *pCmdDlg = NULL);
		~CGDM_ObjectAddCmd(void);

		bool              Open(QString filename);
		bool              GetSampleMesh(QString filename);
		void              InvalidateSampleMesh();
		
		//this function needed for revise 
		//this will be invoked when user select object for revising
		void              SelectGeoMesh(int windowx,int windowy);
				
		void              SetTopViewState();
		void              SetSampleMeshLocation();
		void              SetToolState(ToolState state);
		
		virtual bool      OnMousePress(CursorState* mouseEvent);
		virtual bool      OnMouseRelease(CursorState* mouseEvent);
		virtual bool      OnMouseMove(CursorState* mouseEvent);
		virtual bool      OnMouseWheelEvent(CursorState* mouseEvent);
		virtual void      OnKeyPressEvent(KeyState keyState);
		virtual void      OnCancel();
		virtual void      OnEnd();

		//! save opened object to database
		void              Save();

		//!save opened object to given level 's database
		void              Save(int level);

		void              SaveRevisedGeoMesh(SGeoMesh* mesh);
	
		void              SetMaterialType(int val);
		void              SetDiffuseColor(QColor color);
		void              SetAmbientColor(QColor color);
		void              SetSpecularColor(QColor color);
		void              SetEmissiveColor(QColor color);
		void              SetEnableLighting(bool e);
		void              SetEnableFog(bool e);
		void              SetShininess(float f);

		void              SetSelectedGeoMeshLocation();
		void              SetSelectState(bool val);
			
		bool              SetLocation(int x,int y);
		void              SetLength(double length);
		void              SetWidth(double width);
		void              SetHeight(double height);
	
		void              DeleteSelectedGeoMeshs();
	 	void              DeleteGeoMesh(SGeoMesh* mesh);

		void              Flip();
		void              RotateMinus90AroundXAxis();
		void              Rotate90AroundXAxis();
		void              Rotate90AroundYAxis();
		void              InvertXAxis();

	  	void              NormalizeMesh(SMesh* mesh);

		inline void       SetObjectType(ObjectType type) {_objectType=type;}
		inline ObjectType GetObjectType() {return _objectType;}
private:
	SGeoMesh::GeoMeshHeader _GeoInfo;

	//SMesh that user opens for entering database
	//if _selectedMesh is already existed , clear it;
	SMesh*               _sampleMesh;

	//GeoMesh that user last selects for revising
	//if sample is already existed, clear it. 
	SGeoMesh*            _selectedGeoMesh;

	bool                 _selectedGeoMeshUpdated;

	IVideoDriver*        _driver;
	ToolState            _toolState;
	int                  _prevScreenX,_prevScreenY;

	CSampleMeshSceneNode*   _pSampleMeshSceneNode;

	//All 3d objects has an own type for example building, tree and so on
	ObjectType              _objectType;

	GeoBuildingProperty     _buildingProperty;

	bool                    _SetPositionImmediatelySave;
	QMap<qint64,SGeoMesh*>  _SelectedGeoMeshs; 
	bool                    _MultiSelect;

	friend class ObjectAddDlg;
};



          