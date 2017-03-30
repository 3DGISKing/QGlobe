#ifndef TILELATLONG_H
#define TILELATLONG_H

#include <QDialog>
#include "ui_tilelatlong.h"

class TileLatLong : public QDialog
{
	Q_OBJECT

public:
	TileLatLong(QWidget *parent = 0);
	~TileLatLong();

private:
	Ui::TileLatLongClass ui;

	int  m_tileX;
	int  m_tileY;
	int  m_tileLevel;
	double m_ulLat;
	double m_ulLong;
	double m_rbLat;
	double m_rbLong;
	double m_derived;

	int  m_LLtileX;
	int  m_LLtileY;
	int  m_LLtileLevel;

	int  m_M,m_N;
	double m_LLLat;
	double m_LLLong;

	void OnLeftBtn();
	void OnRightBtn();
	void Change_derived();
private slots:
		void Change_X();
		void Change_Y();
		void Change_Level();
		void Change_Lat();
		void Change_Long();
		void Change_LLlevel();
};

#endif // TILELATLONG_H
