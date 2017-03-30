#ifndef GDEM_CONFIG_H
#define GDEM_CONFIG_H

#ifndef STANDARD_ALONE
#define STANDARD_ALONE
#endif // STANDARD_ALONE

#ifndef USE_MYSQL
//#define USE_MYSQL
#endif // USE_MYSQL

#ifndef ONE_CLIENT_SOCKET_NUM
#define ONE_CLIENT_SOCKET_NUM		6

#ifndef NDEBUG 
	#define DEBUG_LOG
	#define GDEM_DEBUG
#endif

#ifdef GDEM_DEBUG
	#ifndef DEBUG
		#define DEBUG 
	#endif
#else
	#ifdef DEBUG
		#undef DEBUG 
	#endif
#endif

#endif

#endif // GDEM_CONFIG_H
