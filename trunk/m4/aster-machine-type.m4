AC_DEFUN([CHECK_ASTER_MACHINE_TYPE],[
    AC_REQUIRE([AC_CANONICAL_HOST])dnl
    AC_CHECKING("castem unix type")
    case $host in
    # machine 386 -> 686, 32 bits
      i[[3456]]86*linux*)
	ASTER_ARCH=32
        ;;
    # machine amd opteron, 64 bits
      x86_64*linux*)
	ASTER_ARCH=64
        ;;
   # ben on ne sait pas :-(
      *)
        AC_MSG_ERROR(unsupported host $host)
        ;;
    esac
    AC_DEFINE_UNQUOTED(ASTER_ARCH,$ASTER_ARCH)
])