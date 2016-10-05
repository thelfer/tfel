checkfiles:
	if test "@top_srcdir@" != "@top_builddir@";                                 \
	then                                                                        \
		cp @srcdir@/test1.check  test1.check;                     \
		cp @srcdir@/test2.check  test2.check;                     \
		cp @srcdir@/test2.ref    test2.ref;                       \
		cp @srcdir@/test3.check  test3.check;                     \
		cp @srcdir@/test3.ref    test3.ref;                       \
		cp @srcdir@/test4.check  test4.check;                     \
		cp @srcdir@/test4.ref    test4.ref;                       \
		cp @srcdir@/test4.check  test5.check;                     \
		cp @srcdir@/test4.ref    test5.ref;                       \
		cp @srcdir@/test4.check  test6.check;                     \
		cp @srcdir@/test4.ref    test6.ref;                       \
		cp @srcdir@/UPuN_GaseousFissionProductsReleaseModel.check \
		   UPuN_GaseousFissionProductsReleaseModel.check;                   \
		cp @srcdir@/UPuN_GaseousFissionProductsReleaseModel.ref   \
		   UPuN_GaseousFissionProductsReleaseModel.ref;                     \
		cp @srcdir@/UPuN_GaseousFissionProductsReleaseModel.txt   \
		   UPuN_GaseousFissionProductsReleaseModel.txt;                     \
		cp @srcdir@/Alcyone1D.ref    Alcyone1D.ref;               \
		cp @srcdir@/Alcyone1D.res    Alcyone1D.res;               \
		cp @srcdir@/Alcyone1D.check  Alcyone1D.check;             \
	fi

check-am:checkfiles

check_PROGRAMS = test1 test2 test3 test4 test5 test6
test1_SOURCES  = test1.cxx
test2_SOURCES  = test2.cxx
test3_SOURCES  = test3.cxx
test4_SOURCES  = test4.cxx
test5_SOURCES  = test5.cxx
test6_SOURCES  = test6.cxx

EXTRA_DIST = test1.check     \
	     test2.ref       \
	     test2.check     \
	     test3.ref       \
	     test3.check     \
	     test4.ref     \
	     test4.check     \
	     test5.ref     \
	     test5.check     \
	     test6.ref     \
	     test6.check     \
	     Alcyone1D.res   \
	     Alcyone1D.ref   \
	     Alcyone1D.check \
	     UPuN_GaseousFissionProductsReleaseModel.check \
	     UPuN_GaseousFissionProductsReleaseModel.txt   \
	     UPuN_GaseousFissionProductsReleaseModel.ref   \
	     CMakeLists.txt

TESTS = @top_builddir@/tfel-check/src/tfel-check

clean-local:
	$(RM) -f tfel-check.log *.checklog
	$(RM) -f *.xml *.out *.check *.ref
	$(RM) -f Alcyone1D.res test2.res test4.res
	$(RM) -f UPuN_GaseousFissionProductsReleaseModel.txt
