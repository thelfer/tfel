dnl 
dnl a set of macros related to icpc
dnl 
dnl Helfer Thomas 22/12/06

AC_DEFUN([AC_CHECK_INTEL],
    [
	if test "x$INTEL" == "xyes"; then
	    dnl icpc default warning options
	    CXXFLAGS="-std=c++11 -Wall  -diag-disable 981,383,444,858,191,68,810,1418 $CXXFLAGS"
	    dnl 444 : destructor for base class ... is not virtual
	    dnl 383 : value copied to temporary, reference to temporary used
	    dnl 981 : operands are evaluated in unspecified order
	    dnl 858 : type qualifier on return type is meaningless
	    dnl 191 : type qualifier is meaningless on cast type
	    dnl 68  : integer conversion resulted in a change of sign
	    dnl 810 : conversion from "long double" to "double" may lose significant bits
	    dnl 1418: external function definition with no prior declaration
	    if test "x$enable_optimizations" != "xno"; then   	    
		if test "x$enable_debug" != "xyes"; then
		    dnl g++ debug options
		    CXXFLAGS="-DNDEBUG -DNO_RUNTIME_CHECK_BOUNDS $CXXFLAGS"
		fi
		dnl icpc optimization options
		CXXFLAGS="-O2 -ipo $CXXFLAGS"
		OPTIMISATION_FLAGS0="-O2 -ipo $OPTIMISATION_FLAGS0"
		if test "x$enable_portable_build" != "xyes";
		then	       	    
		  CXXFLAGS="-xHost $CXXFLAGS"
		fi
		OPTIMISATION_FLAGS="-xHost $OPTIMISATION_FLAGS"
	    fi

	    if test "x$enable_debug" = "xyes"; then
		dnl icpc debug options
		CXXFLAGS="-g $CXXFLAGS"
	    fi
	fi
    ]) dnl end of AC_CHECK_INTEL
