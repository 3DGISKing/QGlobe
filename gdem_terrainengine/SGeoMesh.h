
#ifndef __S_GEOMESH_H_INCLUDED__
#define __S_GEOMESH_H_INCLUDED__

#include "SMesh.h"

namespace geo
{
	namespace scene
	{

		struct SGeoMesh :public SMesh 
		{
			struct GeoMeshHeader
			{
				//by RSH 2014.3.19
				quint64 ID;
				//end RSH

				//���̼�latlong�� ̩�� ��ӻ�����˶� �в��Ҹ�˼ ������ ����.
				//latlon̩ �������˶��� ���������� z��˺�� �����ٳ�.
				//���� Z��˼ �ͳ��� ���溷̩ ��麺�� �ɴݸ���˾ �����ֳ�.
				//������ z��˾ ���˶������� x��˾ �в���.
				//z���� ���̼� x��˾ ���˶������� y��˾ �в���.
				//�˵ֱ� ���̼� �������̩ y��˼ �깣����˾ ���ٳ�.

				double                    Longitude,Latitude;

				///********************************************
				//�ﹶ����̩ �깢�������̩ ̺��̩ ���� 
				//********************************************/
				double                    Height;

				//����̩ �깢�������̩ y������� longitude, latitude�� ̩�� �μ���� ����̩ �ڻ���
				//�溷 ��˼ ��ӻ������̩ y�����ạ��̩ ��
				
				double                    Azimuth;
			};
		public:
			SGeoMesh()
			{
				Selected=false;
			}
			bool GetSelected()         {return Selected;}
			void SetSelected(bool val) {Selected=val;}

	
			GeoMeshHeader             GeoInfo;
			bool                      Selected;
			double                    OldLongitude,OldLatitude;//for revising

		};
	}
}

#endif