noinst_LTLIBRARIES = libTFELPhysicalConstantsFortran90Bindings.la
libTFELPhysicalConstantsFortran90Bindings_la_SOURCES = tfel_physical_constants.f90
EXTRA_DIST=CMakeLists.txt

clean-local:
	-rm -f *.mod

install-exec-local: libTFELPhysicalConstantsFortran90Bindings.la
	-mkdir -p $(includedir)/TFEL
	-cp *.mod $(includedir)/TFEL
