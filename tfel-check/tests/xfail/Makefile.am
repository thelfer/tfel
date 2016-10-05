checkfiles:
	if test "@top_srcdir@" != "@top_builddir@";                                 \
	then                                                                        \
		cp @srcdir@/test4.res    test4.res;                     \
		cp @srcdir@/test4.ref    test4.ref;                       \
		cp @srcdir@/test4.check  test4.check;                       \
	fi

check-am:checkfiles

EXTRA_DIST = test4.res      \
	     test4.ref      \
	     test4.check    \
	     CMakeLists.txt

TESTS       = @top_builddir@/tfel-check/src/tfel-check
XFAIL_TESTS = @top_builddir@/tfel-check/src/tfel-check


clean-local:
	$(RM) -f tfel-check.log test4.checklog TEST-test4.xml
	$(RM) -f test4.res test4.ref test4.check

