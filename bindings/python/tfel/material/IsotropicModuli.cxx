/*!
 * \file   IsotropicModuli.cxx
 * \brief
 * \author Antoine MARTIN
 * \date 17/05/2025
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
  pybind11::class_<IM, std::shared_ptr<IM>>(
      m, n, pybind11::buffer_protocol(),
      "This class is an abstract class which represents isotropic moduli.\n"
      "3 formats are possible: (Young,Nu), (Lambda,Mu) and (K,Mu)");
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static auto declareKGModuli(pybind11::module_& m,
                                                    const char* const n) {
  using KG = tfel::material::KGModuli<StressType>;
  using IM = tfel::material::IsotropicModuli<StressType>;

  pybind11::class_<KG, IM, std::shared_ptr<KG>> c(
      m, n, pybind11::buffer_protocol(),
      "This class is relative to the format (K,Mu).\n"
      "It can be converted to the other formats (Lambda,Mu) and (Young,Nu)");
  c.def(pybind11::init<const KG&>())
      .def(pybind11::init<const StressType&, const StressType&>(),
           pybind11::arg("Kappa"), pybind11::arg("Mu"))
      .def_readwrite("kappa", &KG::kappa)
      .def_readwrite("mu", &KG::mu);
  return c;
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static auto declareYoungNuModuli(pybind11::module_& m,
                                                         const char* const n) {
  using YN = tfel::material::YoungNuModuli<StressType>;
  using IM = tfel::material::IsotropicModuli<StressType>;

  pybind11::class_<YN, IM, std::shared_ptr<YN>> c(
      m, n, pybind11::buffer_protocol(),
      "This class is relative to the format (Young,Nu).\n"
      "It can be converted to the other formats (Lambda,Mu) and (K,Mu)");
  c.def(pybind11::init<const YN&>())
      .def(pybind11::init<const StressType&,
                          const tfel::types::real<StressType>&>(),
           pybind11::arg("Young"), pybind11::arg("Nu"))
      .def_readwrite("young", &YN::young)
      .def_readwrite("nu", &YN::nu);
  return c;
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static auto declareLambdaMuModuli(pybind11::module_& m,
                                                          const char* const n) {
  using LM = tfel::material::LambdaMuModuli<StressType>;
  using IM = tfel::material::IsotropicModuli<StressType>;

  pybind11::class_<LM, IM, std::shared_ptr<LM>> c(
      m, n, pybind11::buffer_protocol(),
      "This class is relative to the format (Lambda,Mu).\n"
      "It can be converted to the other formats (Young,Nu) and (K,Mu)");
  c.def(pybind11::init<const LM&>())
      .def(pybind11::init<const StressType&, const StressType&>(),
           pybind11::arg("Lambda"), pybind11::arg("Mu"))
      .def_readwrite("lamb", &LM::lambda)
      .def_readwrite("mu", &LM::mu);
  return c;
}

/*!
 * \brief declare the conversions to the three formats
 *
 * The conversions are declared once the classes of the three formats are
 * registered, so that their signatures refer to the python names of these
 * classes.
 */
template <typename Moduli, typename... Options>
static void declareConversions(pybind11::class_<Moduli, Options...>& c) {
  c.def("ToYoungNu", [](const Moduli& im) { return im.ToYoungNu(); })
      .def("ToLambdaMu", [](const Moduli& im) { return im.ToLambdaMu(); })
      .def("ToKG", [](const Moduli& im) { return im.ToKG(); });
}

void declareIsotropicModuli(pybind11::module_&);

void declareIsotropicModuli(pybind11::module_& m) {
  declareIsotropicModuli<double>(m, "IsotropicModuli");
  auto kg = declareKGModuli<double>(m, "KGModuli");
  auto yn = declareYoungNuModuli<double>(m, "YoungNuModuli");
  auto lm = declareLambdaMuModuli<double>(m, "LambdaMuModuli");
  declareConversions(kg);
  declareConversions(yn);
  declareConversions(lm);
  m.def("computeKGModuli", &tfel::material::computeKGModuli<double>,
        pybind11::arg("A"),
        "This function returns the isotropized moduli of a st2tost2\n"
        "It uses computeKappaMu\n"
        "The implementation goes for dimension 3 only.\n"
        "A: `st2tost2`\n"
        "Return a KGModuli");
  m.def("computeIsotropicStiffnessTensor",
        &tfel::material::computeIsotropicStiffnessTensor<double>,
        pybind11::arg("IM"),
        "This function takes a IsotropicModuli and\n"
        "returns the corresponding st2tost2\n"
        "IM: `IsotropicModuli`\n"
        "Return a `st2tost2`");
}
