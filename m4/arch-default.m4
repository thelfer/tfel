dnl 
dnl guess architecture specific based on the $build variable
dnl 
dnl Helfer Thomas 22/12/06

AC_DEFUN([AC_CHECK_ARCH_DEFAULT],[
	case "$build" in 
	    i?86-* | k?-* | athlon-* | pentium* | x86_64* )
		case "$build" in
		    i386-*)
			arch="i386" ;;
		    i486-*)
			arch="i486" ;;
		    i586-*)
			arch="pentium" ;;
		    pentium-mmx-*)
			arch="pentium-mmx" ;;
		    pentiumpro-* | pentium2-* | i686-*)
			arch="pentiumpro";;
		    k6-2-* | k6-3-*)
			arch="k6-2" ;;
		    k6-*)
			arch="k6" ;;
		    pentium3-*)
			arch="pentium3" ;;
		    pentium4-*)
			arch="pentium4" ;;
		    athlon-4-* | athlon-xp-* | athlon-mp-*)
			arch="athlon-4" ;;
		    k7-* | athlon-tbird-* | athlon-*)
			arch="athlon";;
		    x86_64-*)
			arch="nocona";;
		    *)
			AC_MSG_ERROR("Unsupported build type : $build")
		esac dnl end of case "$build"
		;;
	    *) 
		AC_MSG_ERROR("Unsupported build type : $build")
	esac dnl end of case "$build"
	]) dnl end of AC_CHECK_ARCH_DEFAULT
