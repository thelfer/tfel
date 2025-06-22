/*!
 * \file  bindings/python/tfel/ExternalLibraryManager.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 juil. 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

#include "TFEL/System/ExternalLibraryManager.hxx"

void declareExternalLibraryManager();

static void ELM_loadLibrary(tfel::system::ExternalLibraryManager& elm,
                            const std::string& n) {
  elm.loadLibrary(n);
}

static void ELM_setUShortParameter(tfel::system::ExternalLibraryManager& elm,
                                   const std::string& l,
                                   const std::string& n,
                                   const std::string& p,
                                   const unsigned short v) {
  elm.setParameter(l, n, p, v);
}

static void ELM_setIntParameter(tfel::system::ExternalLibraryManager& elm,
                                const std::string& l,
                                const std::string& n,
                                const std::string& p,
                                const int v) {
  elm.setParameter(l, n, p, v);
}

static void ELM_setDoubleParameter(tfel::system::ExternalLibraryManager& elm,
                                   const std::string& l,
                                   const std::string& n,
                                   const std::string& p,
                                   const double v) {
  elm.setParameter(l, n, p, v);
}

static void ELM_setUShortParameter2(tfel::system::ExternalLibraryManager& elm,
                                    const std::string& l,
                                    const std::string& n,
                                    const std::string& h,
                                    const std::string& p,
                                    const unsigned short v) {
  elm.setParameter(l, n, h, p, v);
}

static void ELM_setIntParameter2(tfel::system::ExternalLibraryManager& elm,
                                 const std::string& l,
                                 const std::string& n,
                                 const std::string& h,
                                 const std::string& p,
                                 const int v) {
  elm.setParameter(l, n, h, p, v);
}

static void ELM_setDoubleParameter2(tfel::system::ExternalLibraryManager& elm,
                                    const std::string& l,
                                    const std::string& n,
                                    const std::string& h,
                                    const std::string& p,
                                    const double v) {
  elm.setParameter(l, n, h, p, v);
}

void declareExternalLibraryManager() {
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::system;
  using ELM = tfel::system::ExternalLibraryManager;
  class_<ELM, noncopyable>("ExternalLibraryManager", no_init)
      .def("getExternalLibraryManager", ELM::getExternalLibraryManager,
           return_value_policy<reference_existing_object>())
      .staticmethod("getExternalLibraryManager")
      .def("loadLibrary", ELM_loadLibrary)
      .def("getSource", &ELM::getSource)
      .def("setUShortParameter", ELM_setUShortParameter)
      .def("setIntParameter", ELM_setIntParameter)
      .def("setDoubleParameter", ELM_setDoubleParameter)
      .def("setParameter", ELM_setDoubleParameter)
      .def("setUShortParameter", ELM_setUShortParameter2)
      .def("setIntParameter", ELM_setIntParameter2)
      .def("setDoubleParameter", ELM_setDoubleParameter2)
      .def("setParameter", ELM_setDoubleParameter2)
      .def("contains", &ELM::contains)
      .def("getUMATBehaviourType", &ELM::getUMATBehaviourType)
      .def("getUMATSymmetryType", &ELM::getUMATSymmetryType)
      .def("getUMATElasticSymmetryType", &ELM::getUMATElasticSymmetryType)
      .def("getUMATMaterialPropertiesNames",
           &ELM::getUMATMaterialPropertiesNames)
      .def("getUMATInternalStateVariablesNames",
           &ELM::getUMATInternalStateVariablesNames)
      .def("getUMATInternalStateVariablesTypes",
           &ELM::getUMATInternalStateVariablesTypes)
      .def("getUMATExternalStateVariablesNames",
           &ELM::getUMATExternalStateVariablesNames)
      .def("getUMATRequiresStiffnessTensor",
           &ELM::getUMATRequiresStiffnessTensor)
      .def("getUMATRequiresThermalExpansionCoefficientTensor",
           &ELM::getUMATRequiresThermalExpansionCoefficientTensor)
      .def("checkIfAsterBehaviourSaveTangentOperator",
           &ELM::checkIfAsterBehaviourSavesTangentOperator);

}  // end of declareExternalLibraryManager
