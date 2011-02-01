if WITH_TESTS

INCLUDES = -I$(top_srcdir)/include

testdir = $(pkgdatadir)/tests/System

LDADD = -lTFELSystem -lTFELException    \
	-L$(top_builddir)/src/System    \
	-L$(top_builddir)/src/Exception

test_PROGRAMS   = process_test_target \
		  process             \
		  wstream             \
		  rstream

process_test_target_SOURCES = process_test_target.cxx
process_SOURCES             = process.cxx
rstream_SOURCES             = rstream.cxx
wstream_SOURCES             = wstream.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt