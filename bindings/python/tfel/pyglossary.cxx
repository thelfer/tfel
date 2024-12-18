
/*!
 * \file  bindings/python/tfel/glossary.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 31 mars 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>

void declareGlossaryEntry(pybind11::module_&);
void declareGlossary(pybind11::module_&);

PYBIND11_MODULE(glossary, m) {
  declareGlossaryEntry(m);
  declareGlossary(m);
}
