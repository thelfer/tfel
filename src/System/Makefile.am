## Makefile.am -- Process this file with automake to produce a Makefile.in file.

INCLUDES = -I$(top_srcdir)/include

lib_LTLIBRARIES = libTFELSystem.la 
libTFELSystem_la_SOURCES = ExternalLibraryManager.cxx \
			   ProcessManager-c.c         \
			   ProcessManager.cxx         \
			   ChildProcess.cxx           \
			   SignalHandler.cxx          \
			   SignalManager.cxx          \
			   System.cxx                 \
			   SystemError.cxx            \
			   basic_rstream.cxx          \
			   basic_wstream.cxx          \
			   rfstream.cxx               \
			   wfstream.cxx               \
			   getFunction.c

libTFELSystem_la_LIBADD = -L$(top_builddir)/src/Exception/ -lTFELException -ldl