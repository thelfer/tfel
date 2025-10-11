/*!
 * \file   IsotropicModuli.cxx
 * \brief
 * \author Antoine MARTIN
 * \date   17/05/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Material/IsotropicModuli.hxx"

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static void declareKGModuli(pybind11::module_& m,
                                                    const char* const n) {
  using KG = tfel::material::KGModuli<StressType>;
  pybind11::class_<KG>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const KG&>())
      .def(pybind11::init<const StressType&, const StressType&>())
      .def("kappa",
           [](const KG& kg) {
             return kg.kappa;})
      .def("mu",
           [](const KG& kg) {
             return kg.mu;})
      .def("ToYoungNu", [](const KG& kg) { return kg.ToYoungNu(); })
      .def("ToLambdaMu", [](const KG& kg) { return kg.ToLambdaMu(); })
      .def("ToKG", [](const KG& kg) { return kg.ToKG(); });
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static void declareYoungNuModuli(pybind11::module_& m,
                                                         const char* const n) {
  using YN = tfel::material::YoungNuModuli<StressType>;
  pybind11::class_<YN>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const YN&>())
      .def(pybind11::init<const StressType&,
                          const tfel::types::real<StressType>&>())
      .def("young",
           [](const YN& yn) {
             return yn.young;
           })
      .def("nu",
           [](const YN& yn) {
             return yn.nu;
           })
      .def("ToYoungNu", [](const YN& yn) { return yn.ToYoungNu(); })
      .def("ToLambdaMu", [](const YN& yn) { return yn.ToLambdaMu(); })
      .def("ToKG", [](const YN& yn) { return yn.ToKG(); });
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static void declareLambdaMuModuli(pybind11::module_& m,
                                                          const char* const n) {
  using LM = tfel::material::LambdaMuModuli<StressType>;
  pybind11::class_<LM>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const LM&>())
      .def(pybind11::init<const StressType&, const StressType&>())
      .def("lambda",
           [](const LM& lm) {return lm.lambda;
           })
      .def("mu",
           [](const LM& lm) {return lm.mu;
           })
      .def("ToYoungNu", [](const LM& lm) { return lm.ToYoungNu(); })
      .def("ToLambdaMu", [](const LM& lm) { return lm.ToLambdaMu(); })
      .def("ToKG", [](const LM& lm) { return lm.ToKG(); });
}

void declareIsotropicModuli(pybind11::module_&);

void declareIsotropicModuli(pybind11::module_& m) {
  declareKGModuli<double>(m, "KGModuli");
  declareYoungNuModuli<double>(m, "YoungNuModuli");
  declareLambdaMuModuli<double>(m, "LambdaMuModuli");
}
