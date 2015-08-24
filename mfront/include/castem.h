/*  declarations dependantes machines de Cast3M  */
#if ((defined _WIN32) && (! defined _WIN64))
  #define SUN 1
  #define UNIX32 1
  #undef UNIX64
  #undef BUFFER
  #undef AIO
  #define THREAD 1
  #undef LIMIT
#endif
#ifdef _WIN64
  #define SUN 1
  #define UNIX64 1
  #undef UNIX32  
  #undef BUFFER
  #undef AIO
  #define THREAD 1
  #undef LIMIT
#endif

#ifdef LINUX
  #define SUN 1
  #define UNIX32 1
  #undef BUFFER
  #define AIO 1
  #define THREAD 1
  #define LIMIT 1
#endif
#ifdef LINUX64
  #define SUN 1
  #define UNIX64 1
  #undef BUFFER
  #define AIO 1
  #define THREAD 1
  #define LIMIT 1
#endif
#ifdef OSX
  #define SUN 1
  #define UNIX64 1
  #undef BUFFER
  #define AIO 1
  #define THREAD 1
  #define LIMIT 1
#endif

#ifdef AIX
  #undef SUN
  #define UNIX32 1
  #undef BUFFER
  #undef AIO   /* version spéciale pour aix */
  #define THREAD 1
  #define LIMIT 1
#endif

#if (defined UNIX32) || ((defined _WIN32) && (!defined _WIN64))
typedef int cast_int;
typedef float cast_float;
typedef long cast_long;
typedef double cast_double;
#endif

#if (defined UNIX64) || (defined _WIN64)
#ifdef _WIN64
 typedef long long cast_int;
#else
 typedef long cast_int;
#endif
typedef double cast_float;
typedef long cast_long;
typedef double cast_double;
#endif
