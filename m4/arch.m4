dnl 
dnl guess architecture specific options
dnl 
dnl Helfer Thomas 22/12/06

AC_DEFUN([AC_CHECK_ARCH],[
	if test "x$host" = "x$build"; then
	    echo $host | grep "linux" &> /dev/null
	    if test "x$?" = "x0"; then
		sys="linux"
	    fi
	    case "$sys" in
		linux*)
		    AC_CHECK_ARCH_LINUX;;
		*)
		    AC_CHECK_ARCH_DEFAULT
	    esac
	else
	    AC_CHECK_ARCH_DEFAULT
	fi
	])
