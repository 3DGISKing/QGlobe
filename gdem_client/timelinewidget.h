#include <QtGui/QTableWidget>
#include <QtGui/QLabel>

#include "keyframe.h"

class QAction;

class TimeLineDisplay: public QLabel
{
	Q_OBJECT
public:
	TimeLineDisplay(QWidget* parent=0);
	~TimeLineDisplay(){}

	int  totalScaleHeight();
	void setOrigTime(double t){m_OrigTime=t;}
	void setWidthPerSec(int val){m_WidthPerSec=val;}
protected:
	virtual void paintEvent(QPaintEvent * event);
private:
	/*_____________________________________
	|
	|0      1      2       3       4       5  digitheight
	||______|______|_______|_______|_______|
	|______________________________________timelineheight
	--
	*/
	int m_WidthPerSec;
	int m_WidthPerOneScale;
	int m_ScaleHeight;
	int m_DigitHeight;
	int m_TimeLineHeight;
	double m_OrigTime; //in second
};

class TimeLineWidget : public QTableWidget
{
	Q_OBJECT
public:
	TimeLineWidget(QWidget* parent=0);
	~TimeLineWidget();

	virtual void      newFrame()=0;
	virtual void      pushFrame(KeyFrame* keyframe)=0;
	virtual	void      insertFrame(KeyFrame* para,int i)=0;
	virtual int       frameTime(int i)=0;   //milliseconds
	
	float             frameTimeSeconds(int i){return frameTime(i)/1000.0;}

	double            getOrigTime(); //in second
	qint64            totalTime();
	float             totalTimeSeconds(){return totalTime()/1000.0;}
	void              setWidthPerSec(int value);
	void              setScrollPos(int value);
protected:
	virtual void resizeEvent(QResizeEvent* event);

protected slots:
	void         onAdd();
	void         onDelete();
	void         onSliderChanged(int);
	virtual void onItemChanged(QTableWidgetItem*);
signals:
	void sliderChanged(int);
protected: 
	int              m_WidthPerSecond;
	QAction*         m_AddAction;
	QAction*         m_DeleteAction;
	TimeLineDisplay* m_TimeDisplay;
	QScrollBar *     m_ScrollBar;
};

class CameraTimeLineWidget: public TimeLineWidget
{
	Q_OBJECT
public:
	CameraTimeLineWidget(QWidget* parent=0);
	~CameraTimeLineWidget();

	virtual void       newFrame();
	virtual void       pushFrame(KeyFrame* keyframe);
	virtual void       insertFrame(KeyFrame* para,int i);
	CameraKeyFrame     getFrame(int i);
	virtual int        frameTime(int i);   //milliseconds
private slots: 
	void               showContextMenu(QPoint);
	void               onGoto();
private:
	QAction*           m_GotoAction;
};

class VideoTimeLineWidget: public TimeLineWidget
{
	Q_OBJECT
public:
	VideoTimeLineWidget(QWidget* parent=0);
	~VideoTimeLineWidget();

	virtual void      newFrame();
	virtual void      pushFrame(KeyFrame* keyframe);
	virtual void      insertFrame(KeyFrame* para,int i);
	VideoKeyFrame     getFrame(int i);
	virtual int       frameTime(int i);   //milliseconds
protected:
	void onItemChanged(QTableWidgetItem*);
private slots:
	void              showContextMenu(QPoint);
};