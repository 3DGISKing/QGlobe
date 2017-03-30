
// Default alignment
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#	pragma pack( pop, packing )
#elif defined (__DMC__)
#	pragma pack( pop )
#elif defined( __GNUC__ )
#   if (__GNUC__ >= 4 ) && (__GNUC_MINOR__ >= 7)
#	    pragma pack( pop, packing )
#   endif
#endif

#undef PACK_STRUCT

