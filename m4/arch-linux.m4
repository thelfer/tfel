dnl 
dnl guess architecture specific options on linux systems
dnl 
dnl Helfer Thomas 22/12/06

AC_DEFUN([AC_CHECK_ARCH_LINUX],[
	case "$host" in 
	    i?86-* | k?-* | athlon-* | pentium* | x86_64* )
		case "$host" in
		    i386-*)
			arch="i386" ;;
		    i486-*)
			arch="i486" ;;
		    i586-*)
			arch="pentium" ;;
		    pentium-mmx-*)
			arch="pentium-mmx" ;;
		    pentiumpro-* | pentium2-* | i686-*)
			if test -f /proc/cpuinfo; then
                          modelname=`cat /proc/cpuinfo | 
                                     grep "model name" | 
                                     sed -e 's/ //g'   | 
                                     cut -d':' -f2`
			  echo $modelname |grep "Intel" |grep "Pentium"| grep "4CPU" &> /dev/null
			  if test "$?" = "0"; then
			      arch="pentium4"
			  else
			      arch="pentiumpro"
			  fi
			fi
			;;
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
			if test -f /proc/cpuinfo; then
                          modelname=`cat /proc/cpuinfo | 
                                     grep "model name" | 
                                     sed -e 's/ //g'   | 
                                     cut -d':' -f2`
			  echo $modelname |grep "AMD" &> /dev/null
			  if test "$?" = "0"; then
			      arch="athlon64"
			  else
			      arch="nocona"
			  fi
			fi
			;;
		    *)
			AC_MSG_ERROR("Unsupported host type : $host")
		esac dnl end of case "$host"
		;;
	    *) 
		AC_MSG_ERROR("Unsupported host type : $host")
	esac dnl end of case "$host"
	]) dnl end of AC_CHECK_ARCH_LINUX
