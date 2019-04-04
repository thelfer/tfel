AC_DEFUN([CHECK_ZSET],[

AC_CHECKING(for Zset)

USE_ZSET=no

AC_ARG_WITH(zset,
    AC_HELP_STRING([--with-zset=DIR ],[directory where zset module is installed ]),
    [ZSET_ROOT="$withval";
     USE_ZSET=yes;
    ], )

ZSET_OK=no

if test "x$USE_ZSET" == "xyes";
then
  DCHK=$ZSET_ROOT
  if test -n $DCHK -o -d ${DCHK};
  then
    if [ test ! -e $ZSET_ROOT/include/Behavior.h ];
    then
	AC_MSG_ERROR(header Behavior.h not found in ${DCHK})
    else 
	ZSET_OK=yes
	AC_MSG_NOTICE("ZSET intallation path : $ZSET_ROOT")
    fi
  else
    AC_MSG_ERROR(argument of --with_zset is invalid)
  fi
fi

if test "x$ZSET_OK" == "xyes";
then
    AC_DEFINE_UNQUOTED(ZSET_ROOT,"$ZSET_ROOT")
else
    AC_MSG_NOTICE("No ZSET intallation path given")
fi

])dnl
