AM_CPPFLAGS = -I$(top_srcdir)/include        \
              -I$(top_srcdir)/mfront/include

bin_PROGRAMS = mfm
mfm_SOURCES  = mfm.cxx
mfm_LDADD    = -L$(top_builddir)/src/System -lTFELSystem       \
               -L$(top_builddir)/src/Utilities -lTFELUtilities

EXTRA_DIST=CMakeLists.txt
