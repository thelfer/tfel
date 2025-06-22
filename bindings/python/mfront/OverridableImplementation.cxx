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

#include <boost/python.hpp>
#include "MFront/OverridableImplementation.hxx"

void declareOverridableImplementation() {
  auto write_ptr =
      static_cast<void (*)(const mfront::OverridableImplementation&,
                           const std::string&)>(mfront::write);

  boost::python::class_<mfront::OverridableImplementation>(
      "OverridableImplementation", boost::python::init<std::string>())
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
           "override a variable by the given parameter");

  boost::python::def("write", write_ptr, "export an implementation to a file");

}  // end of declareOverridableImplementation
