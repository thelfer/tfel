if TFEL_PYTHON_BINDINGS
SUBDIRS = python
endif

if TFEL_FORTRAN_BINDINGS
if TFEL_PYTHON_BINDINGS
SUBDIRS += fortran77 fortran90
else
SUBDIRS  = fortran77 fortran90
endif
endif

EXTRA_DIST=CMakeLists.txt
