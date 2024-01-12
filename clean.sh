#!/bin/sh

if [ -d build ];
then
    rm -fr build
fi

rm -fr ABOUT-NLS
rm -f docs/mfront/Images/2norton-rk.aux
rm -f tools/glossary/glossary.xml
rm -f tools/glossary/Glossary.cxx
rm -f tools/glossary/PythonGlossary.cxx
rm -f tools/glossary/Glossary.hxx
rm -f tools/glossary/generate_glossary
rm -f mfront/tests/behaviours/castem/dgibi/*.trace

