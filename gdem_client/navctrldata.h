#ifndef NAVCTRLDATA_H
#define NAVCTRLDATA_H

#define RINGN_HOV	"new_ring_n_hover.png"
#define RINGN_ACT	"new_ring_n_active"

#define RING_BAC	"new_ring_background.png"
#define	RING_NOR	"new_ring_norm.png"
#define RING_HOV	"new_ring_hover.png"
#define RING_ACT	"new_ring_active.png"

#define LOOK_BAC	"new_look_background.png"
#define LOOK_NOR	"new_look_norm.png"
#define	LOOK_HOV	"new_look_hover.png"
#define	LOOK_ACT	"new_ring_active.png"
#define LOOK_LHOV	"new_look_spotlight_hover.png"
#define LOOK_LACT	"new_look_spotlight_active.png"

#define MOV_BAC		"new_move_background.png"
#define MOV_NOR		"new_move_norm.png"
#define MOV_LHOV	"new_move_spotlight_hover.png"
#define MOV_LACT	"new_move_spotlight_active.png"

#define	ZOOM_BAC	"new_zoom_background.png"
#define ZOOM_NOR	"new_zoom_norm.png"

#define ZOOM_PHOV	"new_zoom_plus_hover.png"
#define ZOOM_PACT	"new_zoom_plus_active.png"

#define ZOOM_MHOV	"new_zoom_minus_hover.png"
#define ZOOM_MACT	"new_zoom_minus_active.png"

#define ZOOM_BNOR	"new_zoombar_norm.png"
#define ZOOM_BHOV	"new_zoombar_hover.png"
#define ZOOM_BACT	"new_zoombar_active.png"

#define COMP_BAC	"compasstop.png"

#define ZOOM_BAR_MIN	(-46)
#define ZOOM_BAR_MAX	46

static char *s_imgNames[NCT_MAX+1][NS_MAX] =
{// kind        back		norm		hover       active
/*0-ring_n*/    {"",		"",			RINGN_HOV,	RINGN_ACT	},
/*1-ring*/      {RING_BAC,	RING_NOR,	RING_HOV,	RING_ACT	},
/*2-look*/      {LOOK_BAC,	LOOK_NOR,	LOOK_LHOV,	LOOK_LACT	},
/*3-mov*/       {MOV_BAC,	MOV_NOR,	MOV_LHOV,	MOV_LACT	},
/*4-zoom*/      {ZOOM_BAC,	ZOOM_NOR,	"",			""			},
/*5-zoom_min*/  {"",		"",			ZOOM_MHOV,	ZOOM_MACT	},
/*6-zoom_plus*/ {"",		"",			ZOOM_PHOV,	ZOOM_PACT	},
/*7-zoom_bar*/  {"",		ZOOM_BNOR,	ZOOM_BHOV,	ZOOM_BACT	},
/*8-compass*/   {COMP_BAC,	"",			"",			"",			},
	{0}
};

static NavCtrlRegion s_ctrlRegion[NCT_MAX] =
{//                x  y     w   h
/*ring_n*/      { 45, 50,   78, 78  },
/*ring*/        { 45, 50,   78, 78  },
/*look*/        { 45, 50,   50, 50  },
/*mov*/         { 45, 132,  50, 50  },
/*zoom*/        { 45, 258,  0, 0    },
/*zoom_min*/    { 45, 258,  18, 144 },
/*zoom_plus*/   { 45, 258,  18, 144 },
/*zoom_bar*/    { 45, 258,  16, 10  },
/*compass*/     { 72, 70,   0, 0,   }
};

static NavCtrlSpec s_ctrlSpec[NCT_MAX] =
{//               movType       r   w   h   min max
/*ring_n*/      { NCMT_ROTATE,  38, 12, 12, 0,  360	},
/*ring*/        { NCMT_ROTATE,  0,  78, 78, 0,  360	},
/*look*/        { NCMT_ROTATE,  0,  50, 50, 0   },
/*mov*/         { NCMT_ROTATE,  0,  50, 50, 0   },
/*zoom*/        { NCMT_NO,      0,  0,  0,  0   },
/*zoom_min*/    { NCMT_NO,      61, 18, 18, 0   },
/*zoom_plus*/   { NCMT_NO,     -61, 18, 18, 0   },
/*zoom_bar*/    { NCMT_SLIDE,   0,  16, 10, -46,46	},
/*compass*/     { NCMT_NO,      0,  0,  0,  0   }
};

static int s_checkOrder[] = {NCT_LOOK, NCT_RING_N, NCT_RING, NCT_LOOK, NCT_MOVE,
							NCT_ZOOM_PLUS, NCT_ZOOM_MINUS, NCT_ZOOM_BAR, -1};

static NavStateRule s_stateRule[] =
{
//			Menu				Joystic					Compass
//------------------------+-----------------------+-----------------------
//	curMode		newMode		del			disp		del			disp
	{NM_AUTO,	NM_ALWAYS,	NS_BACK,	NS_NORMAL,	-1,			-1		},
	{NM_AUTO,	NM_COMPASS,	NS_BACK,	-1,			-1,			NS_BACK	},
	{NM_AUTO,	NM_NODISP,	NS_BACK,	-1,			-1,			-1		},

	{NM_ALWAYS,	NM_AUTO,	NS_NORMAL,	NS_BACK,	-1,			-1		},
	{NM_ALWAYS,	NM_COMPASS,	NS_NORMAL,	-1,			-1,			NS_BACK	},
	{NM_ALWAYS,	NM_NODISP,	NS_NORMAL,	-1,			-1,			-1		},

	{NM_COMPASS,NM_AUTO,	-1,			NS_BACK,	NS_BACK,	-1		},
	{NM_COMPASS,NM_ALWAYS,	-1,			NS_NORMAL,	NS_BACK,	-1		},
	{NM_COMPASS,NM_NODISP,	-1,			-1,			NS_BACK,	-1		},

	{NM_NODISP,	NM_AUTO,	-1,			NS_BACK,	-1,			-1		},
	{NM_NODISP,	NM_ALWAYS,	-1,			NS_NORMAL,	-1,			-1		},
	{NM_NODISP,	NM_COMPASS,	-1,			-1,			-1,			NS_BACK	},

	-1
};

#endif // NAVCTRLDATA_H
