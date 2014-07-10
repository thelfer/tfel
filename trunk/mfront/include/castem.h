/*  declarations dependantes machines de Cast3M  */
#ifdef WIN32
  #define SUN 1
  #define UNIX32 1
  #undef UNIX64
  #undef BUFFER
  #undef AIO
  #define THREAD 1
  #undef LIMIT
#endif
#ifdef WIN64
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

#ifdef UNIX32
typedef int cast_int;
typedef float cast_float;
typedef long cast_long;
typedef double cast_double;
#endif
#ifdef UNIX64
#ifndef WIN64
 typedef long cast_int;
#endif
typedef double cast_float;
typedef long cast_long;
typedef double cast_double;
#endif
#ifdef WIN64
typedef long long cast_int;
#endif
