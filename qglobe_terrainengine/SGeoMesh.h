
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

				//¼ÑÊÌ¼êlatlongËæ Ì©ÃÍ Áì¼Ó»ô¶®Áì±Ë¶¦ ÊĞ²÷¸Ò¸âË¼ ³ŞËÁ±á °¯³Ş.
				//latlonÌ© »ô¶®¹ÖÀË¶£¶¦ ¼³°öÃùÂ×ÊŞ z¾¢Ëºµá º¼¼³ÂÙ³Ş.
				//ËËÆÁ Z¾¢Ë¼ ÃÍ³ëÂÙ ¼°Ëæº·Ì© Êà¼éºº±á ¸É´İ¸ÒÂáË¾ °¡¶®¿Ö³Ş.
				//¹£°ü±á z¾¢Ë¾ ¹ÖÀË¶£¼¬Â×ÊŞ x¾¢Ë¾ ÊĞ²÷³Ş.
				//z¾¢±á ÊĞÊÌ¼ê x¾¢Ë¾ ¹ÖÀË¶£¼¬Â×ÊŞ y¾¢Ë¾ ÊĞ²÷³Ş.
				//ËËµÖ±Â ÊĞÊÌ¼ê »ô¶®Áì±ËÌ© y¾¢Ë¼ ¼ê¹£¸ÒÂáË¾ ÂáÂÙ³Ş.

				double                    Longitude,Latitude;

				///********************************************
				//Êï¹¶½¤ÀâÌ© °ê¹¢»ô¶®Áì±ËÌ© Ìº¼°Ì© ²ÚËË 
				//********************************************/
				double                    Height;

				//·ô»ÕÌ© °ê¹¢»ô¶®Áì±ËÌ© y¾¢¸ÒÂá±á longitude, latitudeËæ Ì©ÃÍ °Î¼³´ó²÷ ·ô»ÕÌ© ¼Ú»ª¼°
				//Ëæº· ÊĞË¼ Áì¼Ó»ô¶®Áì±ËÌ© y¾¢¸ÒÂáº¡ËËÌ© °¢
				
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