# Process this file with autoconf to produce a configure script.
# Process this file with automake to produce Makefile.in files.

AC_INIT(tfel,2.0.2-dev,thomas.helfer@cea.fr)
AC_CONFIG_SRCDIR([include/TFEL/Config/TFELConfig.hxx])
AC_CONFIG_COMMANDS
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

# Automake specific stuff
AM_INIT_AUTOMAKE([foreign dist-bzip2 tar-ustar])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

# ok, this is ugly, but is sufficient for our needs
# remember that using autotools is considered deprecated
case "$libdir" in
  *lib64)
	AC_DEFINE(LIB_SUFFIX,"64",[library directory suffix])	
	;;
    *)
	;;
esac


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

#disable default flags
if test "x$CFLAGS" = "x";
    then
    CFLAGS="";
fi

AC_PROG_CXX

AC_CHECKING("architecture type")

TFEL_WIN="no"
TFEL_LINUX="no"
TFEL_ARCH32="no"
TFEL_ARCH64="no"

case $host in
# machine 386 -> 686, 32 bits
    i[[3456]]86*linux*)
	TFEL_ARCH32="yes"
	TFEL_LINUX="yes"
        ;;
# machine amd opteron, 64 bits
    x86_64*linux*)
	TFEL_ARCH64="yes"
	TFEL_LINUX="yes"
        ;;
# machine 386 -> 686, 32 bits
    i[[3456]]86*bsd*)
	TFEL_ARCH32="yes"
        ;;
# machine amd opteron, 64 bits
    x86_64*bsd*)
	TFEL_ARCH64="yes"
        ;;
    x86_64*-mingw32*)
# Pas sûr, on verra plus tard
	TFEL_ARCH32="yes"
	TFEL_WIN="yes"
	;;
    i*86-*-mingw32*)
	TFEL_ARCH32="yes"
	TFEL_WIN="yes"
	;;
    x86_64*mingw*)
	TFEL_ARCH64="yes"
	TFEL_WIN="yes"
	;;
    i[[3456]]86*cygwin*)
	TFEL_ARCH32="yes"
	TFEL_WIN="yes"
	;;
    x86_64*cygwin*)
	TFEL_WIN="yes"
	TFEL_ARCH64="yes"
	;;
    # ben on ne sait pas :-(
    *)
        AC_MSG_ERROR(unsupported host $host)
        ;;
esac

if test "x$TFEL_WIN" = "xyes" -a "x$TFEL_ARCH32" = "xyes" ;
then
    # When compiling against Cast3M, a 32 bits compiler can be used on a
    # 64 bits machine, so we explicitely test if we are on a 64 bits
    # system
    if test -d '/c/Program Files (x86)' ;
    then
	TFEL_ARCH32="no"
	TFEL_ARCH64="yes"
    fi
fi

if test "x$TFEL_ARCH32" = "xyes" ;
then
	AC_DEFINE(TFEL_ARCH32)
fi

if test "x$TFEL_ARCH64" = "xyes" ;
then
	AC_DEFINE(TFEL_ARCH64)
fi


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
AC_ARG_ENABLE([portable_build],
	       AC_HELP_STRING([--enable-portable_build],
		              [produce binary that can be shared between various machine
			       (same architecture, same gcc version, different processors)]))
AC_ARG_ENABLE([optimizations],
	       AC_HELP_STRING([--disable-optimizations],
		              [Do not try to guess what optimization to enable]))

# Enable debug
AC_ARG_ENABLE([debug],
	AC_HELP_STRING([--enable-debug], [Compile with debug options]))


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
COMPILER_FLAGS="";
COMPILER_WARNINGS="";
OPTIMISATION_FLAGS0=""
OPTIMISATION_FLAGS=""
OPTIMISATION_FLAGS2=""

# g++ specific treatement
if test "x$GXX" == "xyes"; then
    COMPILER_FLAGS="";
    COMPILER_WARNINGS="";
    OPTIMISATION_FLAGS0="-DNO_RUNTIME_CHECK_BOUNDS -DNDEBUG"
    OPTIMISATION_FLAGS=""
    OPTIMISATION_FLAGS2=""
    AC_CHECK_GXX
fi

# intel icc specific treatement
if test "x$INTEL" == "xyes"; then
    COMPILER_FLAGS="";
    COMPILER_WARNINGS="";
    OPTIMISATION_FLAGS0="-DNO_RUNTIME_CHECK_BOUNDS -DNDEBUG"
    OPTIMISATION_FLAGS=""
    OPTIMISATION_FLAGS2=""
    AC_CHECK_INTEL
fi

CFLAGS="$CFLAGS $COMPILER_FLAGS"
CXXFLAGS="$CXXFLAGS $COMPILER_FLAGS"
CXXFLAGS="$CXXFLAGS $COMPILER_WARNINGS"

AC_DEFINE_UNQUOTED(COMPILER_FLAGS,"$COMPILER_FLAGS")
AC_DEFINE_UNQUOTED(OPTIMISATION_FLAGS0,"$OPTIMISATION_FLAGS0")
AC_DEFINE_UNQUOTED(OPTIMISATION_FLAGS,"$OPTIMISATION_FLAGS")
AC_DEFINE_UNQUOTED(OPTIMISATION_FLAGS2,"$OPTIMISATION_FLAGS2")
AC_DEFINE_UNQUOTED(COMPILER_WARNINGS,"$COMPILER_WARNINGS")

# check for C++-11 support
use_cxx11=no;
AC_ARG_ENABLE(cxx11,
   AC_HELP_STRING([--enable-cxx11],
                  [enable C++ 11 support.]),
   use_cxx11=yes,[])
if test "x$use_cxx11" != "xno";
then
  AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])
  AC_DEFINE(HAVE_CXX11)
  AM_CONDITIONAL([HAVE_CXX_11],true)
else
  AM_CONDITIONAL([HAVE_CXX_11],false)
fi

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

#zmat
AC_ARG_ENABLE([zmat],
               AC_HELP_STRING([--enable-zmat],
		   [build zmat interface for mechanical behaviours]),
	       [enable_zmat=true],
	       [enable_zmat=false])
AM_CONDITIONAL(HAVE_ZMAT,[test x"$enable_zmat" = "xtrue"])
if(test "x$enable_zmat" = "xtrue")
then
    CHECK_ZSET
    AC_DEFINE(HAVE_ZMAT)
fi

#cyrano
AC_ARG_ENABLE([cyrano],
               AC_HELP_STRING([--enable-cyrano],
		   [build cyrano interface for mechanical behaviours]),
	       [enable_cyrano=true],
	       [enable_cyrano=false])
AM_CONDITIONAL(HAVE_CYRANO,[test x"$enable_cyrano" = "xtrue"])
if(test "x$enable_cyrano" = "xtrue")
then
    AC_DEFINE(HAVE_CYRANO)
    CHECK_CYRANO_MACHINE_TYPE
fi

AC_ARG_ENABLE([tests],
               AC_HELP_STRING([--enable-tests], [compile tests file]),
[case "${enableval}" in
  yes) enable_tests=true ;;
  no)  enable_tests=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable_tests) ;;
esac],[enable_tests=false])
AM_CONDITIONAL(WITH_TESTS, test x"$enable_tests" = "xtrue")

AM_CONDITIONAL([TFEL_WIN],[test x"$TFEL_WIN" = "xyes"])
AM_CONDITIONAL([TFEL_LINUX],[test x"$TFEL_LINUX" = "xyes"])
AM_CONDITIONAL([TFEL_WIN32],[test x"$TFEL_WIN" = "xyes" -a x"$TFEL_ARCH32" = "xyes"])
AM_CONDITIONAL([TFEL_WIN64],[test x"$TFEL_WIN" = "xyes" -a x"$TFEL_ARCH64" = "xyes"])

AC_OUTPUT([debug.sh
	   Makefile
	   Doxyfile
	   docs/Makefile
	   docs/tfel/Makefile
	   docs/tfel/Makefile.doc
	   docs/tfel/reference-guide.tex
	   docs/tfel/Champs.tex
	   docs/release-notes-2.0/Champs.tex
	   docs/release-notes-2.0/tfel-2.0.tex
	   docs/release-notes-2.0/Makefile.doc
	   docs/release-notes-2.0/Makefile
	   docs/mtest/Makefile
	   docs/mtest/Makefile.doc
	   docs/mtest/mtest.tex
	   docs/mtest/Champs.tex
	   docs/mfront/Makefile
	   docs/mfront/Makefile.doc
	   docs/mfront/mfront.tex
	   docs/mfront/behaviours.tex
	   docs/mfront/finitestrain.tex
	   docs/mfront/planestress.tex
	   docs/mfront/aster.tex
	   docs/mfront/umat.tex
	   docs/mfront/zmat.tex
	   docs/mfront/glossary.tex
	   docs/mfront/InternalNames.tex
	   docs/mfront/Champs.tex
           docs/mfront/annexe-orthotropie.tex
	   docs/presentations/Makefile
	   docs/presentations/Makefile.doc
	   docs/presentations/mtest-presentation.tex
	   docs/presentations/mfront-presentation.tex
	   docs/presentations/mfront-behaviours.tex
	   docs/presentations/mfront-finitestrain.tex
	   docs/presentations/tp.tex
	   docs/presentations/mfront-formation.tex
	   docs/presentations/norton.tex
	   docs/presentations/Chaboche.tex
	   docs/presentations/lemaitre.tex
	   docs/presentations/appendix.tex
	   docs/presentations/mfront-user-meeting-2015/Makefile
	   docs/presentations/mfront-user-meeting-2015/mfront-cea.tex
	   docs/presentations/mfront-user-meeting-2015/tfel-3.0.tex
	   docs/presentations/mfront-user-meeting-2015/writing-portable-behaviour.tex
	   docs/presentations/portable-behaviour/Makefile
	   docs/presentations/Materiaux2014/Makefile
	   docs/presentations/ClubCast3M2014/Makefile
	   docs/tutorial/Makefile
	   docs/tutorial/introduction.tex
	   docs/tutorial/Conclusions.tex
	   docs/tutorial/Chaboche.tex
	   docs/tutorial/resume.tex
	   docs/tutorial/rappels.tex
	   docs/tutorial/Bibliographie.tex
	   docs/html/Makefile
	   docs/tex/Makefile
	   docs/web/Makefile
	   docs/doxygen/Makefile
	   cmake/Makefile
	   cmake/modules/Makefile
	   include/Makefile
	   src/Makefile
	   src/Tests/Makefile
	   src/Exception/Makefile
	   src/Utilities/Makefile
	   src/Glossary/Makefile
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
           mfront/tests/properties/interfaces/Makefile
           mfront/tests/properties/interfaces/include/Makefile
           mfront/tests/properties/interfaces/src/Makefile
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
           mfront/tests/behaviours/aster/asterelasticity18.mtest
           mfront/tests/behaviours/aster/asterelasticity19.mtest
           mfront/tests/behaviours/aster/asterelasticity20.mtest
           mfront/tests/behaviours/aster/asterelasticity21.mtest
           mfront/tests/behaviours/aster/asterelasticity22.mtest
           mfront/tests/behaviours/aster/asterelasticity23.mtest
           mfront/tests/behaviours/aster/astermazars.mtest
           mfront/tests/behaviours/aster/astermazars2.mtest
           mfront/tests/behaviours/aster/asterlorentz.mtest
           mfront/tests/behaviours/aster/asterlorentz2.mtest
           mfront/tests/behaviours/aster/asternorton.mtest
           mfront/tests/behaviours/aster/asternorton2.mtest
           mfront/tests/behaviours/aster/asterstressrelaxation.mtest
           mfront/tests/behaviours/aster/asterorthotropiccreep.mtest
           mfront/tests/behaviours/aster/asterorthotropiccreep-rk42.mtest
           mfront/tests/behaviours/aster/asterorthotropiccreep-rkCastem.mtest
           mfront/tests/behaviours/aster/asterimplicitnorton.mtest
           mfront/tests/behaviours/aster/asterimplicitnorton2.mtest
           mfront/tests/behaviours/aster/asterimplicitnorton3.mtest
           mfront/tests/behaviours/aster/asterimplicitnorton_broyden.mtest
           mfront/tests/behaviours/aster/asterimplicitnorton_broyden2.mtest
           mfront/tests/behaviours/aster/asterimplicitnorton_levenbergmarquardt.mtest	   
           mfront/tests/behaviours/aster/asterimplicitorthotropiccreep.mtest
           mfront/tests/behaviours/aster/asterellipticcreep.mtest
           mfront/tests/behaviours/aster/asterplasticity.mtest
           mfront/tests/behaviours/aster/asterstrainhardeningcreep.mtest
           mfront/tests/behaviours/aster/asterplasisocin.mtest
           mfront/tests/behaviours/aster/asterviscoplasisocin.mtest
           mfront/tests/behaviours/aster/asterchaboche.mtest
           mfront/tests/behaviours/aster/asterchaboche2.mtest
           mfront/tests/behaviours/aster/asterviscochaboche.mtest
	   mfront/tests/behaviours/aster/asterviscochaboche-memo-nrad.py
	   mfront/tests/behaviours/aster/asterxx.py
           mfront/tests/behaviours/aster/astermonocrystal.mtest
           mfront/tests/behaviours/aster/astermonocrystal-2.mtest
           mfront/tests/behaviours/aster/asterimplicitmonocrystal.mtest
           mfront/tests/behaviours/aster/asterimplicitmonocrystal-2.mtest
           mfront/tests/behaviours/aster/astermonocrystal_dd_cfc.mtest
           mfront/tests/behaviours/aster/astermonocrystal_dd_cfc_numericaljacobian.mtest
           mfront/tests/behaviours/aster/astermonocrystal_dd_cfc_irradiation.mtest
           mfront/tests/behaviours/aster/astermonocrystal_dd_cc.mtest
           mfront/tests/behaviours/aster/astermonocrystal_dd_cc_numericaljacobian.mtest
           mfront/tests/behaviours/aster/astermonocrystal_dd_cc_irradiation.mtest
           mfront/tests/behaviours/aster/astermonocrystal_dd_cc_irradiation_numericaljacobian.mtest
           mfront/tests/behaviours/aster/asterpolycrystals.mtest
           mfront/tests/behaviours/aster/asterpolycrystals_dd_cc.mtest
           mfront/tests/behaviours/aster/asterpolycrystals_dd_cfc.mtest
           mfront/tests/behaviours/aster/asterimplicithayhurst.mtest
           mfront/tests/behaviours/aster/asterburger.mtest
           mfront/tests/behaviours/aster/asterageingburger.mtest
	   mfront/tests/behaviours/aster/astergursontvergaardneedlemanplasticflow.mtest
	   mfront/tests/behaviours/aster/astergursontvergaardneedlemanviscoplasticflow.mtest
           mfront/tests/behaviours/aster/asteranistropiclemaitreviscoplasticbehaviour.mtest
           mfront/tests/behaviours/aster/astertvergaard.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-shear-1-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-shear-2-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-shear-1-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-shear-2-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-shear-3-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-shear-4-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-shear-5-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity-shear-6-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-shear-1-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-shear-2-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-shear-1-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-shear-2-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-shear-3-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-shear-4-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-shear-5-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity2-shear-6-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-shear-1-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-shear-2-2D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-shear-1-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-shear-2-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-shear-3-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-shear-4-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-shear-5-3D.mtest
           mfront/tests/behaviours/aster/astersaintvenantkirchhoffelasticity3-shear-6-3D.mtest
           mfront/tests/behaviours/aster/asterfinitestrainsinglecrystal-001.mtest
           mfront/tests/behaviours/aster/asterfinitestrainsinglecrystal-012.mtest
           mfront/tests/behaviours/aster/asterfinitestrainsinglecrystal-012-2.mtest
           mfront/tests/behaviours/aster/asterfinitestrainsinglecrystal-159.mtest
           mfront/tests/behaviours/aster/asterfinitestrainsinglecrystal_numericaljacobian-001.mtest
           mfront/tests/behaviours/aster/asterfinitestrainsinglecrystal_numericaljacobian-012.mtest
           mfront/tests/behaviours/aster/asterfinitestrainsinglecrystal_numericaljacobian-012-2.mtest
           mfront/tests/behaviours/aster/asterfinitestrainsinglecrystal_numericaljacobian-159.mtest
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
           mfront/tests/behaviours/castem/castemelasticity17.mtest
           mfront/tests/behaviours/castem/castemelasticity18.mtest
           mfront/tests/behaviours/castem/castemelasticity19.mtest
           mfront/tests/behaviours/castem/castemelasticity20.mtest
           mfront/tests/behaviours/castem/castemelasticity21.mtest
           mfront/tests/behaviours/castem/castemelasticity22.mtest
           mfront/tests/behaviours/castem/castemelasticity23.mtest
           mfront/tests/behaviours/castem/castemelasticity24.mtest
           mfront/tests/behaviours/castem/castemelasticity25.mtest
           mfront/tests/behaviours/castem/castemelasticity26.mtest
           mfront/tests/behaviours/castem/castemelasticity27.mtest
           mfront/tests/behaviours/castem/castemelasticity28.mtest
           mfront/tests/behaviours/castem/castemelasticity29.mtest
           mfront/tests/behaviours/castem/castemelasticity30.mtest
           mfront/tests/behaviours/castem/castemelasticity31.mtest
           mfront/tests/behaviours/castem/castemelasticity32.mtest
           mfront/tests/behaviours/castem/castemgeneralizedmaxwell.mtest
           mfront/tests/behaviours/castem/castemddif2.mtest
           mfront/tests/behaviours/castem/castemddif2-2.mtest
           mfront/tests/behaviours/castem/castemddif2-3.mtest
           mfront/tests/behaviours/castem/castemddif2-4.mtest
           mfront/tests/behaviours/castem/castemmazars.mtest
           mfront/tests/behaviours/castem/castemmazars2.mtest
           mfront/tests/behaviours/castem/castemlorentz.mtest
           mfront/tests/behaviours/castem/castemlorentz2.mtest
           mfront/tests/behaviours/castem/castemdruckerprager.mtest
           mfront/tests/behaviours/castem/castemdruckerprager2.mtest
           mfront/tests/behaviours/castem/castemnorton.mtest
           mfront/tests/behaviours/castem/castemnorton2.mtest
           mfront/tests/behaviours/castem/castemnortonrk2.mtest
           mfront/tests/behaviours/castem/castemnortonrk3.mtest
           mfront/tests/behaviours/castem/castemnortonrk4.mtest
           mfront/tests/behaviours/castem/castemimplicitnorton.mtest
           mfront/tests/behaviours/castem/castemimplicitnorton2.mtest
           mfront/tests/behaviours/castem/castemimplicitnorton3.mtest
           mfront/tests/behaviours/castem/castemimplicitnorton-planestress.mtest
           mfront/tests/behaviours/castem/castemimplicitnorton-planestress2.mtest
           mfront/tests/behaviours/castem/castemimplicitnorton-levenbergmarquardt.mtest	   
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
           mfront/tests/behaviours/castem/castemviscochaboche.mtest
           mfront/tests/behaviours/castem/castemmonocrystal.mtest
           mfront/tests/behaviours/castem/castemmonocrystal-2.mtest
           mfront/tests/behaviours/castem/castemimplicitmonocrystal.mtest
           mfront/tests/behaviours/castem/castemimplicitmonocrystal-2.mtest
           mfront/tests/behaviours/castem/castemmonocrystal_dd_cfc.mtest
           mfront/tests/behaviours/castem/castemmonocrystal_dd_cfc_numericaljacobian.mtest
           mfront/tests/behaviours/castem/castemmonocrystal_dd_cfc_irradiation.mtest
           mfront/tests/behaviours/castem/castemmonocrystal_dd_cc.mtest
           mfront/tests/behaviours/castem/castemmonocrystal_dd_cc_numericaljacobian.mtest
           mfront/tests/behaviours/castem/castemmonocrystal_dd_cc_irradiation.mtest
           mfront/tests/behaviours/castem/castemmonocrystal_dd_cc_irradiation_numericaljacobian.mtest
           mfront/tests/behaviours/castem/castempolycrystals.mtest
           mfront/tests/behaviours/castem/castempolycrystals_dd_cc.mtest
           mfront/tests/behaviours/castem/castempolycrystals_dd_cfc.mtest
           mfront/tests/behaviours/castem/castemhayhurst.mtest
           mfront/tests/behaviours/castem/castemimplicithayhurst.mtest
	   mfront/tests/behaviours/castem/castemgursontvergaardneedlemanplasticflow.mtest
	   mfront/tests/behaviours/castem/castemgursontvergaardneedlemanviscoplasticflow.mtest
           mfront/tests/behaviours/castem/castemburger.mtest
           mfront/tests/behaviours/castem/castemageingburger.mtest
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
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-shear-1-2D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-shear-2-2D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-shear-1-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-shear-2-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-shear-3-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-shear-4-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-shear-5-3D.mtest
           mfront/tests/behaviours/castem/castemfiniterotationsmallstrainelasticity2-shear-6-3D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-shear-1-2D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-shear-2-2D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-shear-1-3D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-shear-2-3D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-shear-3-3D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-shear-4-3D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-shear-5-3D.mtest
           mfront/tests/behaviours/castem/castemelasticity_frst-shear-6-3D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-planestress.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-planestress.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/castem/castemelasticity_log1D-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/castem/castemelasticity_log1D-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/castem/castemelasticity_log1D-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/castem/castemnorton_log1D-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/castem/castemnorton_log1D-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/castem/castemnorton_log1D-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/castem/castemelasticity_malls-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/castem/castemelasticity_malls-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/castem/castemelasticity_malls-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/castem/castemelasticity_malls-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/castem/castemelasticity_malls-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/castem/castemelasticity_malls-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/castem/castemelasticity_malls-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/castem/castemelasticity_malls-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/castem/castemelasticity_malls-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/castem/castemmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/castem/castemincompressiblemooneyrivlinbehaviour.mtest
           mfront/tests/behaviours/castem/castemincompressibleplanestressgornetdesmoratbehavioursheartesting.mtest
           mfront/tests/behaviours/castem/castemincompressibleplanestressgornetdesmoratbehaviouruniaxialtesting.mtest
           mfront/tests/behaviours/castem/castemcompressibleneohookeanbehaviour.mtest
           mfront/tests/behaviours/castem/castemincompressibleplanestressmooneyrivlinbehaviouruniaxialtesting.mtest
           mfront/tests/behaviours/castem/castemincompressibleplanestressgornetdesmoratbehaviourbiaxialtesting.mtest
           mfront/tests/behaviours/castem/castemimplicitsimomieheelastoplasticity.mtest
           mfront/tests/behaviours/castem/castemT91-satohtest.mtest
           mfront/tests/behaviours/castem/castemT91-viscositytest.mtest
           mfront/tests/behaviours/castem/castemfinitestrainsinglecrystal-001.mtest
           mfront/tests/behaviours/castem/castemfinitestrainsinglecrystal-012.mtest
           mfront/tests/behaviours/castem/castemfinitestrainsinglecrystal-012-2.mtest
           mfront/tests/behaviours/castem/castemfinitestrainsinglecrystal-159.mtest
           mfront/tests/behaviours/castem/castemfinitestrainsinglecrystal2-001.mtest
           mfront/tests/behaviours/castem/castemfinitestrainsinglecrystal_numericaljacobian-001.mtest
           mfront/tests/behaviours/castem/castemfinitestrainsinglecrystal_numericaljacobian-012.mtest
           mfront/tests/behaviours/castem/castemfinitestrainsinglecrystal_numericaljacobian-012-2.mtest
           mfront/tests/behaviours/castem/castemfinitestrainsinglecrystal_numericaljacobian-159.mtest
           mfront/tests/behaviours/castem/castemanistropiclemaitreviscoplasticbehaviour.mtest
           mfront/tests/behaviours/castem/dgibi/Makefile
           mfront/tests/behaviours/castem2/Makefile   
           mfront/tests/behaviours/castem2/castem2burger.mtest
           mfront/tests/behaviours/castem2/castem2chaboche2.mtest
           mfront/tests/behaviours/castem2/castem2chaboche.mtest
           mfront/tests/behaviours/castem2/castem2druckerprager2.mtest
           mfront/tests/behaviours/castem2/castem2druckerprager.mtest
           mfront/tests/behaviours/castem2/castem2elasticity10.mtest
           mfront/tests/behaviours/castem2/castem2elasticity11.mtest
           mfront/tests/behaviours/castem2/castem2elasticity12.mtest
           mfront/tests/behaviours/castem2/castem2elasticity13.mtest
           mfront/tests/behaviours/castem2/castem2elasticity14.mtest
           mfront/tests/behaviours/castem2/castem2elasticity15.mtest
           mfront/tests/behaviours/castem2/castem2elasticity16.mtest
           mfront/tests/behaviours/castem2/castem2elasticity2.mtest
           mfront/tests/behaviours/castem2/castem2elasticity3.mtest
           mfront/tests/behaviours/castem2/castem2elasticity4.mtest
           mfront/tests/behaviours/castem2/castem2elasticity5.mtest
           mfront/tests/behaviours/castem2/castem2elasticity6.mtest
           mfront/tests/behaviours/castem2/castem2elasticity7.mtest
           mfront/tests/behaviours/castem2/castem2elasticity8.mtest
           mfront/tests/behaviours/castem2/castem2elasticity9.mtest
           mfront/tests/behaviours/castem2/castem2elasticity.mtest
           mfront/tests/behaviours/castem2/castem2ellipticcreep.mtest
           mfront/tests/behaviours/castem2/castem2gursontvergaardneedlemanplasticflow.mtest
           mfront/tests/behaviours/castem2/castem2gursontvergaardneedlemanviscoplasticflow.mtest
           mfront/tests/behaviours/castem2/castem2implicithayhurst.mtest
           mfront/tests/behaviours/castem2/castem2implicitmonocrystal-2.mtest
           mfront/tests/behaviours/castem2/castem2implicitmonocrystal.mtest
           mfront/tests/behaviours/castem2/castem2monocrystal-2.mtest
           mfront/tests/behaviours/castem2/castem2monocrystal_dd_cc_irradiation_numericaljacobian.mtest
           mfront/tests/behaviours/castem2/castem2monocrystal_dd_cc.mtest
           mfront/tests/behaviours/castem2/castem2monocrystal_dd_cc_numericaljacobian.mtest
           mfront/tests/behaviours/castem2/castem2monocrystal_dd_cfc_irradiation.mtest
           mfront/tests/behaviours/castem2/castem2monocrystal_dd_cfc.mtest
           mfront/tests/behaviours/castem2/castem2monocrystal_dd_cfc_numericaljacobian.mtest
           mfront/tests/behaviours/castem2/castem2monocrystal.mtest
           mfront/tests/behaviours/castem2/castem2norton2.mtest
           mfront/tests/behaviours/castem2/castem2norton.mtest
           mfront/tests/behaviours/castem2/castem2plasisocin.mtest
           mfront/tests/behaviours/castem2/castem2plasticity.mtest
           mfront/tests/behaviours/castem2/castem2strainhardeningcreep.mtest
           mfront/tests/behaviours/castem2/castem2stressrelaxation.mtest
           mfront/tests/behaviours/castem2/castem2viscoplasisocin.mtest   
	   mfront/tests/behaviours/secant/Makefile
           mfront/tests/behaviours/secant/secantchaboche2.mtest
           mfront/tests/behaviours/secant/secantchaboche.mtest
           mfront/tests/behaviours/secant/secantcompressibleneohookeanbehaviour.mtest
           mfront/tests/behaviours/secant/secantddif2-2.mtest
           mfront/tests/behaviours/secant/secantddif2-3.mtest
           mfront/tests/behaviours/secant/secantddif2-4.mtest
           mfront/tests/behaviours/secant/secantddif2.mtest
           mfront/tests/behaviours/secant/secantdruckerprager2.mtest
           mfront/tests/behaviours/secant/secantdruckerprager.mtest
           mfront/tests/behaviours/secant/secantelasticity_malls-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/secant/secantelasticity_malls-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/secant/secantelasticity_malls-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/secant/secantelasticity_malls-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/secant/secantelasticity_malls-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/secant/secantelasticity_malls-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/secant/secantelasticity_malls-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/secant/secantelasticity_malls-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/secant/secantelasticity_malls-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/secant/secantellipticcreep.mtest
           mfront/tests/behaviours/secant/secantfinitestrainsinglecrystal-001.mtest
           mfront/tests/behaviours/secant/secantfinitestrainsinglecrystal-012-2.mtest
           mfront/tests/behaviours/secant/secantfinitestrainsinglecrystal-012.mtest
           mfront/tests/behaviours/secant/secantfinitestrainsinglecrystal-159.mtest
           mfront/tests/behaviours/secant/secantfinitestrainsinglecrystal2-001.mtest
           mfront/tests/behaviours/secant/secantfinitestrainsinglecrystal_numericaljacobian-001.mtest
           mfront/tests/behaviours/secant/secantfinitestrainsinglecrystal_numericaljacobian-012-2.mtest
           mfront/tests/behaviours/secant/secantfinitestrainsinglecrystal_numericaljacobian-012.mtest
           mfront/tests/behaviours/secant/secantfinitestrainsinglecrystal_numericaljacobian-159.mtest
           mfront/tests/behaviours/secant/secanthayhurst.mtest
           mfront/tests/behaviours/secant/secantimplicithayhurst.mtest
           mfront/tests/behaviours/secant/secantimplicitmonocrystal.mtest
           mfront/tests/behaviours/secant/secantimplicitnorton3.mtest
           mfront/tests/behaviours/secant/secantimplicitnorton-planestress2.mtest
           mfront/tests/behaviours/secant/secantimplicitnorton-planestress.mtest
           mfront/tests/behaviours/secant/secantimplicitsimomieheelastoplasticity.mtest
           mfront/tests/behaviours/secant/secantincompressiblemooneyrivlinbehaviour.mtest
           mfront/tests/behaviours/secant/secantincompressibleplanestressgornetdesmoratbehaviourbiaxialtesting.mtest
           mfront/tests/behaviours/secant/secantincompressibleplanestressgornetdesmoratbehavioursheartesting.mtest
           mfront/tests/behaviours/secant/secantincompressibleplanestressgornetdesmoratbehaviouruniaxialtesting.mtest
           mfront/tests/behaviours/secant/secantincompressibleplanestressmooneyrivlinbehaviouruniaxialtesting.mtest
           mfront/tests/behaviours/secant/secantlorentz.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-planestress.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-planestress.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-1D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-2D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-1D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-2D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-1D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-2D.mtest
           mfront/tests/behaviours/secant/secantmieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D.mtest
           mfront/tests/behaviours/secant/secantmonocrystal_dd_cc_irradiation.mtest
           mfront/tests/behaviours/secant/secantmonocrystal_dd_cc_irradiation_numericaljacobian.mtest
           mfront/tests/behaviours/secant/secantmonocrystal_dd_cfc_irradiation.mtest
           mfront/tests/behaviours/secant/secantmonocrystal_dd_cfc.mtest
           mfront/tests/behaviours/secant/secantmonocrystal_dd_cfc_numericaljacobian.mtest
           mfront/tests/behaviours/secant/secantmonocrystal.mtest
           mfront/tests/behaviours/secant/secantnorton2.mtest
           mfront/tests/behaviours/secant/secantnorton-euler.mtest
           mfront/tests/behaviours/secant/secantnorton.mtest
           mfront/tests/behaviours/secant/secantnorton-rk2.mtest
           mfront/tests/behaviours/secant/secantnortonrk2.mtest
           mfront/tests/behaviours/secant/secantnortonrk3.mtest
           mfront/tests/behaviours/secant/secantnorton-rk4.mtest
           mfront/tests/behaviours/secant/secantnortonrk4.mtest
           mfront/tests/behaviours/secant/secantorthotropiccreep.mtest
           mfront/tests/behaviours/secant/secantorthotropiccreep-rk42.mtest
           mfront/tests/behaviours/secant/secantorthotropiccreep-rkCastem.mtest
           mfront/tests/behaviours/secant/secantplasisocin.mtest
           mfront/tests/behaviours/secant/secantplasticity.mtest
           mfront/tests/behaviours/secant/secantpolycrystals_dd_cfc.mtest
           mfront/tests/behaviours/secant/secantstrainhardeningcreep.mtest
           mfront/tests/behaviours/secant/secantstressrelaxation.mtest
           mfront/tests/behaviours/secant/secantT91-satohtest.mtest
           mfront/tests/behaviours/secant/secantT91-viscositytest.mtest
           mfront/tests/behaviours/secant/secantviscochaboche.mtest
           mfront/tests/behaviours/secant/secantviscoplasisocin.mtest
           mfront/tests/behaviours/cyrano/Makefile
           mfront/tests/behaviours/cyrano/cyranoelasticity.mtest
           mfront/tests/behaviours/cyrano/cyranoelasticity10.mtest
           mfront/tests/behaviours/cyrano/cyranoelasticity18.mtest
           mfront/tests/behaviours/cyrano/cyranoelasticity19.mtest
           mfront/tests/behaviours/cyrano/cyranoelasticity20.mtest
           mfront/tests/behaviours/cyrano/cyranoelasticity21.mtest
           mfront/tests/behaviours/cyrano/cyranonorton.mtest
           mfront/tests/behaviours/cyrano/cyranonorton2.mtest
           mfront/tests/behaviours/cyrano/cyranoellipticcreep.mtest
           mfront/tests/behaviours/cyrano/cyranoorthotropiccreep.mtest
           mfront/tests/behaviours/cyrano/cyranoimplicitnorton.mtest
           mfront/tests/behaviours/cyrano/cyranoimplicitnorton2.mtest
           mfront/tests/behaviours/cyrano/cyranoimplicitnorton3.mtest
           mfront/tests/models/Makefile
           mfront/tests/models/include/Makefile
           mfront/tests/models/src/Makefile
	   mfront/Examples/Makefile
	   tfel-doc/Makefile
	   tfel-doc/src/Makefile
	   tfel-doc/include/Makefile
	   bindings/Makefile
	   bindings/python/Makefile
	   bindings/python/std/Makefile
	   bindings/python/std/__init__.py
	   bindings/python/tfel/Makefile
	   bindings/python/tfel/__init__.py
	   bindings/python/mfront/Makefile
	   bindings/python/mfront/__init__.py
	   bindings/python/mfront/umatxx/Makefile
	   bindings/python/mfront/umatxx/__init__.py
	   bindings/python/mfront/umatxx/Behaviour.py
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
	   tests/Math/t2tot2/Makefile
	   tests/Math/st2tost2/Makefile
	   tests/Math/t2tost2/Makefile
	   tests/Math/st2tot2/Makefile
	   tests/Math/Composite/Makefile
	   tests/Lambda/Makefile
	   tests/FSAlgorithms/Makefile
	   tests/Utilities/Makefile
	   tests/MetaProgramming/Makefile
	   tests/Material/Makefile
	   tests/System/Makefile
	   ])
