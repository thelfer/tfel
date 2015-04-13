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
rm -f m4/lib-link.m4
rm -f m4/gettext.m4
rm -f m4/iconv.m4
rm -f m4/po.m4
rm -f  configure aclocal.m4 
rm -f  $(find . -name "*~")
rm -f  $(find . -name "Makefile.in")
rm -f  $(find . -name "Makefile")
rm -fr ABOUT-NLS
rm -f docs/mfront/Images/2norton-rk.aux
rm -f tools/glossary/glossary.xml
rm -f tools/glossary/Glossary.cxx
rm -f tools/glossary/PythonGlossary.cxx
rm -f tools/glossary/Glossary.hxx
rm -f tools/glossary/generate_glossary
rm -f mfront/tests/behaviours/castem/dgibi/*.trace

