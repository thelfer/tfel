AC_DEFUN([CHECK_CASTEM],[
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_F77_LIBRARY_LDFLAGS])dnl
AC_REQUIRE([MP_WITH_CURSES])dnl
dnl voir pour ncurse !!!

AC_CHECKING(for Castem)

USE_CASTEM=no

AC_ARG_WITH(castem,
    AC_HELP_STRING([--with-castem=DIR ],[directory where castem module is installed ]),
    [CASTEM_ROOT="$withval";
     USE_CASTEM=yes;
    ], )

CASTEM_OK=no

if test "x$USE_CASTEM" == "xyes";
then
  
  DCHK=$CASTEM_ROOT
  
  if test -n $DCHK -o -d ${DCHK};
  then
    if [ test ! -e $CASTEM_ROOT/include/castem.h ];
	then
	CASTEM_OK=no
	AC_MSG_ERROR(header castem.h not found in ${DCHK})
	fi
    CASTEM_OK=yes
    #
    # test de la version :
    castem_version="undef"
    castem2010=""
    for f in `find ${DCHK} \( -type f -o -type l \) -name 'cast*10*'`
    do
      castem2010=$f
    done
    castem2008=""
    for f in `find ${DCHK} \( -type f -o -type l \) -name 'cast*08*'`
    do
      castem2008=$f
    done
    castem2007=""
    for f in `find ${DCHK} \( -type f -o -type l \) -name 'cast*07*'`
    do
      castem2007=$f
    done
    castem2006=""
    for f in `find ${DCHK} \( -type f -o -type l \) -name 'cast*06*'`
    do
      castem2006=$f
    done
    if test "x$castem2010" != "x" ; then
      castem_version="V2010"
      CASTEM_LIB="$CASTEM_LIB -lXext -ljpeg -lglut -lGLU -lGL -lpthread -ldl"
    fi
    if test "x$castem2008" != "x" ; then
      castem_version="V2008"
      CASTEM_LIB="$CASTEM_LIB -lXext -ljpeg -lglut -lGLU -lGL -lpthread -ldl"
    fi
    if test "x$castem2007" != "x" ; then
      castem_version="V2007"
      CASTEM_LIB="$CASTEM_LIB -lXext -ljpeg -lglut -lGLU -lGL -lpthread -ldl"
    fi
    if test "x$castem2006" != "x" ; then
      castem_version="V2006"
      CASTEM_LIB="$CASTEM_LIB -lXext -ljpeg -lglut -lGLU -lGL -lpthread -ldl"
    fi
    castem2005=""
    for f in `find ${DCHK} \( -type f -o -type l \) -name 'cast*05*'`
    do
      castem2005=$f
    done
    if test "x$castem2005" != "x" ; then
      castem_version="V2005"
    fi

    if test "x$castem2010" == "x" -a "x$castem2008" == "x" -a "x$castem2007" == "x" -a "x$castem2006" == "x" -a "x$castem2005" == "x" ; then
      CASTEM_OK=no
      AC_MSG_ERROR(No right version of castem found in ${DCHK})
    fi
  
    AC_PATH_X
  
    AC_DEFINE_UNQUOTED(CASTEM_VERSION,"$castem_version")
    AC_SUBST(CASTEM_ROOT)
  
    case $host in
    # machine 386 -> 686, 32 bits
      i[[3456]]86*linux*)
	CASTEM_UNIX_TYPE=UNIX32
        CASTEM_CPPFLAGS="-DSUN -DUNIX32 -DTHREAD"
        CASTEM_FFLAGS=""
        CASTEM_CFLAGS=""
        ;;
    # machine amd opteron, 64 bits
      x86_64*linux*)
        CASTEM_UNIX_TYPE=UNIX64
        CASTEM_CPPFLAGS="-DSUN -DUNIX64 -DTHREAD"
        CASTEM_FFLAGS="-fPIC -fdefault-double-8  -fdefault-integer-8  -fdefault-real-8"
        CASTEM_CFLAGS="-fPIC"
        ;;
      i686-pc-cygwin)
	CASTEM_UNIX_TYPE=UNIX32
	CASTEM_CPPFLAGS="-DUNIX32"
	;;
    # ben on ne sait pas :-(
      *)
        AC_MSG_ERROR(unsupported host $host)
        ;;
    esac
    AC_SUBST(CASTEM_CPPFLAGS)
    AC_SUBST(CASTEM_UNIX_TYPE)
    AC_SUBST(CASTEM_FFLAGS)
    AC_SUBST(CASTEM_CFLAGS)
    AC_SUBST(CASTEM_ROOT)
  else
    AC_MSG_ERROR(argument of --with_castem is invalid)
  fi
fi

AM_CONDITIONAL(HAVE_CASTEM, test "x$CASTEM_OK" == "xyes")

if test "x$CASTEM_OK" == "xyes";
then
  AC_DEFINE(HAVE_CASTEM)
  AC_DEFINE_UNQUOTED(CASTEM_CPPFLAGS,"$CASTEM_CPPFLAGS")
  AC_DEFINE_UNQUOTED(CASTEM_ROOT,"$CASTEM_ROOT")
fi

])dnl
