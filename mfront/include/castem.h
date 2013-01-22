#ifdef WIN32
  #define SUN 1
  #define UNIX32 1
  #undef BUFFER
  #undef AIO
  #undef THREAD
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
#ifdef AIX
  #undef SUN
  #define UNIX32 1
  #undef BUFFER
  #undef AIO   /* version spéciale pour aix */
  #define THREAD 1
  #define LIMIT 1
#endif

#ifdef SUN
#define getcpu getcpu_
#define xopen xopen_
#define xrinit xrinit_
#define xfenet xfenet_
#define xrlabl xrlabl_
#define xhcoul xhcoul_
#define xvalis xvalis_
#define xolrl xolrl_
#define xrface xrface_
#define xraiso xraiso_
#define xraff xraff_
#define xrget xrget_
#define trfin trfin_
#define xclik xclik_
#define xenu xenu_
#define xranim xranim_
#define xrimag xrimag_
#define xrswap xrswap_
#define timespv timespv_
#define cgibnam cgibnam_
#define verdat verdat_
#define lance lance_
#define ecrdon ecrdon_
#define lires lires_
#define becrdon becrdon_
#define numloi numloi_
#define loiexterne loiexterne_
#define blires blires_
#define gistty gistty_
#define cgibnam cgibnam_
#define initserver initserver_
#define initclient initclient_
#define writesocket writesocket_
#define readsocket readsocket_
#define closesocket closesocket_
#define threadii threadii_
#define threadis threadis_
#define threadid threadid_
#define threadif threadif_
#define inithread inithread_
#define otrbox otrbox_
#define ofvali ofvali_
#define omenu omenu_
#define oinseg oinseg_
#define otrdig otrdig_
#define otrais otrais_
#define otreff otreff_
#define otrmfi otrmfi_
#define ozoom ozoom_
#define ochang ochang_
#define oini oini_
#define oflgi oflgi_
#define oval oval_
#define omajse omajse_
#define otrani otrani_
#define otrima otrima_
#define otrmes otrmes_
#define oglini oglini_
#define ogldfene ogldfene_
#define ogltrlabl ogltrlabl_
#define oglchcou oglchcou_
#define oglpolrl oglpolrl_
#define ogltrfac ogltrfac_
#define oglaff oglaff_
#define ogltriso ogltriso_
#define oglfvali oglfvali_
#define oglinsegt oglinsegt_
#define oglmajse oglmajse_
#define oglchang oglchang_
#define ogltrmess ogltrmess_
#define oglzoom oglzoom_
#define invrs invrs_
#define otrdigro otrdigro_
#define oglget oglget_
#define externalumat externalumat_
#endif
#ifdef UNIX32
typedef int cast_int;
typedef float cast_float;
typedef long cast_long;
typedef double cast_double;
#endif
#ifdef UNIX64
typedef long cast_int;
typedef double cast_float;
typedef long cast_long;
typedef double cast_double;
#endif

