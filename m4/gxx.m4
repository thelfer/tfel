dnl 
dnl a set of macros related to gxx
dnl 
dnl Helfer Thomas 22/12/06

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
			     if test "x$TFEL_ARCH32" = "xyes" ;
			     then
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
			     if test "x$TFEL_ARCH32" = "xyes" ;
			     then
				 line="prescott $line"
			     else
				 line="nocona $line"
			     fi
			 fi
		     fi
		     dnl failsafe with default value
		     if test "x$line" == "x" ; then
			 if test "x$TFEL_ARCH32" = "xyes" ;
			 then
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

	     CXXFLAGS="$MY_ARCH_FLAG $MY_FPMATH_FLAG $MY_SSE_FLAG $CXXFLAGS"
	     CFLAGS="$MY_ARCH_FLAG $MY_FPMATH_FLAG $MY_SSE_FLAG $CFLAGS"
	     OPTIMISATION_FLAGS="$MY_ARCH_FLAG $MY_FPMATH_FLAG $MY_SSE_FLAG $OPTIMISATION_FLAGS"

	 ]) dnl end of AC_GCC_LINUX_OPTIMISATIONS

AC_DEFUN([AC_GCC_DEFAULT_OPTIMISATIONS],
	 [
	     OPTIMISATION_FLAGS0="-O2 $OPTIMISATION_FLAGS0"
	     CXXFLAGS="-O2 $CXXFLAGS"
	     CFLAGS="-O2 $CFLAGS"
	 ])

dnl 
dnl guess architecture specific options
dnl 
dnl Helfer Thomas 22/12/06
dnl Helfer Thomas 30/10/14 (add test on '-march=native')
AC_DEFUN([AC_GCC_ARCH_OPTIMISATIONS],[
             AC_GCC_DEFAULT_OPTIMISATIONS
	     if test "x$host" = "x$build";
	     then
		 AC_GCC_CHECK_FLAG(-march=native,NATIVE_MARCH_FLAG)
		 if test "x$NATIVE_MARCH_FLAG" != "x" ;
		 then	
		     if test "x$enable_portable_build" != "xyes";
		     then   	    
			 CXXFLAGS="-march=native $CXXFLAGS"
			 CFLAGS="-march=native $CFLAGS"
		     fi
		     OPTIMISATION_FLAGS="-march=native $OPTIMISATION_FLAGS"
		 else
		     if test "x$enable_portable_build" != "xyes";
		     then   	    
			 echo $host | grep "linux" &> /dev/null
			 if test "x$?" = "x0"; then
			     sys="linux"
			 fi
			 case "$sys" in
			     linux*)
				 AC_GCC_LINUX_OPTIMISATIONS
				 ;;
			     *)
				 ;;
			 esac
		     fi
		 fi
	     fi
	 ])

AC_DEFUN([AC_CHECK_GXX],
	 [
	     if test "x$GXX" = "xyes"; then
		 
		 dnl gcc default warning options
		 CFLAGS="$CFLAGS -W -Wall"
		 
		 dnl g++ default warning options
		 COMPILER_WARNINGS="-W -Wall -Wconversion -Wshadow"
		 COMPILER_WARNINGS="$COMPILER_WARNINGS -Wpointer-arith -Wcast-qual -Wcast-align"
		 COMPILER_WARNINGS="$COMPILER_WARNINGS -Wwrite-strings -Wctor-dtor-privacy -Wnon-virtual-dtor"
		 COMPILER_WARNINGS="$COMPILER_WARNINGS -Woverloaded-virtual -Wreturn-type -Wfloat-equal"
		 COMPILER_WARNINGS="$COMPILER_WARNINGS -Wno-endif-labels  -Wsign-compare -Wmissing-format-attribute"
		 COMPILER_WARNINGS="$COMPILER_WARNINGS -Wno-multichar -Wno-deprecated-declarations -Wpacked"
		 COMPILER_WARNINGS="$COMPILER_WARNINGS -Wredundant-decls -Wlong-long -Wdisabled-optimization"
		 COMPILER_WARNINGS="$COMPILER_WARNINGS -Wunknown-pragmas -Wundef  -Wreorder"

		 if test "${CXX}" = "clang++";
		 then
		     CXXFLAGS="$CXXFLAGS -ftemplate-depth-250"
		     #	       CXXFLAGS="$CXXFLAGS -Wno-mismatched-tags"
		     CXXFLAGS="$CXXFLAGS"
		     COMPILER_FLAGS="-ftemplate-depth-250 -Wno-mismatched-tags $COMPILER_FLAGS"
		 fi

		 if test "${CXX}" != "clang++";
		 then
		     if test "x$TFEL_WIN" = "xyes" ;
		     then
			 if test "x$TFEL_ARCH64" = "xyes" ;
			 then
			     COMPILER_FLAGS="-m64 $COMPILER_FLAGS"
			 fi
			 if test "x$TFEL_ARCH32" = "xyes" ;
			 then
			     COMPILER_FLAGS="-m32 $COMPILER_FLAGS"
			 fi
                     fi
		 fi

		 dnl symbol visibility
		 GCC_SYMBOL_VISIBILITY=""
		 AC_GCC_CHECK_FLAG(-fvisibility-inlines-hidden,GCC_SYMBOL_VISIBILITY)
		 if test "x$GCC_SYMBOL_VISIBILITY" != "x"; then
		     dnl a small test because this compilation flag may cause problems
		     dnl on some systems
		     cat > test-fvisibility-inlines-hidden.cxx <<EOF
#include<string>
#include<sstream>
void
function(const double a)
{
  using namespace std;
  ostringstream c;
  c << a;}
EOF
		     
		     $CXX -fvisibility-inlines-hidden --shared -DPIC -fPIC test-fvisibility-inlines-hidden.cxx -o libtest-fvisibility-inlines-hidden.so &> /dev/null
		     if test x"$?" == "x0" ; then
			 CXXFLAGS="$CXXFLAGS $GCC_SYMBOL_VISIBILITY"
			 OPTIMISATION_FLAGS0="$GCC_SYMBOL_VISIBILITY $OPTIMISATION_FLAGS0"
			 rm libtest-fvisibility-inlines-hidden.so
			 AC_MSG_NOTICE([-fvisibility-inlines-hidden enabled])
		     else
			 AC_MSG_NOTICE([-fvisibility-inlines-hidden disabled])
		     fi
		     rm test-fvisibility-inlines-hidden.cxx
		 fi

		 if test "x$enable_debug" != "xyes"; then
		     dnl g++  no debug options
		     CPPFLAGS="-DNDEBUG -DNO_RUNTIME_CHECK_BOUNDS $CPPFLAGS"
		 fi

		 if test "x$enable_optimizations" != "xno"; then   	    
		     AC_GCC_ARCH_OPTIMISATIONS
		 fi
		 
		 if test "x$enable_debug" == "xyes"; then
		     dnl g++ debug options
		     CXXFLAGS="-g $CXXFLAGS"
		 fi
		 
	         dnl hide all symbols by default
	         GCC_SYMBOL_VISIBILITY=""
	         AC_GCC_CHECK_FLAG(-fvisibility=hidden,GCC_SYMBOL_VISIBILITY)
	         CXXFLAGS="$CXXFLAGS $GCC_SYMBOL_VISIBILITY"
	         CFLAGS="$CFLAGS $GCC_SYMBOL_VISIBILITY"
	         OPTIMISATION_FLAGS0="$GCC_SYMBOL_VISIBILITY $OPTIMISATION_FLAGS0"
		 
		 OPTIMISATION_FLAGS2="-ffast-math"
	     fi
	 ]) dnl end of AC_CHECK GXX
