/*  declarations dependantes machines de Cast3M  */

#if (defined UNIX32) || ((defined _WIN32) && (!defined _WIN64))
typedef int cast_int;
typedef float cast_float;
typedef long cast_long;
#ifndef cast_double
typedef double cast_double;
#endif
#endif

#if (defined UNIX64) || (defined _WIN64)
#ifdef _WIN64
 typedef long long cast_int;
#else
 typedef long cast_int;
#endif
typedef double cast_float;
typedef long cast_long;
#ifndef cast_double
typedef double cast_double;
#endif
#endif
