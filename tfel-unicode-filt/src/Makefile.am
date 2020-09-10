## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include

if TFEL_WIN
AM_LDFLAGS=-no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

bin_PROGRAMS = tfel-unicode-filt
tfel_unicode_filt_SOURCES = tfel-unicode-filt.cxx               
tfel_unicode_filt_LDADD = -L$(top_builddir)/src/UnicodeSupport \
			  -L$(top_builddir)/src/Utilities      \
			  -lTFELUnicodeSupport -lTFELUtilities
EXTRA_DIST = CMakeLists.txt
