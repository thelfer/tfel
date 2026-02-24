
/*!
 * \file  bindings/python/tfel/glossary.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 31 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include "TFEL/Macros.hxx"

#define TFEL_GLOSSARY_MODULE_NAME \
  TFEL_PP_JOIN(glossary_, TFEL_SUFFIX_FOR_PYTHON_MODULES)

void declareGlossaryEntry(pybind11::module_&);
void declareGlossary(pybind11::module_&);

#ifdef TFEL_SUFFIX_FOR_PYTHON_MODULES
PYBIND11_MODULE(TFEL_GLOSSARY_MODULE_NAME, m) {
#else
PYBIND11_MODULE(glossary, m) {
#endif
  declareGlossaryEntry(m);
  declareGlossary(m);
}
