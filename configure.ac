# Process this file with autoconf to produce a configure script.
# Process this file with automake to produce Makefile.in files.

AC_INIT(tfel,2.0-dev,thomas.helfer@cea.fr)
AC_CONFIG_SRCDIR([include/TFEL/Config/TFELConfig.hxx])
AC_CONFIG_COMMANDS
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

# Automake specific stuff
AM_INIT_AUTOMAKE([foreign dist-bzip2 tar-ustar])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

define([svnrevision],esyscmd([sh -c "svnversion|tr -d '\n'"]))
AC_DEFINE(TFEL_SVN_REVISION,"svnrevision",[SVN REVISION])
AC_SUBST(TFEL_SVN_REVISION,"svnrevision")

#disable static library generation by default
AC_ENABLE_STATIC(no)

# disable automatic declaration of
# CXXFLAGS by AC_PROG_CXX
if test "x$CXXFLAGS" = "x";
    then
    CXXFLAGS=""
fi

AC_PROG_CXX

AC_CHECKING("architecture type")

TFEL_WIN32="no"
TFEL_WIN64="no"
TFEL_LINUX="no"

case $host in
# machine 386 -> 686, 32 bits
    i[[3456]]86*linux*)
	AC_DEFINE(TFEL_ARCH32)
	TFEL_LINUX="yes"
        ;;
# machine amd opteron, 64 bits
    x86_64*linux*)
	AC_DEFINE(TFEL_ARCH64)
	TFEL_LINUX="yes"
        ;;
# machine 386 -> 686, 32 bits
    i[[3456]]86*bsd*)
	AC_DEFINE(TFEL_ARCH32)
        ;;
# machine amd opteron, 64 bits
    x86_64*bsd*)
	AC_DEFINE(TFEL_ARCH64)
        ;;
    x86_64*-mingw32*)
# Pas sûr, mais c'est pour le test
	AC_DEFINE(TFEL_ARCH32)
	TFEL_WIN32="yes"
	;;
    i*86-*-mingw32*)
	AC_DEFINE(TFEL_ARCH32)
	TFEL_WIN32="yes"
	;;
    x86_64*mingw*)
	AC_DEFINE(TFEL_ARCH64)
	TFEL_WIN64="yes"
	;;
    i[[3456]]86*cygwin*)
	AC_DEFINE(TFEL_ARCH32)
	TFEL_WIN32="yes"
	;;
    x86_64*cygwin*)
	TFEL_WIN64="yes"
	AC_DEFINE(TFEL_ARCH64)
	;;
    # ben on ne sait pas :-(
    *)
        AC_MSG_ERROR(unsupported host $host)
        ;;
esac

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
OPTIMISATION_FLAGS2="-DNO_RUNTIME_CHECK_BOUNDS -DNDEBUG"

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
AC_DEFINE_UNQUOTED(OPTIMISATION_FLAGS2,"$OPTIMISATION_FLAGS2")
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

AM_CONDITIONAL([TFEL_WIN],[test x"$TFEL_WIN32" = "xyes" -o x"$TFEL_WIN64" = "xyes"])
AM_CONDITIONAL([TFEL_LINUX],[test x"$TFEL_LINUX" = "xyes"])
AM_CONDITIONAL([TFEL_WIN32],[test x"$TFEL_WIN32" = "xyes"])
AM_CONDITIONAL([TFEL_WIN64],[test x"$TFEL_WIN64" = "xyes"])

AC_OUTPUT([debug.sh
	   Makefile
	   Doxyfile
	   docs/Makefile
	   docs/tfel/Makefile
	   docs/tfel/Makefile.doc
	   docs/tfel/reference-guide.tex
	   docs/tfel/Champs.tex
	   docs/mtest/Makefile
	   docs/mtest/Makefile.doc
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
	   docs/presentations/Makefile
	   docs/presentations/Makefile.doc
	   docs/presentations/mtest-presentation.tex
	   docs/presentations/mfront-presentation.tex
	   docs/presentations/mfront-behaviours.tex
	   docs/tex/Makefile
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
           mfront/tests/behaviours/aster/asterelasticity.mtest
           mfront/tests/behaviours/aster/asterelasticity2.mtest
           mfront/tests/behaviours/aster/asterelasticity3.mtest
           mfront/tests/behaviours/aster/asterelasticity4.mtest
           mfront/tests/behaviours/aster/asterelasticity5.mtest
           mfront/tests/behaviours/aster/asterelasticity6.mtest
           mfront/tests/behaviours/aster/asterelasticity7.mtest
           mfront/tests/behaviours/aster/asterelasticity8.mtest
           mfront/tests/behaviours/aster/asterelasticity9.mtest
           mfront/tests/behaviours/aster/asterelasticity10.mtest
           mfront/tests/behaviours/aster/asterelasticity11.mtest
           mfront/tests/behaviours/aster/asterelasticity12.mtest
           mfront/tests/behaviours/aster/asterelasticity13.mtest
           mfront/tests/behaviours/aster/asterelasticity14.mtest
           mfront/tests/behaviours/aster/asterelasticity15.mtest
           mfront/tests/behaviours/aster/asterelasticity16.mtest
           mfront/tests/behaviours/aster/astermazars.mtest
           mfront/tests/behaviours/aster/asterlorentz.mtest
           mfront/tests/behaviours/aster/asternorton.mtest
           mfront/tests/behaviours/aster/asternorton2.mtest
           mfront/tests/behaviours/aster/asterstressrelaxation.mtest
           mfront/tests/behaviours/aster/asterorthotropiccreep.mtest
           mfront/tests/behaviours/aster/asterorthotropiccreep-rk42.mtest
           mfront/tests/behaviours/aster/asterorthotropiccreep-rkCastem.mtest
           mfront/tests/behaviours/aster/asterimplicitnorton.mtest
	   mfront/tests/behaviours/aster/asterimplicitnorton-generalisedplanestress.mtest
           mfront/tests/behaviours/aster/asterimplicitnorton2.mtest
           mfront/tests/behaviours/aster/asterimplicitnorton_broyden.mtest
           mfront/tests/behaviours/aster/asterimplicitnorton_broyden2.mtest
           mfront/tests/behaviours/aster/asterimplicitorthotropiccreep.mtest
           mfront/tests/behaviours/aster/asterellipticcreep.mtest
           mfront/tests/behaviours/aster/asterplasticity.mtest
           mfront/tests/behaviours/aster/asterstrainhardeningcreep.mtest
           mfront/tests/behaviours/aster/asterplasisocin.mtest
           mfront/tests/behaviours/aster/asterviscoplasisocin.mtest
           mfront/tests/behaviours/aster/asterchaboche.mtest
           mfront/tests/behaviours/aster/asterchaboche2.mtest
           mfront/tests/behaviours/aster/astermonocristal.mtest
           mfront/tests/behaviours/aster/astermonocristal-2.mtest
           mfront/tests/behaviours/aster/asterimplicitmonocristal.mtest
           mfront/tests/behaviours/aster/asterimplicitmonocristal-2.mtest
           mfront/tests/behaviours/aster/astermonocristal_dd_cfc.mtest
           mfront/tests/behaviours/aster/astermonocristal_dd_cfc_numericaljacobian.mtest
           mfront/tests/behaviours/aster/astermonocristal_dd_cfc_irradiation.mtest
           mfront/tests/behaviours/aster/astermonocristal_dd_cc_numericaljacobian.mtest
           mfront/tests/behaviours/aster/astermonocristal_dd_cc_irradiation_numericaljacobian.mtest
           mfront/tests/behaviours/aster/asterpolycristals.mtest
           mfront/tests/behaviours/aster/asterpolycristals_dd_cfc.mtest
           mfront/tests/behaviours/aster/asterimplicithayhurst.mtest
           mfront/tests/behaviours/aster/asterburger.mtest
           mfront/tests/behaviours/castem/Makefile
           mfront/tests/behaviours/castem/castemelasticity.mtest
           mfront/tests/behaviours/castem/castemelasticity2.mtest
           mfront/tests/behaviours/castem/castemelasticity3.mtest
           mfront/tests/behaviours/castem/castemelasticity4.mtest
           mfront/tests/behaviours/castem/castemelasticity5.mtest
           mfront/tests/behaviours/castem/castemelasticity6.mtest
           mfront/tests/behaviours/castem/castemelasticity7.mtest
           mfront/tests/behaviours/castem/castemelasticity8.mtest
           mfront/tests/behaviours/castem/castemelasticity9.mtest
           mfront/tests/behaviours/castem/castemelasticity10.mtest
           mfront/tests/behaviours/castem/castemelasticity11.mtest
           mfront/tests/behaviours/castem/castemelasticity12.mtest
           mfront/tests/behaviours/castem/castemelasticity13.mtest
           mfront/tests/behaviours/castem/castemelasticity14.mtest
           mfront/tests/behaviours/castem/castemelasticity15.mtest
           mfront/tests/behaviours/castem/castemelasticity16.mtest
           mfront/tests/behaviours/castem/castemmazars.mtest
           mfront/tests/behaviours/castem/castemlorentz.mtest
           mfront/tests/behaviours/castem/castemdruckerprager.mtest
           mfront/tests/behaviours/castem/castemdruckerprager2.mtest
           mfront/tests/behaviours/castem/castemnorton.mtest
           mfront/tests/behaviours/castem/castemnorton2.mtest
           mfront/tests/behaviours/castem/castemnortonrk2.mtest
           mfront/tests/behaviours/castem/castemnortonrk3.mtest
           mfront/tests/behaviours/castem/castemnortonrk4.mtest
           mfront/tests/behaviours/castem/castemimplicitnorton-planestress.mtest
           mfront/tests/behaviours/castem/castemimplicitnorton-planestress2.mtest
           mfront/tests/behaviours/castem/castemimplicitnorton-generalisedplanestress.mtest
           mfront/tests/behaviours/castem/castemnorton-euler.mtest
           mfront/tests/behaviours/castem/castemnorton-rk2.mtest
           mfront/tests/behaviours/castem/castemnorton-rk4.mtest
           mfront/tests/behaviours/castem/castemellipticcreep.mtest
           mfront/tests/behaviours/castem/castemstressrelaxation.mtest
           mfront/tests/behaviours/castem/castemorthotropiccreep.mtest
           mfront/tests/behaviours/castem/castemorthotropiccreep-rk42.mtest
           mfront/tests/behaviours/castem/castemorthotropiccreep-rkCastem.mtest
           mfront/tests/behaviours/castem/castemplasticity.mtest
           mfront/tests/behaviours/castem/castemstrainhardeningcreep.mtest
           mfront/tests/behaviours/castem/castemplasisocin.mtest
           mfront/tests/behaviours/castem/castemviscoplasisocin.mtest
           mfront/tests/behaviours/castem/castemchaboche.mtest
           mfront/tests/behaviours/castem/castemchaboche2.mtest
           mfront/tests/behaviours/castem/castemmonocristal.mtest
           mfront/tests/behaviours/castem/castemmonocristal-2.mtest
           mfront/tests/behaviours/castem/castemimplicitmonocristal.mtest
           mfront/tests/behaviours/castem/castemimplicitmonocristal-2.mtest
           mfront/tests/behaviours/castem/castemmonocristal_dd_cfc.mtest
           mfront/tests/behaviours/castem/castemmonocristal_dd_cfc_numericaljacobian.mtest
           mfront/tests/behaviours/castem/castemmonocristal_dd_cfc_irradiation.mtest
           mfront/tests/behaviours/castem/castemmonocristal_dd_cc_numericaljacobian.mtest
           mfront/tests/behaviours/castem/castemmonocristal_dd_cc_irradiation_numericaljacobian.mtest
           mfront/tests/behaviours/castem/castempolycristals.mtest
           mfront/tests/behaviours/castem/castempolycristals_dd_cfc.mtest
           mfront/tests/behaviours/castem/castemhayhurst.mtest
           mfront/tests/behaviours/castem/castemimplicithayhurst.mtest
           mfront/tests/behaviours/castem/castemburger.mtest
           mfront/tests/behaviours/castem/castemtvergaard.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-shear-1-2D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-shear-2-2D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-shear-1-3D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-shear-2-3D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-shear-3-3D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-shear-4-3D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-shear-5-3D.mtest
           mfront/tests/behaviours/castem/castemsaintvenantkirchhoffelasticity-shear-6-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-shear-1-2D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-shear-2-2D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-shear-1-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-shear-2-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-shear-3-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-shear-4-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-shear-5-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity-shear-6-3D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/castem/castemincompressiblemooneyrivlinbehaviour.mtest
           mfront/tests/behaviours/castem/castemincompressibleplanestressgornetdesmoratbehavioursheartesting.mtest
           mfront/tests/behaviours/castem/castemincompressibleplanestressgornetdesmoratbehaviouruniaxialtesting.mtest
           mfront/tests/behaviours/castem/castemcompressibleneohookeanbehaviour.mtest
           mfront/tests/behaviours/castem/castemincompressibleplanestressmooneyrivlinbehaviouruniaxialtesting.mtest
           mfront/tests/behaviours/castem/castemincompressibleplanestressgornetdesmoratbehaviourbiaxialtesting.mtest
           mfront/tests/behaviours/castem/castemimplicitsimomieheelastoplasticity.mtest
           mfront/tests/behaviours/castem/dgibi/Makefile
	   mfront/Examples/Makefile
	   tfel-doc/Makefile
	   tfel-doc/src/Makefile
	   tfel-doc/include/Makefile
	   bindings/Makefile
	   bindings/python/Makefile
	   bindings/python/std/Makefile
	   bindings/python/tfel/Makefile
	   bindings/python/mfront/Makefile
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
	   tests/Math/st2tost2/Makefile
	   tests/Math/t2tost2/Makefile
	   tests/Math/Composite/Makefile
	   tests/Lambda/Makefile
	   tests/FSAlgorithms/Makefile
	   tests/Utilities/Makefile
	   tests/MetaProgramming/Makefile
	   tests/Material/Makefile
	   tests/System/Makefile
	   ])

