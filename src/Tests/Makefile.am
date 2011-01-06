## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include

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
EXTRA_DIST = CMakeLists.txt