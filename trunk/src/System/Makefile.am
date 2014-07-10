## Makefile.am -- Process this file with automake to produce a Makefile.in file.
if TFEL_WIN
AM_CPPFLAGS = -DTFELSystem_EXPORTS
AM_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

INCLUDES = -I$(top_srcdir)/include -I$(srcdir)

lib_LTLIBRARIES = libTFELSystem.la 
libTFELSystem_la_SOURCES = ExternalLibraryManager.cxx \
			   System.cxx                 \
			   SystemError.cxx


if TFEL_WIN
libTFELSystem_la_SOURCES += getFunction-win.cxx
endif

if ! TFEL_WIN
libTFELSystem_la_SOURCES += RecursiveFind.cxx          \
			    ProcessManager-c.c         \
			    ProcessManager.cxx         \
			    ChildProcess.cxx           \
			    SignalHandler.cxx          \
			    SignalManager.cxx          \
			    basic_rstream.cxx          \
			    basic_wstream.cxx          \
			    rfstream.cxx               \
			    wfstream.cxx               \
			    BinaryRead.cxx             \
			    BinaryWrite.cxx            \
			    getFunction.c
endif

libTFELSystem_la_LIBADD = -L$(top_builddir)/src/Exception/ -lTFELException

if ! TFEL_WIN
libTFELSystem_la_LIBADD += -ldl
endif

EXTRA_DIST = CMakeLists.txt   \
	     getFunction.impl
