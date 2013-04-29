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
AC_LIBTOOL_WIN32_DLL
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

case $host in
    i[[3456]]86*mingw*)
    WIN32="yes"
    ;;
    i[[3456]]86*cygwin*)
    WIN32="yes"
    ;;
    *)
	WIN32="no"
	;;
esac    

case $host in
    x86_64*mingw*)
	WIN64="yes"
	;;
    x86_64*cygwin*)
	WIN64="yes"
	;;
    *)
	WIN64="no"
	;;
esac    

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
# enable python bindings
use_python_bindings=no
AC_ARG_ENABLE([python_bindings],
    AC_HELP_STRING([--enable-python-bindings],
	[Compile python bindings and python tests]),
    [use_python_bindings="yes"],[])

have_python=no
if test "x$use_python" != "xno" -o "x$use_python_bindings" != "xno" ;
then
    AM_PATH_PYTHON([2.4],,[:])
    if test  "$PYTHON" != : ;
	then
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
	PYTHONPATH=$(echo $PYTHON |sed 's/\/bin\/python.*//')
	AC_SUBST(PYTHONPATH)
	have_python=yes
    fi
fi

if test "x$use_python" != "xno";
   then
    if test "x$have_python" == "xno";
    then
	AC_MSG_ERROR("'python' not found. python is mandatory for python support.")
    fi
    AC_DEFINE(HAVE_PYTHON)
    AM_CONDITIONAL([HAVE_PYTHON],true)
else
    AM_CONDITIONAL([HAVE_PYTHON],false)
fi

if test "x$use_python_bindings" != "xno";
    then  
    if test "x$have_python" == "xno";
    then
	AC_MSG_ERROR("'python' not found. python is mandatory for python bindings.")
    fi
    CHECK_BOOST
    CHECK_BOOST_PYTHON
    TFEL_PYTHON_BINDINGS=yes
    AC_DEFINE(HAVE_TFEL_PYTHON_BINDINGS)
    AM_CONDITIONAL([TFEL_PYTHON_BINDINGS],true)
else
    TFEL_PYTHON_BINDINGS=no
    AM_CONDITIONAL([TFEL_PYTHON_BINDINGS],false)
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
local_castem_header=false
AC_ARG_ENABLE([local-castem-header],
               AC_HELP_STRING([--enable-local-castem-header],
		   [use TFEL own castem.h header file]),
	       [local_castem_header=true],
	       [local_castem_header=false])

CHECK_CASTEM_UNIX_TYPE
if(test "x$local_castem_header" = "xtrue")
then
    AC_DEFINE(HAVE_CASTEM)
    AM_CONDITIONAL(HAVE_CASTEM,true)
    AC_DEFINE(LOCAL_CASTEM_HEADER)
    AM_CONDITIONAL(LOCAL_CASTEM_HEADER,true)
else
    CHECK_CASTEM
    AM_CONDITIONAL(LOCAL_CASTEM_HEADER,false)
fi

#aster
AC_ARG_ENABLE([aster],
               AC_HELP_STRING([--enable-aster],
		   [build aster interface for mechanical behaviours]),
	       [enable_aster=true],
	       [enable_aster=false])
AM_CONDITIONAL(HAVE_ASTER,[test x"$enable_aster" = "xtrue"])
if(test "x$enable_aster" = "xtrue")
then
    AC_DEFINE(HAVE_ASTER)
    CHECK_ASTER_MACHINE_TYPE
fi

AC_ARG_ENABLE([tests],
               AC_HELP_STRING([--enable-tests], [compile tests file]),
[case "${enableval}" in
  yes) enable_tests=true ;;
  no)  enable_tests=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable_tests) ;;
esac],[enable_tests=false])
AM_CONDITIONAL(WITH_TESTS, test x"$enable_tests" = "xtrue")

AM_CONDITIONAL([TFEL_WIN],[test x"$WIN32" = "xyes" -o x"$WIN64" = "xyes"])
AM_CONDITIONAL([TFEL_WIN32],[test x"$WIN32" = "xyes"])
AM_CONDITIONAL([TFEL_WIN64],[test x"$WIN64" = "xyes"])

AC_OUTPUT([debug.sh
	   Makefile
	   Doxyfile
	   docs/Makefile
	   docs/mtest/Makefile
	   docs/mtest/mtest.tex
	   docs/mtest/Champs.tex
	   docs/mfront/Makefile
	   docs/mfront/Makefile.doc
	   docs/mfront/mfront.tex
	   docs/mfront/behaviours.tex
	   docs/mfront/aster.tex
	   docs/mfront/umat.tex
	   docs/mfront/glossary.tex
	   docs/mfront/InternalNames.tex
	   docs/mfront/Champs.tex
           docs/mfront/annexe-orthotropie.tex
	   docs/tex/Makefile
	   docs/tex/texmf/note_technique_2010.cls
	   docs/tex/texmf/couleurs.sty
	   docs/tex/texmf/gnuplot.sty
	   docs/tex/texmf/helvetCEA.sty
	   docs/tex/texmf/LSC.tex
	   docs/tex/texmf/mathematiques.sty
	   docs/tex/texmf/mecanique.sty
	   docs/tex/texmf/mise_en_page_2010.tex
	   docs/tex/texmf/presentation.sty
	   docs/tex/texmf/SESC.tex
	   docs/tex/texmf/variables_2010.tex
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
           mfront/mtest/Makefile
           mfront/tests/Makefile
           mfront/tests/properties/Makefile
           mfront/tests/behaviours/Makefile
           mfront/tests/behaviours/aster/Makefile
           mfront/tests/behaviours/aster/elasticity.mtest
           mfront/tests/behaviours/aster/elasticity2.mtest
           mfront/tests/behaviours/aster/elasticity3.mtest
           mfront/tests/behaviours/aster/elasticity4.mtest
           mfront/tests/behaviours/aster/elasticity5.mtest
           mfront/tests/behaviours/aster/elasticity6.mtest
           mfront/tests/behaviours/aster/elasticity7.mtest
           mfront/tests/behaviours/aster/elasticity8.mtest
           mfront/tests/behaviours/aster/elasticity9.mtest
           mfront/tests/behaviours/aster/elasticity10.mtest
           mfront/tests/behaviours/aster/elasticity11.mtest
           mfront/tests/behaviours/aster/elasticity12.mtest
           mfront/tests/behaviours/aster/elasticity13.mtest
           mfront/tests/behaviours/aster/elasticity14.mtest
           mfront/tests/behaviours/aster/norton.mtest
           mfront/tests/behaviours/aster/norton2.mtest
           mfront/tests/behaviours/aster/implicitnorton.mtest
           mfront/tests/behaviours/aster/implicitnorton2.mtest
           mfront/tests/behaviours/aster/implicitorthotropiccreep.mtest
           mfront/tests/behaviours/aster/plasticity.mtest
           mfront/tests/behaviours/aster/strainhardeningcreep.mtest
           mfront/tests/behaviours/castem/Makefile
           mfront/tests/behaviours/castem/elasticity.mtest
           mfront/tests/behaviours/castem/elasticity2.mtest
           mfront/tests/behaviours/castem/elasticity3.mtest
           mfront/tests/behaviours/castem/elasticity4.mtest
           mfront/tests/behaviours/castem/elasticity5.mtest
           mfront/tests/behaviours/castem/elasticity6.mtest
           mfront/tests/behaviours/castem/elasticity7.mtest
           mfront/tests/behaviours/castem/elasticity8.mtest
           mfront/tests/behaviours/castem/elasticity9.mtest
           mfront/tests/behaviours/castem/elasticity10.mtest
           mfront/tests/behaviours/castem/elasticity11.mtest
           mfront/tests/behaviours/castem/elasticity12.mtest
           mfront/tests/behaviours/castem/elasticity13.mtest
           mfront/tests/behaviours/castem/elasticity14.mtest
           mfront/tests/behaviours/castem/norton.mtest
           mfront/tests/behaviours/castem/norton2.mtest
           mfront/tests/behaviours/castem/orthotropiccreep.mtest
           mfront/tests/behaviours/castem/plasticity.mtest
           mfront/tests/behaviours/castem/strainhardeningcreep.mtest
	   mfront/Examples/Makefile
	   tfel-doc/Makefile
	   tfel-doc/src/Makefile
	   tfel-doc/include/Makefile
	   bindings/Makefile
	   bindings/python/Makefile
	   bindings/python/std/Makefile
	   bindings/python/tfel/Makefile
	   bindings/python/include/Makefile
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

