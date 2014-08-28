/*! 
 * \file  bindings/python/tfel/glossary.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 31 mars 2014
 */

#include<boost/python.hpp>

void
declareGlossaryEntry(void);

void
declareGlossary(void);

BOOST_PYTHON_MODULE(glossary)
{
  declareGlossaryEntry();
  declareGlossary();
}

