dnl@synopsis ENABLE_PTHREADS
dnl
dnl modify CFLAGS, CXXFLAGS and LIBS for compiling pthread-based programs.
dnl
dnl@author  (C) Ruslan Shevchenko <Ruslan@Shevchenko.Kiev.UA>, 1998, 2000
dnl@id  $Id$
dnl
dnl
AC_DEFUN([ENABLE_PTHREADS],[
AC_REQUIRE([CHECK_PTHREADS])

if test -z "$enable_pthreads_done"
then
 CFLAGS="$CFLAGS $CFLAGS_PTHREADS"
 CXXFLAGS="$CXXFLAGS $CXXFLAGS_PTHREADS"
 LIBS="$LIBS $LIBS_PTHREADS"
fi
enable_pthreads_done=yes
])dnl
dnl
