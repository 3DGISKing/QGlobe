#ifndef __GDEM_DEFINE_H_INCLUDED__
#define __GDEM_DEFINE_H_INCLUDED__

#define IN
#define OUT
#define INOUT

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#define EARTH_RADIUS		6378137.0f		// m
#define CAMERA_DIST_MIN		100
#define CAMERA_DIST_MAX		EARTH_RADIUS * 7


typedef		int				GDEM_INT;
typedef		unsigned int	GDEM_UINT;
typedef		unsigned long	GDEM_UINT32;
typedef		double			GDEM_DOUBLE;

typedef enum
{
	GDM_SUCCESS = 0,
	GDM_FAIL,
	GDM_INVALID_HANDLE,
	
	// CAMERA
	GDM_CAMERA_ROTATE_FAIL,

	// Render Paint
	GDM_NO_RENDER_PAINT,
	
	GDM_UNKNOW_ERROR

} GDM_ERROR_NO;

#endif // __GDEM_DEFINE_H_INCLUDED__
