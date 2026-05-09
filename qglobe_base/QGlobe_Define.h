#ifndef __QGLOBE_DEFINE_H_INCLUDED__
#define __QGLOBE_DEFINE_H_INCLUDED__

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


typedef		int				QGLOBE_INT;
typedef		unsigned int	QGLOBE_UINT;
typedef		unsigned long	QGLOBE_UINT32;
typedef		double			QGLOBE_DOUBLE;

typedef enum
{
	QGlobe_SUCCESS = 0,
	QGlobe_FAIL,
	QGlobe_INVALID_HANDLE,
	
	// CAMERA
	QGlobe_CAMERA_ROTATE_FAIL,

	// Render Paint
	QGlobe_NO_RENDER_PAINT,
	
	QGlobe_UNKNOW_ERROR

} QGlobe_ERROR_NO;

#endif // __QGLOBE_DEFINE_H_INCLUDED__
