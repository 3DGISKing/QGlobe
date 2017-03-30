#ifndef KEY_FRAME_H
#define KEY_FRAME_H
#include <QMetaType>
class KeyFrame 
{
public:
	enum FrameType
	{
		Camera,
		Video,
		Html,
		FlashStrings
	};
	KeyFrame();
	
	const FrameType  GetType()   {return m_Type;}
	virtual int      totalTime()=0; //in milliseconds
	virtual QString  label()=0;
	float            totalTimeSeconds(){return totalTime()/1000.0;}
protected:
	FrameType m_Type;
};

class CameraKeyFrame: public KeyFrame
{
public:
	CameraKeyFrame();
	CameraKeyFrame(const CameraKeyFrame& ckf);
	~CameraKeyFrame(){}

	inline QString getName()    {return m_Name;}
	inline double getLongitude(){return m_Longitude;}
	inline double getLatitude (){return m_Latitude;}
	inline double getAltitude() {return m_Altitude;}
	inline double getHeight()   {return m_Height;}
	inline double getTilt()     {return m_Tilt;}
	inline double getHeading()  {return m_Heading;}
	inline int    getGotoTime() {return m_GotoTime;}
	inline int    getStopTime() {return m_StopTime;}

	inline void setName(QString s)    {m_Name=s;}
	inline void setLongitude(double d) {m_Longitude=d;}
	inline void setLatitude(double d) {m_Latitude=d;}
	inline void setAltitude(double d) {m_Altitude=d;}
	inline void setHeight(double d) {m_Height=d;}
	inline void setHeading(double d) {m_Heading=d;}
	inline void setTilt(double d) {m_Tilt=d;}
	inline void setGotoTime(int d) {m_GotoTime=d;}
	inline void setStopTime(int d) {m_StopTime=d;}
	
	virtual int totalTime();
	virtual QString label();
private:
	QString m_Name;
	double m_Longitude;
	double m_Latitude;
	double m_Altitude;
	double m_Height;
	double m_Heading;
	double m_Tilt;

	int    m_GotoTime; //in millisecond
	int    m_StopTime; //in millisecond
};

class VideoKeyFrame: public KeyFrame
{
public:
	VideoKeyFrame();
	VideoKeyFrame(const VideoKeyFrame& ckf);
	~VideoKeyFrame(){}

	virtual int totalTime();
	virtual QString label();
	
	inline  void    setFileName(QString s) {	m_VideoFileName=s;	}
	inline  QString getFileName()          {return m_VideoFileName;}
	inline  void    setTotalTime(int d)    {m_TotalTime=d;}
private:
	QString     m_VideoFileName;
	int         m_TotalTime;         //in millisecond
};

Q_DECLARE_METATYPE(CameraKeyFrame)
Q_DECLARE_METATYPE(VideoKeyFrame)

#endif