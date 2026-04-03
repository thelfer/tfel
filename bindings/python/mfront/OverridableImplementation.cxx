/*!
 * \file   bindings/python/mfront/OverridableImplementation.cxx
 * \brief
 * \author Thomas Helfer
 * \date   01/10/2020
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
      m, "OverridableImplementation")
      .def(pybind11::init<std::string>())
      .def(
          "overrideAuthor",
          +[](mfront::OverridableImplementation& i, const std::string& a) {
            i.setOverridenValue<mfront::OverridableImplementation::AUTHOR_NAME>(
                a);
          },
          "override the name of the author")
      .def(
          "overrideDate",
          +[](mfront::OverridableImplementation& i, const std::string& a) {
            i.setOverridenValue<mfront::OverridableImplementation::DATE>(a);
          },
          "override the date at which the implementation was written")
      .def(
          "overrideDescription",
          +[](mfront::OverridableImplementation& i, const std::string& a) {
            i.setOverridenValue<mfront::OverridableImplementation::DESCRIPTION>(
                a);
          },
          "override the description")
      .def(
          "overrideMaterial",
          +[](mfront::OverridableImplementation& i, const std::string& a) {
            i.setOverridenValue<
                mfront::OverridableImplementation::MATERIAL_NAME>(a);
          },
          "override the name of the material")
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
          },
          "override the name of the material property  (fails if the source "
          "file does not describe a material property)")
      .def(
          "overrideBehaviourName",
          +[](mfront::OverridableImplementation& i, const std::string& a) {
            if (i.getTargetType() != mfront::AbstractDSL::BEHAVIOURDSL) {
              tfel::raise(
                  "overrideBehaviourName: invalid call (the DSL does not "
                  "describe a behaviour)");
            }
            i.setOverridenValue<mfront::OverridableImplementation::
                                    MATERIAL_KNOWLEDGE_IDENTIFIER>(a);
          },
          "override the name of the behaviour  (fails if the source file does "
          "not describe a behaviour)")
      .def(
          "overrideModelName",
          +[](mfront::OverridableImplementation& i, const std::string& a) {
            if (i.getTargetType() != mfront::AbstractDSL::MODELDSL) {
              tfel::raise(
                  "overrideModelName: invalid call (the DSL does not describe "
                  "a model");
            }
            i.setOverridenValue<mfront::OverridableImplementation::
                                    MATERIAL_KNOWLEDGE_IDENTIFIER>(a);
          },
          "override the name of the model (fails if the source file does not "
          "describe a model)")
      .def("overrideByAParameter",
           &mfront::OverridableImplementation::overrideByAParameter,
           "override a variable by the given parameter")
      .def("getMaterialKnowledgeIdentifier",
           &mfront::OverridableImplementation::getMaterialKnowledgeIdentifier,
           "return the material knowledge identifier, i.e. the law name "
           "or the behaviour name or the model name")
      .def("getMaterial", &mfront::OverridableImplementation::getMaterial,
           "return the name of the material. This name can be inherited "
           "from the overriden implementation or overriden");

  m.def("getDestinationPathInMadnexFile",
        static_cast<std::string (*)(const mfront::OverridableImplementation&)>(
            mfront::getDestinationPathInMadnexFile),
        R"(
      return the path where an overridable implementation will be written in a madnex file.
      )");

  m.def("write", write_ptr,
        R"(
        export an overridable implementation to a file
        o: OverridableImplementation
        f: generated file
        )");
  m.def("write", write_ptr2,
        R"(
        export an overridable implementation to a file using a template file
        
        
        o: OverridableImplementation
        t: template file
        f: generated file
        )");

}  // end of declareOverridableImplementation
