#pragma once

class IGIS_Progress
{
public:
	IGIS_Progress(void);
	~IGIS_Progress(void);
public:
	virtual void setProgress(double percent) = 0;
	virtual bool isStop() = 0; 
};
