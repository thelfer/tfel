dnl 
dnl a set of macros related to gxx
dnl 
dnl Helfer Thomas 22/12/06

AC_DEFUN([AC_CHECK_GXX],
    [
	if test "x$GXX" = "xyes"; then

	    dnl g++ default warning options
	    CXXFLAGS="$CXXFLAGS -W -Wall -Wconversion -Wshadow"
	    CXXFLAGS="$CXXFLAGS -Wpointer-arith -Wcast-qual -Wcast-align"
	    CXXFLAGS="$CXXFLAGS -Wwrite-strings -Wctor-dtor-privacy -Wnon-virtual-dtor"
            CXXFLAGS="$CXXFLAGS -Woverloaded-virtual -Wimplicit -Wreturn-type -Wfloat-equal"
            CXXFLAGS="$CXXFLAGS -Wno-endif-labels  -Wsign-compare -Wmissing-format-attribute"
            CXXFLAGS="$CXXFLAGS -Wno-multichar -Wno-deprecated-declarations -Wpacked"
            CXXFLAGS="$CXXFLAGS -Wredundant-decls -Wlong-long -Wdisabled-optimization"
            CXXFLAGS="$CXXFLAGS -Wunknown-pragmas -Wundef  -Wreorder"

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

 	if test "x$enable_optimizations" != "xno"; then   	    
	    if test "x$enable_debug" != "xyes"; then
		dnl g++ debug options
		CPPFLAGS="-DNDEBUG $CPPFLAGS"
	    fi
	    dnl g++ default optimization options
	    CXXFLAGS="-O2 -march=$arch $CXXFLAGS"
	    OPTIMISATION_FLAGS="-O2 -march=$arch $OPTIMISATION_FLAGS"
	    case "x$arch" in
		pentium4*)
		    CXXFLAGS="-malign-double $CXXFLAGS"
		    OPTIMISATION_FLAGS="-malign-double $OPTIMISATION_FLAGS"
		    ;;
	    esac
 	fi

	if test "x$enable_debug" == "xyes"; then
	    dnl g++ debug options
	    CXXFLAGS="-g $CXXFLAGS"
	fi

	]) dnl end of AC_CHECK GXX
