#!/bin/sh

if [ -f Makefile ];
then
    make distclean
fi

if [ -d build ];
then
    rm -fr build
fi

rm -fr config autom4te.cache
rm -f  libtool config.log config.status 
rm -f  m4/ltoptions.m4
rm -f  m4/lt~obsolete.m4
rm -f  m4/ltversion.m4
rm -f  m4/ltsugar.m4
rm -f  m4/libtool.m4
rm -f  configure aclocal.m4 
rm -f  $(find . -name "*~")
rm -f  $(find . -name "Makefile.in")
rm -f  $(find . -name "Makefile")
rm -f  Kernel/docs/Doxyfile
rm -fr Kernel/docs/html
rm -f  Mesh/src/pleiadesMesh.cxx
rm -fr ABOUT-NLS