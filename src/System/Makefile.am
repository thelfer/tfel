## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(srcdir) $(TFEL_THREAD_FLAGS)
if TFEL_WIN
AM_LDFLAGS   = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

lib_LTLIBRARIES = libTFELSystem.la 
libTFELSystem_la_SOURCES  = LibraryInformation.cxx           \
			    ExternalLibraryManager.cxx       \
			    ExternalBehaviourDescription.cxx \
			    System.cxx                       \
			    SystemError.cxx                  \
			    ThreadPool.cxx                   \
		  	    ThreadedTaskResult.cxx

libTFELSystem_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELSystem_EXPORTS

if TFEL_WIN
libTFELSystem_la_SOURCES += getFunction-win.cxx
endif

if ! TFEL_WIN
libTFELSystem_la_SOURCES += RecursiveFind.cxx      \
			    ProcessManager-c.c     \
			    ProcessManager.cxx     \
			    ChildProcess.cxx       \
			    SignalHandler.cxx      \
			    SignalManager.cxx      \
			    basic_rstream.cxx      \
			    basic_wstream.cxx      \
			    rfstream.cxx           \
			    wfstream.cxx           \
			    BinaryRead.cxx         \
			    BinaryWrite.cxx        \
			    getFunction.c
endif

libTFELSystem_la_LIBADD = -L$(top_builddir)/src/Exception/ -lTFELException $(TFEL_THREAD_FLAGS) $(TFEL_THREAD_LIBS)

if TFEL_LINUX
libTFELSystem_la_LIBADD += -ldl
endif

EXTRA_DIST = CMakeLists.txt                       \
	     getFunction.impl                     \
             LibraryInformationImplementation.cxx \
	     LibraryInformation-elf_info.cxx      \
             LibraryInformation-macho_info.cxx    \
             LibraryInformation-pe_info.cxx
