AC_DEFUN([CHECK_CASTEM],[
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_F77_LIBRARY_LDFLAGS])dnl
AC_REQUIRE([MP_WITH_CURSES])dnl
AC_REQUIRE([MP_WITH_CURSES])dnl
AC_REQUIRE([CHECK_CASTEM_UNIX_TYPE])dnl
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
  
  AC_MSG_NOTICE("Seaching Cast3M version")

  DCHK=$CASTEM_ROOT
  
  if test -n $DCHK -o -d ${DCHK};
  then
    CASTEM_OK=no
    if [ test -e $CASTEM_ROOT/include/castem.h ];
    then
        CASTEM_OK=yes
    fi
    if [ test -e $CASTEM_ROOT/include/c/castem.h ];
    then
        CASTEM_OK=yes
    fi
    if [ test "x$CASTEM_OK" == "xno" ];
    then
	AC_MSG_ERROR(header castem.h not found in ${DCHK})
    fi
    CASTEM_OK=yes
    # test de la version de Castem :
    #
    year=$(echo `date +%F` | cut -c1-4)   # date +%F is of form yyyy-mm-dd, ex: 2010-09-01
    until=2006
    CASTEM_VERSION="undef"
    while [[ $year -ge $until ] && [ "x$CASTEM_VERSION" == "xundef" ]]
    do
	AC_MSG_NOTICE("Looking for Cast3M $year")
	subYear=$(echo $year | cut -c3-4)
	castString="castem**${subYear}*"
	castem_version_name="castem20${subYear}"
	CASTEM_VERSION_NAME=$(echo $castem_version_name | tr [:lower:] [:upper:])
	eval $castem_version_name=
	if test "x$CASTEM_VERSION" == "xundef" ; then
            for aFileName in `find ${DCHK} \( -type f -o -type l \) -name "${castString}"`
            do
		eval $castem_version_name=${aFileName}
            done
	fi
	eval $CASTEM_VERSION_NAME=${!castem_version_name}
	if test -n "${!castem_version_name}"; then
            AC_MSG_RESULT($CASTEM_VERSION_NAME: ${!CASTEM_VERSION_NAME} ....)
            CASTEM_VERSION="${year}"
            year=$until
	fi
	year=$((year-1))
    done
    dnl Not found !
    if test "x$CASTEM_VERSION" == "xundef" ; then
      CASTEM_OK=no
      AC_MSG_ERROR(No right version of castem found in ${DCHK})
    fi
  
    AC_DEFINE_UNQUOTED(CASTEM_VERSION,"$CASTEM_VERSION")
    AC_SUBST(CASTEM_ROOT)
  else
    AC_MSG_ERROR(argument of --with_castem is invalid)
  fi
fi

AM_CONDITIONAL(HAVE_CASTEM, test "x$CASTEM_OK" == "xyes")

if test "x$CASTEM_OK" == "xyes";
then
  AC_DEFINE(HAVE_CASTEM)
  AC_DEFINE_UNQUOTED(CASTEM_ROOT,"$CASTEM_ROOT")
  AC_MSG_NOTICE("Found Cast3M version $CASTEM_VERSION in $CASTEM_ROOT")
fi

])dnl
