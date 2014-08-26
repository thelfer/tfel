AC_DEFUN([CHECK_CYRANO_MACHINE_TYPE],[
    AC_REQUIRE([AC_CANONICAL_HOST])dnl
    AC_CHECKING("castem unix type")
    case $host in
    # machine 386 -> 686, 32 bits
      i[[3456]]86*linux*)
	CYRANO_ARCH=32
        ;;
    # machine amd opteron, 64 bits
      x86_64*linux*)
	CYRANO_ARCH=64
        ;;
   # ben on ne sait pas :-(
      *)
        AC_MSG_ERROR(unsupported host $host)
        ;;
    esac
    AC_DEFINE_UNQUOTED(CYRANO_ARCH,$CYRANO_ARCH)
])