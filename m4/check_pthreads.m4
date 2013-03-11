#@synonpsis CHECK_PTHREADS
dnl  check for pthreads system interfaces.
dnl  set CFLAGS_PTHREADS,  CXXFLAGS_PTHREADS and LIBS_PTHREADS to
dnl  flags to compiler flags for multithread program compilation (if exists),
dnl  and library, if one required.
dnl
dnl@author   (C) Ruslan Shevchenko <Ruslan@Shevchenko.Kiev.UA>, 1998
dnl@id $Id$
dnl ----------------------------------------------------------------
dnl CHECK_PTHREADS

AC_DEFUN([CHECK_PTHREADS],[
AC_REQUIRE([AC_CANONICAL_SYSTEM])dnl
AC_CHECK_HEADER(pthread.h,AC_DEFINE(HAVE_PTHREAD_H))
AC_CHECK_LIB(posix4,nanosleep, LIBS_PTHREADS="-lposix4",LIBS_PTHREADS="")
AC_CHECK_LIB(pthread,pthread_mutex_lock, 
             LIBS_PTHREADS="-lpthread $LIBS_PTHREADS")
AC_MSG_CHECKING([parameters for using pthreads])
case $build_os in
  freebsd*)
    CFLAGS_PTHREADS="-pthread"
    CXXFLAGS_PTHREADS="-pthread"
    ;;
  *)
    ;;
esac

AC_MSG_RESULT(["flags: $CFLAGS_PTHREADS\;libs: $LIBS_PTHREADS"])
])dnl
dnl
dnl
