dnl 
dnl a set of macros related to gxx
dnl 
dnl Helfer Thomas 22/12/06

AC_DEFUN([AC_GCC_CHECK_MACHINE_TYPE],[
  AC_REQUIRE([AC_CANONICAL_HOST])dnl

  AC_CHECKING([for MACHINE_TYPE])

  GCC_MACHINE_TYPE="unknown"
  case "$build_cpu" in
  x86_64*)  GCC_MACHINE_TYPE="64" ;; dnl opteron 64 bits
  i[[3456]]86*linux*) GCC_MACHINE_TYPE="32" ;; dnl intel et compatible 32 bits
  *)        GCC_MACHINE_TYPE="32" ;; dnl defaut 32 bits
  esac])

dnl
dnl @synopsis AC_CHECK_FLAG(-flag,variable where we add flag if ok, optional flag)
dnl
dnl Check flags for C++ compiler
dnl
dnl @author Stephane Bernaud - 06/04/2010
dnl
AC_DEFUN([AC_GCC_CHECK_FLAG], [
	dogxx=`$CXX $1 $3 -S -o /dev/null -xc /dev/null 2>&1 && echo "__OK__"`
	if echo $dogxx | grep -e "__OK__" > /dev/null 2>&1 ; then
	    $2="${$2} $1"
	fi
	]) dnl end of AC_GCC_CHECK_FLAG


dnl
dnl @synopsis AC_GCC_LINUX_OPTIMISATIONS
dnl
dnl set CXXFLAGS, CFLAGS and FFLAGS regarding gcc version and processor type
dnl
dnl @author Stephane Bernaud - 06/04/2010
dnl  Reference : http://www.pixelbeat.org/
dnl  Reference : http://en.gentoo-wiki.com/wiki/Safe_Cflags
dnl

AC_DEFUN([AC_GCC_LINUX_OPTIMISATIONS],[
	AC_CHECKING(for gcc options regarding gcc version and processor type)
	AC_REQUIRE([AC_GCC_CHECK_MACHINE_TYPE])

	AC_SUBST(CXXFLAGS)
	AC_SUBST(CFLAGS)
	AC_SUBST(FFLAGS)

	if test -f /proc/cpuinfo; then
	    vendor_id=`cat /proc/cpuinfo | 
         grep "vendor_id" | 
	 uniq |
         cut -d':' -f2 |
	 sed -e 's/ //g'`

	    cpu_family=`cat /proc/cpuinfo | 
         grep "cpu family" | 
	 uniq |
         cut -d':' -f2 |
         sed -e 's/ //g'`

	    cpu_model=`cat /proc/cpuinfo | 
         grep "model" |
	 grep -v "model name" | 
	 uniq |
         cut -d':' -f2 | 
         sed -e 's/ //g'`

	    flags=`cat /proc/cpuinfo | 
         grep "flags" | 
	 uniq |
         cut -d':' -f2`
	else
	    AC_MSG_ERROR("Unable to access /proc/cpuinfo.")
	fi

	dnl AC_MSG_NOTICE([vendor_id =$vendor_id-])
	dnl AC_MSG_NOTICE([cpu_family =$cpu_family])
	dnl AC_MSG_NOTICE([cpu_model =$cpu_model])
	dnl AC_MSG_NOTICE([flags =$flags])

	case "$vendor_id" in
	    GenuineIntel)
		if test $cpu_family -eq 3 ; then
		    line="i386"
		fi
		if test  $cpu_family -eq 4 ; then
		    line="i486"
		fi
		if test $cpu_family -eq 5 ; then
		    if test $cpu_model -ne 4 ; then
			line="pentium"
		    else
			line="pentium-mmx pentium" 
		    fi
		fi
		if test $cpu_family -eq 6 ; then
		    if test  $cpu_model -eq 0 -o  $cpu_model -eq 1 ; then
			line="pentiumpro"
		    fi
		    if test $cpu_model -ge 3  -a  $cpu_model -le 6 ; then 
			line="pentium2 pentiumpro pentium-mmx pentium i486 i386"
		    fi
		    if test $cpu_model -eq 9  -o $cpu_model -eq 13 ; then 
			line="pentium-m pentium4 pentium3 pentium2 pentiumpro pentium-mmx pentium i486 i386"
		    fi
		    if test $cpu_model -eq 14  -o $cpu_model -eq 22 ; then 
			line="prescott pentium-m pentium4 pentium3 pentium2 pentiumpro pentium-mmx pentium i486 i386"
		    fi
		    if test  $cpu_model -eq 28 ; then 
			line="atom core2 pentium-m pentium4 pentium3 pentium2 pentiumpro pentium-mmx pentium i486 i386"
		    fi
		    if test $cpu_model -ge 15 ; then 
			if test $GCC_MACHINE_TYPE -eq 32 ; then
			    line="core2 prescott pentium-m pentium4 pentium3 pentium2 pentiumpro pentium-mmx pentium i486 i386"
			else
			    line="core2 nocona pentium-m pentium4 pentium3 pentium2 pentiumpro pentium-mmx pentium i486 i386"
			fi
		    fi
		    if test  $cpu_model -ge 7  -a $cpu_model -le 11 ; then
			line="pentium3 pentium2 pentiumpro pentium-mmx pentium i486 i386"
		    fi
		fi    
		if test $cpu_family -eq 15 ; then
		    line="pentium4 pentium3 pentium2 pentiumpro pentium-mmx pentium i486 i386"
		    if test $cpu_model -ge 3 ; then
			if test $GCC_MACHINE_TYPE -eq 32 ; then
			    line="prescott $line"
			else
			    line="nocona $line"
			fi
		    fi
		fi
		dnl failsafe with default value
		if test "x$line" == "x" ; then
		    if test $GCC_MACHINE_TYPE -eq 32 ; then
			line="prescott"
		    else
			line="nocona"
		    fi
		fi
		for f in $line
		  do
		  AC_GCC_CHECK_FLAG(-march=$f,MY_ARCH_FLAG)
		  if test "x$MY_ARCH_FLAG" != "x" ; then	
		      break
		  fi
		done
		;;
	    AuthenticAMD)
		if test $cpu_family -eq 4 ; then
		    line="i486"
		fi
		if test $cpu_family -eq 5 ; then
		    if test $cpu_model -lt 4 ; then
			line="pentium"
		    fi
		    if test $cpu_model -eq 6  -o $cpu_model -eq 7 ; then
			line="k6"
		    fi
		    if test $cpu_model -eq 8  -o $cpu_model -eq 12 ; then
			line="k6-2 k6"
		    fi
		    if test $cpu_model -eq 9  -o $cpu_model -eq 13 ; then
			line="k6-3 k6-2 k6"
		    fi
		    if test $cpu_model -eq 10 ; then 
			line="geode k6-2 k6"
		    fi
		fi
		if test $cpu_family -eq 6 ; then
		    if test $cpu_model -le 3 ; then
			line="athlon k6-3 k6-2 k6"
		    fi
		    if test $cpu_model -eq 4 ; then
			line="athlon-tbird athlon k6-3 k6-2 k6"
		    fi
		    if test $cpu_model -ge 6 ; then 
			line="athlon-4 athlon k6-3 k6-2 k6"
		    fi
		fi
		if test $cpu_family -eq 15 ; then 
		    line="k8 athlon-4 athlon k6-3 k6-2 k6"
		fi
		if test  $cpu_family -eq 16 -o $cpu_family -eq 17 ; then 
		    line="amdfam10 k8 athlon-4 athlon k6-3 k6-2 k6"
		fi
		dnl failsafe with default value
		if test "x$line" == "x" ; then
		    line="k8"
		fi
		for f in $line
		  do
		  AC_GCC_CHECK_FLAG(-march=$f,MY_ARCH_FLAG)
		  if test "x$MY_ARCH_FLAG" != "x" ; then	
		      break
		  fi
		done
		;;
	    *)
		AC_MSG_ERROR([Unsupported CPU vendor : $vendor_id])
		;;
	esac dnl end of case "$vendor_id"

	dnl Do we have a sse capable processor
	echo $flags | grep -q "sse"
	if test "$?" = "0" ; then
	    AC_GCC_CHECK_FLAG(-mfpmath=sse,MY_FPMATH_FLAG,$MY_ARCH_FLAG)
	fi

	dnl we need sse FPU. So exit if not found
	if test -z $MY_FPMATH_FLAG ; then
	    AC_MSG_ERROR([Unable to set FPU to sse. Check gcc version or processor.])
	fi

	dnl optimize for sse2, sse3
	echo $flags | grep -q -E "sse3|pni"
	if test "$?" = "0" ; then
	    AC_GCC_CHECK_FLAG(-msse3,MY_SSE_FLAG,$MY_ARCH_FLAG $MY_FPMATH_FLAG )
	else 
	    echo $flags | grep -q "sse2"
	    if test "$?" = "0" ; then
		AC_GCC_CHECK_FLAG(-msse2,MY_SSE_FLAG,$MY_ARCH_FLAG $MY_FPMATH_FLAG )
	    fi
	fi

	AC_MSG_NOTICE([detected arch flag   =$MY_ARCH_FLAG])
	AC_MSG_NOTICE([detected fpmath flag =$MY_FPMATH_FLAG])
	AC_MSG_NOTICE([detected sse flag    =$MY_SSE_FLAG])

	CXXFLAGS="-O2 $MY_ARCH_FLAG $MY_FPMATH_FLAG $MY_SSE_FLAG $CXXFLAGS"
	CFLAGS="-O2 $MY_ARCH_FLAG $MY_FPMATH_FLAG $MY_SSE_FLAG $CFLAGS"
	OPTIMISATION_FLAGS="-O2 $MY_ARCH_FLAG $MY_FPMATH_FLAG $MY_SSE_FLAG $OPTIMISATION_FLAGS"

]) dnl end of AC_GCC_LINUX_OPTIMISATIONS

AC_DEFUN([AC_GCC_DEFAULT_OPTIMISATIONS],
    [
	OPTIMISATION_FLAGS="-O2 $OPTIMISATION_FLAGS"
	CXXFLAGS="-O2 $CXXFLAGS"
	CFLAGS="-O2 $CFLAGS"
])

dnl 
dnl guess architecture specific options
dnl 
dnl Helfer Thomas 22/12/06
dnl Helfer Thomas 30/10/14 (add test on '-march=native')
AC_DEFUN([AC_GCC_ARCH_OPTIMISATIONS],[
	if test "x$host" = "x$build"; then
	    echo $host | grep "linux" &> /dev/null
	    if test "x$?" = "x0"; then
		sys="linux"
	    fi
	    case "$sys" in
		linux*)
		    AC_GCC_LINUX_OPTIMISATIONS
		    ;;
		*)
		    AC_GCC_DEFAULT_OPTIMISATIONS
		    ;;
    esac
    else
    AC_GCC_DEFAULT_OPTIMISATIONS
    fi
    ])
 
AC_DEFUN([AC_CHECK_GXX],
    [
	if test "x$GXX" = "xyes"; then
	    
	    dnl gcc default warning options
	    CFLAGS="$CFLAGS -W -Wall"
	    
	    dnl g++ default warning options

	    COMPILER_WARNINGS="-W -Wall -Wshadow"
	    COMPILER_WARNINGS="$COMPILER_WARNINGS -Wpointer-arith -Wcast-qual -Wcast-align"
	    COMPILER_WARNINGS="$COMPILER_WARNINGS -Wwrite-strings -Wctor-dtor-privacy -Wnon-virtual-dtor"
	    COMPILER_WARNINGS="$COMPILER_WARNINGS -Woverloaded-virtual -Wreturn-type" 
	    COMPILER_WARNINGS="$COMPILER_WARNINGS -Wno-endif-labels  -Wmissing-format-attribute"
	    COMPILER_WARNINGS="$COMPILER_WARNINGS -Wno-multichar -Wno-deprecated-declarations"
	    COMPILER_WARNINGS="$COMPILER_WARNINGS -Wredundant-decls -Wdisabled-optimization"
	    COMPILER_WARNINGS="$COMPILER_WARNINGS -Wunknown-pragmas -Wundef  -Wreorder"

            CXXFLAGS="$CXXFLAGS $COMPILER_WARNINGS"
	    CXXFLAGS="$CXXFLAGS -std=c++11 -DNO_RUNTIME_CHECK_BOUNDS $COMPILER_WARNINGS"

	    if test "${CXX}" = "clang++"; then
               CXXFLAGS="$CXXFLAGS -ftemplate-depth-250"
	       CXXFLAGS="$CXXFLAGS -Wno-mismatched-tags"
	       COMPILER_SPECIFIC_OPTIONS="-ftemplate-depth-250 -Wno-mismatched-tags $COMPILER_SPECIFIC_OPTIONS"
            fi

	    if test "${CXX}" != "clang++"; then
		dnl gxx versions prior to 3.3 are known to be buggy 
		dnl when two much metaprogramming is involved
		AC_MSG_CHECKING(for gxx 3.4.0 or later)
		GXXVERSION="`"$CXX" -dumpversion`"
		AC_MSG_RESULT($GXXVERSION)
		GXXVERSION="`"$CXX" -dumpversion |
                awk 'BEGIN { FS = "." };
                      1 { if ((@S|@1 * 10000 + @S|@2 * 100 + @S|@3) >= 30400) { print "yes" } }'
                `"
		if test "x$GXXVERSION" = "x$no"; then
		    echo "Found $GXXVERSION"
		    AC_MSG_ERROR("g++ version prior to 3.3 are not supported. Please use at least g++3.4")
		fi
		GXXVERSION="`"$CXX" -dumpversion |
                awk 'BEGIN { FS = "." };
                      1 { if ((@S|@1 * 10000 + @S|@2 * 100 + @S|@3) >= 40000) { print "yes" } }'
                `"
		if test "x$GXXVERSION" = "x$no"; then
		    AC_MSG_WARN("g++3.4 does not work with FSAlgorithm specialisation nicely")
		    AC_MSG_WARN("Using the -DNO_FSALGORITHM_SPECIALISATION flag")
		    CPPFLAGS="-DNO_FSALGORITHM_SPECIALISATION -fno-builtin-abs $CPPFLAGS"
		    COMPILER_SPECIFIC_OPTIONS="-DNO_FSALGORITHM_SPECIALISATION -fno-builtin-abs $COMPILER_SPECIFIC_OPTIONS"
		else
		    CPPFLAGS=" $CPPFLAGS"
		    CXXFLAGS="$CXXFLAGS"
		fi
	    fi
	fi
	if test "x$enable_optimizations" != "xno"; then   	    
	    if test "x$enable_debug" != "xyes"; then
		dnl g++ debug options
		CPPFLAGS="-DNDEBUG $CPPFLAGS"
		AC_GCC_ARCH_OPTIMISATIONS
	    fi
	fi
	
	if test "x$enable_debug" == "xyes"; then
	    dnl g++ debug options
	    CXXFLAGS="-g $CXXFLAGS"
	fi
	
	]) dnl end of AC_CHECK GXX
