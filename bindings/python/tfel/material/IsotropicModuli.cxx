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
requires(
    tfel::math::checkUnitCompatibility<
        tfel::math::unit::Stress,
        StressType>()) static void declareIsotropicModuli(pybind11::module_& m,
                                                          const char* const n) {
  using IM = tfel::material::IsotropicModuli<StressType>;
  pybind11::class_<IM, std::shared_ptr<IM>>(m, n, pybind11::buffer_protocol());
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static void declareKGModuli(pybind11::module_& m,
                                                    const char* const n) {
  using KG = tfel::material::KGModuli<StressType>;
  using IM = tfel::material::IsotropicModuli<StressType>;

  pybind11::class_<KG, IM, std::shared_ptr<KG>>(m, n,
                                                pybind11::buffer_protocol())
      .def(pybind11::init<const KG&>())
      .def(pybind11::init<const StressType&, const StressType&>())
      .def_readwrite("kappa", &KG::kappa)
      .def_readwrite("mu", &KG::mu)
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
  using IM = tfel::material::IsotropicModuli<StressType>;

  pybind11::class_<YN, IM, std::shared_ptr<YN>>(m, n,
                                                pybind11::buffer_protocol())
      .def(pybind11::init<const YN&>())
      .def(pybind11::init<const StressType&,
                          const tfel::types::real<StressType>&>())
      .def_readwrite("young", &YN::young)
      .def_readwrite("nu", &YN::nu)
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
  using IM = tfel::material::IsotropicModuli<StressType>;

  pybind11::class_<LM, IM, std::shared_ptr<LM>>(m, n,
                                                pybind11::buffer_protocol())
      .def(pybind11::init<const LM&>())
      .def(pybind11::init<const StressType&, const StressType&>())
      .def_readwrite("lamb", &LM::lambda)
      .def_readwrite("mu", &LM::mu)
      .def("ToYoungNu", [](const LM& lm) { return lm.ToYoungNu(); })
      .def("ToLambdaMu", [](const LM& lm) { return lm.ToLambdaMu(); })
      .def("ToKG", [](const LM& lm) { return lm.ToKG(); });
}

void declareIsotropicModuli(pybind11::module_&);

void declareIsotropicModuli(pybind11::module_& m) {
  declareIsotropicModuli<double>(m, "IsotropicModuli");
  declareKGModuli<double>(m, "KGModuli");
  declareYoungNuModuli<double>(m, "YoungNuModuli");
  declareLambdaMuModuli<double>(m, "LambdaMuModuli");
  m.def("computeKGModuli", &tfel::material::computeKGModuli<double>);
  m.def("computeIsotropicStiffnessTensor", &tfel::material::computeIsotropicStiffnessTensor<double>);
}
