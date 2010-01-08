if WITH_TESTS

INCLUDES = -I$(top_srcdir)/include

testdir = $(pkgdatadir)/tests/System

LDADD = -lTFELSystem -lTFELException    \
	-L$(top_builddir)/src/System    \
	-L$(top_builddir)/src/Exception

test_PROGRAMS   = test    \
		  process \
		  wstream \
		  rstream

test_SOURCES    = test.cxx
process_SOURCES = process.cxx
rstream_SOURCES = rstream.cxx
wstream_SOURCES = wstream.cxx

TESTS=$(test_PROGRAMS)

endif
