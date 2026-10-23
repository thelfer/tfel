/*!
 * \file   bindings/python/mfront/OverridableImplementation.cxx
 * \brief
 * \author Thomas Helfer
 * \date 01/10/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/OverridableImplementation.hxx"

void declareOverridableImplementation(pybind11::module_&);

void declareOverridableImplementation(pybind11::module_& m) {
  auto write_ptr =
      static_cast<void (*)(const mfront::OverridableImplementation&,
                            const std::string&)>(mfront::write);
  auto write_ptr2 =
      static_cast<void (*)(const mfront::OverridableImplementation&,
                            const std::string&, const std::string&)>(
          mfront::write);

  pybind11::class_<mfront::OverridableImplementation>(
      m, "OverridableImplementation",
      "A data structure representing the implementation of a material\n"
      "knowledge based on MFront.\n"
      "\n"
      "This datastructure allows to take an MFront file, called the source file\n"
      "and override:\n"
      "\n"
      "- some metadata, such as the author name, the date, the description\n"
      "- the parameters\n"
      "- the bounds of the variables")
      .def(pybind11::init<std::string>(), pybind11::arg("f"))
      .def(
          "overrideAuthor",
          +[](mfront::OverridableImplementation& i, const std::string& a) {
            i.setOverridenValue<mfront::OverridableImplementation::AUTHOR_NAME>(
                a);
          },
          pybind11::arg("a"))
      .def(
          "overrideDate",
          +[](mfront::OverridableImplementation& i, const std::string& a) {
            i.setOverridenValue<mfront::OverridableImplementation::DATE>(a);
          })
      .def(
          "overrideDescription",
          +[](mfront::OverridableImplementation& i, const std::string& a) {
            i.setOverridenValue<mfront::OverridableImplementation::DESCRIPTION>(
                a);
          })
      .def(
          "overrideMaterial",
          +[](mfront::OverridableImplementation& i, const std::string& a) {
            i.setOverridenValue<
                mfront::OverridableImplementation::MATERIAL_NAME>(a);
          })
      .def(
          "overrideMaterialPropertyName",
          +[](mfront::OverridableImplementation& i, const std::string& a) {
            if (i.getTargetType() != mfront::AbstractDSL::MATERIALPROPERTYDSL) {
              tfel::raise(
                  "overrideMaterialPropertyName: invalid call (the DSL does "
                  "not describe a material property)");
            }
            i.setOverridenValue<mfront::OverridableImplementation::
                                    MATERIAL_KNOWLEDGE_IDENTIFIER>(a);
          })
      .def(
          "overrideBehaviourName",
          +[](mfront::OverridableImplementation& i, const std::string& a) {
            if (i.getTargetType() != mfront::AbstractDSL::BEHAVIOURDSL) {
              tfel::raise(
                  "overrideBehaviourName: invalid call (the DSL does "
                  "not describe a behaviour)");
            }
            i.setOverridenValue<mfront::OverridableImplementation::
                                    MATERIAL_KNOWLEDGE_IDENTIFIER>(a);
          })
      .def(
          "overrideModelName",
          +[](mfront::OverridableImplementation& i, const std::string& a) {
            if (i.getTargetType() != mfront::AbstractDSL::MODELDSL) {
              tfel::raise(
                  "overrideModelName: invalid call (the DSL does "
                  "not describe a model)");
            }
            i.setOverridenValue<mfront::OverridableImplementation::
                                    MATERIAL_KNOWLEDGE_IDENTIFIER>(a);
          })
      .def("overrideByAParameter",
           &mfront::OverridableImplementation::overrideByAParameter,
           pybind11::arg("n"), pybind11::arg("v"))
      .def("getMaterialKnowledgeIdentifier",
           &mfront::OverridableImplementation::getMaterialKnowledgeIdentifier)
      .def("getMaterial", &mfront::OverridableImplementation::getMaterial);

  m.def("getDestinationPathInMadnexFile",
        static_cast<std::string (*)(const mfront::OverridableImplementation&)>(
            mfront::getDestinationPathInMadnexFile));

  m.def("write", write_ptr, pybind11::arg("i"), pybind11::arg("f"));
  m.def("write", write_ptr2, pybind11::arg("i"), pybind11::arg("f"),
        pybind11::arg("d"));

}  // end of declareOverridableImplementation
