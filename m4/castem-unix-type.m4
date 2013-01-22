AC_DEFUN([CHECK_CASTEM_UNIX_TYPE],[
    AC_REQUIRE([AC_CANONICAL_HOST])dnl
    AC_CHECKING("castem unix type")
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
      x86_64*-mingw32*)
# Pas sûr, mais c'est pour le test
	CASTEM_UNIX_TYPE=UNIX32
	CASTEM_CPPFLAGS="-DUNIX32"
	;;
      i*86-*-mingw32*)
	CASTEM_UNIX_TYPE=UNIX32
	CASTEM_CPPFLAGS="-DUNIX32"
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
    AC_SUBST(CASTEM_FFLAGS)
    AC_SUBST(CASTEM_CFLAGS)
    AC_SUBST(CASTEM_CPPFLAGS)
    AC_SUBST(CASTEM_UNIX_TYPE)
    AC_DEFINE_UNQUOTED(CASTEM_CPPFLAGS,"$CASTEM_CPPFLAGS")
])