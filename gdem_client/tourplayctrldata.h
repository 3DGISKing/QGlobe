#ifndef TOURPLAYCTRLDATA_H
#define TOURPLAYCTRLDATA_H

enum TourPlayState  { TPS_NO = -1, TPS_NORMAL, TPS_HOVER, TPS_ACTIVE, TPS_MAX };
enum TourPlayCtrlType
{
	TPCT_PREV_ON, TPCT_PREV_OFF, TPCT_PLAY_ON, TPCT_PLAY_OFF, 
	TPCT_NEXT_ON, TPCT_NEXT_OFF, TPCT_SLIDER, TPCT_THUMB, 
	TPCT_LOOP_ON, TPCT_LOOP_OFF, TPCT_EXIT,
    TPCT_MAX
};

#define PREV_ON_NOR			"tourprev_on_norm.png"
#define PREV_ON_HOV			"tourprev_on_hover.png"
#define PREV_ON_ACT			"tourprev_on_active.png"

#define PREV_OFF_NOR		"tourprev_off_norm.png"
#define PREV_OFF_HOV		"tourprev_off_hover.png"
#define PREV_OFF_ACT		"tourprev_off_active.png"

#define PLAY_ON_NOR			"tourplay_on_norm.png"
#define PLAY_ON_HOV			"tourplay_on_hover.png"
#define PLAY_ON_ACT			"tourplay_on_active.png"

#define PLAY_OFF_NOR		"tourplay_off_norm.png"
#define PLAY_OFF_HOV		"tourplay_off_hover.png"
#define PLAY_OFF_ACT		"tourplay_off_active.png"

#define NEXT_ON_NOR			"tournext_on_norm.png"
#define NEXT_ON_HOV			"tournext_on_hover.png"
#define NEXT_ON_ACT			"tournext_on_active.png"

#define NEXT_OFF_NOR		"tournext_off_norm.png"
#define NEXT_OFF_HOV		"tournext_off_hover.png"
#define NEXT_OFF_ACT		"tournext_off_active.png"

#define SLIDER_TRACK		"tour_slider_track.png"

#define THUMB_NOR			"tour_slider_thumb_norm.png"
#define THUMB_HOV			"tour_slider_thumb_hover.png"
#define THUMB_ACT			"tour_slider_thumb_active.png"

#define LOOP_ON_NOR			"tourloop_on_norm.png"
#define LOOP_ON_HOV			"tourloop_on_hover.png"
#define LOOP_ON_ACT			"tourloop_on_active.png"

#define LOOP_OFF_NOR		"tourloop_off_norm.png"
#define LOOP_OFF_HOV		"tourloop_off_hover.png"
#define LOOP_OFF_ACT		"tourloop_off_active.png"

#define EXIT_NOR			"tourexit_norm.png"
#define EXIT_HOV			"tourexit_hover.png"
#define EXIT_ACT			"tourexit_active.png"


static char *s_tourPlayImgNames[TPCT_MAX][TPS_MAX] =
{// kind        norm			hover			active
/*0-prev_on*/   {PREV_ON_NOR,	PREV_ON_HOV,	PREV_ON_ACT	},
/*1-prev_off*/  {PREV_OFF_NOR,	PREV_OFF_HOV,	PREV_OFF_ACT },
/*2-play_on*/   {PLAY_ON_NOR,	PLAY_ON_HOV,	PLAY_ON_ACT },
/*3-play_off*/  {PLAY_OFF_NOR,	PLAY_OFF_HOV,	PLAY_OFF_ACT },
/*4-next-on*/   {NEXT_ON_NOR,	NEXT_ON_HOV,	NEXT_ON_ACT },
/*5-next_off*/  {NEXT_OFF_NOR,	NEXT_OFF_HOV,	NEXT_OFF_ACT },
/*6-slider*/	{SLIDER_TRACK,	SLIDER_TRACK,	SLIDER_TRACK },
/*7-thumb*/		{THUMB_NOR,		THUMB_HOV,		THUMB_ACT },
/*8-loop_on*/   {LOOP_ON_NOR,	LOOP_ON_HOV,	LOOP_ON_ACT	},
/*9-loop_off*/  {LOOP_OFF_NOR,	LOOP_OFF_HOV,	LOOP_OFF_ACT},
/*10-exit*/		{EXIT_NOR,		EXIT_HOV,		EXIT_ACT }
};

typedef struct _TourPlayCtrlRegion
{
    int x1; // left top x;
    int y1; // left top y
    int w;	// widht
    int h;	// height
} TourPlayCtrlRegion;


static TourPlayCtrlRegion s_tourRegion[TPCT_MAX] =
{//               x1  y1    w    h
/*0-prev_on*/   { 15, 75,	20,	20 },
/*1-prev_off*/  { 15, 75,	20,	20 },
/*2-play_on*/   { 40, 78,	27, 27 },
/*3-play_off*/  { 40, 78,	27, 27 },
/*4-next-on*/   { 71, 75,	20, 20 },
/*5-next_off*/  { 71, 75,	20, 20 },
/*6-slider*/	{ 101,70,	137,11 },
/*7-thumb*/		{ 95, 71,	13,	13 },
/*8-loop_on*/   { 308,77,	28,	22 },
/*9-loop_off*/  { 308,77,	28,	22 },
/*10-exit*/		{ 340,83,	16,	16 }
};

#endif // TOURPLAYCTRLDATA_H
