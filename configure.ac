# Process this file with autoconf to produce a configure script.
# Process this file with automake to produce Makefile.in files.

AC_INIT(tfel,1.0,thomas.helfer@cea.fr)
AC_CONFIG_SRCDIR([include/Config/TFELConfig.hxx])
AC_CONFIG_COMMANDS
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CANONICAL_TARGET

# Automake specific stuff
AM_INIT_AUTOMAKE([foreign dist-bzip2])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

# licence choice
gpl_licence=no;
AC_ARG_ENABLE(gpl,
   AC_HELP_STRING([--enable-gpl],
                  [Allow linking to gpl libraries.
		   Resulting librairies or executables
		   will be covered by the gpl licence]),
   AC_DEFINE(GPL_LICENCE)
   gpl_licence=yes,[])

#disable static library generation by default
AC_ENABLE_STATIC(no)

# Checks for compilers
# disable automatic declaration of
# FFLAGS by AC_PROG_F77
if test "x$FFLAGS" = "x";
    then
    FFLAGS=""
fi

AC_PROG_F77
AC_F77_WRAPPERS

# disable automatic declaration of
# CXXFLAGS by AC_PROG_CXX
if test "x$CXXFLAGS" = "x";
    then
    CXXFLAGS=""
fi

AC_PROG_CXX

# Macro for lisp
AM_PATH_LISPDIR

# Libtool specific stuff
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)
AC_SUBST(LIBTOOL_DEPS)

#check for fenv.h
AC_CHECK_HEADER([fenv.h],
		[AC_DEFINE(HAVE_FENV)],[])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([isascii])

# Find what kind of architecture we are in
AC_CHECK_ARCH

# Compiler specific stuff
AC_ARG_ENABLE([optimizations],
	       AC_HELP_STRING([--disable-optimizations],
		              [Do not try to guess what optimization to enable]))

# Enable debug
AC_ARG_ENABLE([debug],
	AC_HELP_STRING([--enable-debug], [Compile with debug options]))

#disable default flags
CFLAGS="";

#choose between intel icpc and g++
INTEL="no"
if test "x$GXX" = "xyes" -o "${CXX##*/}x" = "icpcx" ; then
    if test "${CXX##*/}x" = "icpcx"; then
	GXX="no"
	INTEL="yes"
    fi
fi

# check for compiler
COMPILER_SPECIFIC_OPTIONS="";
OPTIMISATION_FLAGS="-DNO_RUNTIME_CHECK_BOUNDS -DNDEBUG"

# g++ specific treatement
if test "x$GXX" = "xyes"; then
    AC_CHECK_GXX
fi

# intel icc specific treatement
if test "x$INTEL" = "xyes"; then
    AC_CHECK_INTEL
fi

if test "x$COMPILER_SPECIFIC_OPTIONS" != "x";
then
  AC_DEFINE_UNQUOTED(COMPILER_SPECIFIC_OPTIONS,"$COMPILER_SPECIFIC_OPTIONS")
fi

AC_DEFINE_UNQUOTED(OPTIMISATION_FLAGS,"$OPTIMISATION_FLAGS")

# checks for python
AM_PATH_PYTHON([2.2],,[:])
AM_CONDITIONAL([HAVE_PYTHON], [test "$PYTHON" != :])
if test  "$PYTHON" != :
then
	AC_DEFINE(HAVE_PYTHON)
	file=$(mktemp)
	cat > $file << EOF
#!$PYTHON
from distutils import sysconfig
if sysconfig.get_python_inc() != sysconfig.get_python_inc(plat_specific=True) :
    print '-I' + sysconfig.get_python_inc() + '-I' + sysconfig.get_python_inc(plat_specific=True)
else :
    print '-I' + sysconfig.get_python_inc()
EOF
   PYTHON_INCLUDES=$($PYTHON $file)
   rm $file
   AC_DEFINE_UNQUOTED(TFEL_PYTHON_INCLUDES,"$PYTHON_INCLUDES")
fi

#gnuplot
AC_CHECK_PROGS(GNUPLOT,gnuplot,[:])
AM_CONDITIONAL([HAVE_GNUPLOT], [test "$GNUPLOT" != :])
if test "$GNUPLOT" != :
then
	AC_DEFINE_UNQUOTED(GNUPLOT_PATH,"$GNUPLOT")
	AC_DEFINE(HAVE_GNUPLOT)
fi

# gettext
AM_GNU_GETTEXT([external])

# readline
if test "x$gpl_licence" != "xno"
then
	VL_LIB_READLINE
	AM_CONDITIONAL(HAVE_READLINE,test x"$vl_cv_lib_readline_history" = "xyes")
else
	AM_CONDITIONAL(HAVE_READLINE,false)
fi

#gsl
if test "x$gpl_licence" != "xno"
then
	AC_CHECK_PROGS(GSLCONFIG,gsl-config,[:])
	AM_CONDITIONAL([HAVE_GSL], [test "$GSLCONFIG" != :])
	if test "$GSLCONFIG" != :
	then
		GSLINCLUDES=$($GSLCONFIG --cflags)
		GSLLIBS=$($GSLCONFIG --libs)
		AC_SUBST(GSLINCLUDES)
		AC_SUBST(GSLLIBS)
		AC_DEFINE(HAVE_GSL)
	fi
else
	AM_CONDITIONAL([HAVE_GSL],false)
fi

#octave
if test "x$gpl_licence" != "xno"
then
	AC_CHECK_PROGS(MKOCTFILE,mkoctfile,[:])
	AM_CONDITIONAL([HAVE_OCTAVE], [test "$MKOCTFILE" != :])
	if test "$MKOCTFILE" != :
	then
		OCTAVEINCLUDES=$($MKOCTFILE -p INCFLAGS)
		OCTAVELIBS="$($MKOCTFILE -p OCTAVE_LIBS)"
		OCTAVELIBS="$OCTAVELIBS $($MKOCTFILE -p LIBS)"
		OCTAVELIBS="$OCTAVELIBS $($MKOCTFILE -p LFLAGS)"
		OCTAVERLIBS="$($MKOCTFILE -p RLD_FLAG)"
		AC_SUBST(OCTAVEINCLUDES)
		AC_SUBST(OCTAVELIBS)
		AC_SUBST(OCTAVERLIBS)
		AC_DEFINE(HAVE_OCTAVE)
	fi
else
	AM_CONDITIONAL([HAVE_OCTAVE],false)
fi

# gtkmm
PKG_CHECK_MODULES([GTKMM],
		  [gtkmm-2.4 >= 2.12.0 librsvg-2.0 >= 2.18.2],
		  [enable_graphics=true],
	          [enable_graphics=false])

AM_CONDITIONAL(WITH_GRAPHICS,test x"$enable_graphics" = "xtrue")

# check for castem
CHECK_CASTEM

AC_ARG_ENABLE([pleiades],
               AC_HELP_STRING([--enable-pleiades], [compile mfront pleiades specific parsers]),
[case "${enableval}" in
  yes) enable_pleiades=true ;;
  no)  enable_pleiades=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-enable_pleiades) ;;
esac],[enable_pleiades=false])
AM_CONDITIONAL(SUPPORT_PLEIADES, test x"$enable_pleiades" = "xtrue")
if test x"$enable_pleiades" = "xtrue";
then
	AC_DEFINE(HAVE_PLEIADES)
fi

AC_ARG_ENABLE([tests],
               AC_HELP_STRING([--enable-tests], [compile tests file]),
[case "${enableval}" in
  yes) enable_tests=true ;;
  no)  enable_tests=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-enable_tests) ;;
esac],[enable_tests=false])
AM_CONDITIONAL(WITH_TESTS, test x"$enable_tests" = "xtrue")

if test x"$enable_pleiades" = "xtrue";
then
  AC_DEFINE(HAVE_PLEIADES)
fi

AC_OUTPUT([debug.sh
	   Makefile
	   Doxyfile
	   include/Makefile
	   po/Makefile.in
	   src/Makefile
	   src/Exception/Makefile
	   src/Utilities/Makefile
	   src/Math/Makefile
	   src/Material/Makefile
	   src/System/Makefile
	   src/TFELConfig/Makefile
	   src/TFELConfig/tfel-config.hxx
	   src/Graphics/Makefile
	   mfront/Makefile
	   mfront/include/Makefile
	   mfront/src/Makefile
           mfront/src/MFrontHeader.cxx
           mfront/tests/Makefile
           mfront/tests/properties/Makefile
           mfront/tests/behaviours/Makefile
           mfront/lisp/Makefile
	   mfront/Examples/Makefile
	   tfel-check/Makefile
	   tfel-check/src/Makefile
	   tfel-check/include/Makefile
           tplot/Makefile
	   tplot/include/Makefile
	   tplot/src/Makefile
	   tplot/data/Makefile
	   tests/Makefile
	   tests/Math/Makefile
	   tests/Math/include/Makefile
	   tests/Math/ExpressionTemplates/Makefile
	   tests/Math/ExpressionTemplates/include/Makefile
	   tests/Math/RungeKutta/Makefile
	   tests/Math/RungeKutta/include/Makefile
	   tests/Math/Functions/Makefile
	   tests/Math/St2toSt2/Makefile
	   tests/Math/Composite/Makefile
	   tests/Lambda/Makefile
	   tests/FSAlgorithms/Makefile
	   tests/Utilities/Makefile
	   tests/UnaryTests/Makefile
	   tests/UnaryTests/include/Makefile
	   tests/MetaProgramming/Makefile
	   tests/Material/Makefile
	   tests/System/Makefile
	   ])

