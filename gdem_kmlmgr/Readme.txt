***************************************** libkml과 련동하기 위한 환경설정
(2009.08.03 RiSungChol)
------------------- Include Dir
..\libkml
..\libkml\kml
..\libkml\third_party
..\libkml\third_party\expat

------------------- LIB Dir
Link Library:
	libkml.lib

***************************************** kml_manager_lib.lib의 기능 및 구조
(2009.08.05 RiSungChol)
기능:
	Client프.의 UI와 libkml.lib(kml관리 library)을 련결시켜준다.
	
구조:
	UI(Client)	<----> kml_manager_client.lib	<----> libkml.lib