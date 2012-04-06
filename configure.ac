# Process this file with autoconf to produce a configure script.
# Process this file with automake to produce Makefile.in files.

AC_INIT(tfel,1.1,thomas.helfer@cea.fr)
AC_CONFIG_SRCDIR([include/TFEL/Config/TFELConfig.hxx])
AC_CONFIG_COMMANDS
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CANONICAL_TARGET

# Automake specific stuff
AM_INIT_AUTOMAKE([foreign dist-bzip2])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

define([svnrevision],esyscmd([sh -c "svnversion|tr -d '\n'"]))
AC_DEFINE(TFEL_SVN_REVISION,"svnrevision",[SVN REVISION])

#disable static library generation by default
AC_ENABLE_STATIC(no)

# disable automatic declaration of
# CXXFLAGS by AC_PROG_CXX
if test "x$CXXFLAGS" = "x";
    then
    CXXFLAGS=""
fi

AC_PROG_CXX

# Macro for lisp
# AM_PATH_LISPDIR

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

# Compiler specific stuff
AC_ARG_ENABLE([optimizations],
	       AC_HELP_STRING([--disable-optimizations],
		              [Do not try to guess what optimization to enable]))

# Enable debug
AC_ARG_ENABLE([debug],
	AC_HELP_STRING([--enable-debug], [Compile with debug options]))

#disable default flags
CFLAGS="";

#choose between intel icpc and g++ and ekopath
INTEL="no"
PATHSCALE="no"
if test "x$GXX" = "xyes" -o "${CXX##*/}x" = "icpcx" ; then
    if test "${CXX##*/}x" = "icpcx"; then
	GXX="no"
	INTEL="yes"
    fi
fi
if test "x$GXX" = "xyes" -o "${CXX##*/}x" = "pathCCx" ; then
    if test "${CXX##*/}x" = "pathCCx"; then
	GXX="no"
	PATHSCALE="yes"
    fi
fi

# check for compiler
COMPILER_SPECIFIC_OPTIONS="";
COMPILER_WARNINGS="";
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
AC_DEFINE_UNQUOTED(COMPILER_WARNINGS,"$COMPILER_WARNINGS")

# Checks for compilers
# disable automatic declaration of
# FFLAGS by AC_PROG_F77
use_fortran=no;
AC_ARG_ENABLE(fortran,
   AC_HELP_STRING([--enable-fortran],
                  [enable fortran support.]),
   use_fortran=yes,[])

if test "x$use_fortran" != "xno";
    then
    if test "x$FFLAGS" = "x";
	then
	FFLAGS=""
    fi
    AC_PROG_F77
    AC_F77_WRAPPERS
    AC_DEFINE(HAVE_FORTRAN)
    AM_CONDITIONAL([HAVE_FORTRAN],true)
else
    AM_CONDITIONAL([HAVE_FORTRAN],false)
fi

# checks for python
use_python=no;
AC_ARG_ENABLE(python,
   AC_HELP_STRING([--enable-python],
                  [enable python support.]),
   use_python=yes,[])

if test "x$use_python" != "xno";
    then
    AM_PATH_PYTHON([2.2],,[:])
    AM_CONDITIONAL([HAVE_PYTHON], [test "$PYTHON" != :])
    if test  "$PYTHON" != : ;
	then
	AC_DEFINE(HAVE_PYTHON)
#python includes
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
#python libraries
	file=$(mktemp)
	cat > $file << EOF
#!$PYTHON
from distutils import sysconfig
print '-L' + sysconfig.PREFIX  +'/lib/ ' + '-lpython'+sysconfig.get_python_version()
EOF
	PYTHON_LIBS=$($PYTHON $file)
	rm $file
	AC_DEFINE_UNQUOTED(TFEL_PYTHON_LIBS,"$PYTHON_LIBS")
    else
	AC_MSG_ERROR("'python' not found")
    fi
else
    AM_CONDITIONAL([HAVE_PYTHON],false)
fi

#gnuplot
AC_CHECK_PROGS(GNUPLOT,gnuplot,[:])
AM_CONDITIONAL([HAVE_GNUPLOT], [test "$GNUPLOT" != :])
if test "$GNUPLOT" != :
then
	AC_DEFINE_UNQUOTED(GNUPLOT_PATH,"$GNUPLOT")
	AC_DEFINE(HAVE_GNUPLOT)
fi

# check for castem
CHECK_CASTEM

AC_ARG_ENABLE([tests],
               AC_HELP_STRING([--enable-tests], [compile tests file]),
[case "${enableval}" in
  yes) enable_tests=true ;;
  no)  enable_tests=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-enable_tests) ;;
esac],[enable_tests=false])
AM_CONDITIONAL(WITH_TESTS, test x"$enable_tests" = "xtrue")

AC_OUTPUT([debug.sh
	   Makefile
	   Doxyfile
	   cmake/Makefile
	   cmake/modules/Makefile
	   include/Makefile
	   src/Makefile
	   src/Tests/Makefile
	   src/Exception/Makefile
	   src/Utilities/Makefile
	   src/Math/Makefile
	   src/Material/Makefile
	   src/System/Makefile
	   src/TFELConfig/Makefile
	   src/TFELConfig/tfel-config.hxx
	   mfront/Makefile
	   mfront/include/Makefile
	   mfront/src/Makefile
           mfront/src/MFrontHeader.cxx
           mfront/tests/Makefile
           mfront/tests/properties/Makefile
           mfront/tests/behaviours/Makefile
	   mfront/Examples/Makefile
	   tfel-doc/Makefile
	   tfel-doc/src/Makefile
	   tfel-doc/include/Makefile
	   tests/Makefile
	   tests/Tests/Makefile
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
	   tests/MetaProgramming/Makefile
	   tests/Material/Makefile
	   tests/System/Makefile
	   ])

