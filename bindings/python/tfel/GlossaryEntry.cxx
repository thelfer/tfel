/*!
 * \file  bindings/python/tfel/GlossaryEntry.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 31 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

#include "TFEL/Glossary/GlossaryEntry.hxx"

void declareGlossaryEntry();

void declareGlossaryEntry() {
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::glossary;
  class_<GlossaryEntry, noncopyable>("GlossaryEntry", no_init)
      .add_property("key",
                    make_function(&GlossaryEntry::getKey,
                                  return_value_policy<copy_const_reference>()))
      .add_property("names",
                    make_function(&GlossaryEntry::getNames,
                                  return_value_policy<copy_const_reference>()))
      .add_property("unit",
                    make_function(&GlossaryEntry::getUnit,
                                  return_value_policy<copy_const_reference>()))
      .add_property("type",
                    make_function(&GlossaryEntry::getType,
                                  return_value_policy<copy_const_reference>()))
      .add_property("short_description",
                    make_function(&GlossaryEntry::getShortDescription,
                                  return_value_policy<copy_const_reference>()))
      .add_property("description",
                    make_function(&GlossaryEntry::getDescription,
                                  return_value_policy<copy_const_reference>()))
      .add_property("notes",
                    make_function(&GlossaryEntry::getNotes,
                                  return_value_policy<copy_const_reference>()));

}  // end of declareGlossaryEntry
