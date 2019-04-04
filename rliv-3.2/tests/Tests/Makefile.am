AM_CPPFLAGS = -I$(top_srcdir)/include

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

clean-local:
	-$(RM) *.xml
	-$(RM) testmanager-3.txt
	-$(RM) testproxy.txt
	-$(RM) testmanager-2.txt
	-$(RM) stdstreamtestoutput1.txt
	-$(RM) testmanager-1.txt
	-$(RM) multipletestoutputs.txt
	-$(RM) testproxy2.txt
	-$(RM) stdstreamtestoutput3.txt
	-$(RM) stdstreamtestoutput2.txt
	-$(RM) testcase.txt

endif

EXTRA_DIST = CMakeLists.txt
