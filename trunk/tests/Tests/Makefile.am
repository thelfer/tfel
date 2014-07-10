INCLUDES = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Tests
test_PROGRAMS = testcase	    \
		testsuite	    \
		testproxy	    \
		testproxy2	    \
		testmanager	    \
		testfunctionwrapper \
		stdstreamtestoutput \
		multipletestoutputs

testcase_SOURCES            = testcase.cxx           
testsuite_SOURCES           = testsuite.cxx          
testproxy_SOURCES           = testproxy.cxx          
testproxy2_SOURCES          = testproxy2.cxx         
testmanager_SOURCES         = testmanager.cxx        
testfunctionwrapper_SOURCES = testfunctionwrapper.cxx
stdstreamtestoutput_SOURCES = stdstreamtestoutput.cxx 
multipletestoutputs_SOURCES = multipletestoutputs.cxx

LDADD = -L$(top_builddir)/src/Tests -lTFELTests

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST = CMakeLists.txt