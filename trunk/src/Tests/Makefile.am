## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include
if TFEL_WIN
AM_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

lib_LTLIBRARIES = libTFELTests.la 
libTFELTests_la_SOURCES =   Test.cxx                 \
			    TestResult.cxx	     \
			    TestSuite.cxx	     \
			    TestOutput.cxx	     \
			    StdStreamTestOutput.cxx  \
			    XMLTestOutput.cxx        \
			    MultipleTestOutputs.cxx  \
			    TestManager.cxx	     \
			    TestCase.cxx
libTFELTests_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELTests_EXPORTS

EXTRA_DIST = CMakeLists.txt
