/*!
 * \file   LinearHomogenizationBounds.cxx
 * \brief
 * \author Antoine MARTIN
 * \date 14/05/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Material/LinearHomogenizationBounds.hxx"

template <tfel::math::ScalarConcept StressType, std::size_t N>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<N, StressType> computeVoigtStiffness(
        std::vector<tfel::types::real<StressType>>& tab_f,
        std::vector<tfel::math::st2tost2<N, StressType>>& tab_C) {
  return tfel::material::homogenization::elasticity::computeVoigtStiffness<
      N, StressType>(tab_f, tab_C);
}

template <tfel::math::ScalarConcept StressType, std::size_t N>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<N, StressType> computeReussStiffness(
        std::vector<tfel::types::real<StressType>>& tab_f,
        std::vector<tfel::math::st2tost2<N, StressType>>& tab_C) {
  return tfel::material::homogenization::elasticity::computeReussStiffness<
      N, StressType>(tab_f, tab_C);
}

template <tfel::math::ScalarConcept StressType, std::size_t N>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>())
    static std::pair<std::pair<StressType, StressType>,
                     std::pair<StressType, StressType>>
    computeIsotropicHashinShtrikmanBounds(
        std::vector<tfel::types::real<StressType>>& tab_f,
        std::vector<StressType>& tab_K,
        std::vector<StressType>& tab_mu) {
  return tfel::material::homogenization::elasticity::
      computeIsotropicHashinShtrikmanBounds<N, StressType>(tab_f, tab_K,
                                                           tab_mu);
}

void declareLinearHomogenizationBounds(pybind11::module_&);

static const char compute_voigt_stiffness_doc[] =
    "This function gives the Voigt stiffness\n"
    "Return an object of type st2tost2<d,StressType>\n"
    "tab_f: std::span<real> of volumic fractions of phases\n"
    "tab_C: std::span<st2tost2<d,StressType>> of stiffness tensors of\n"
    "phases";

static const char compute_reuss_stiffness_doc[] =
    "This function gives the Reuss stiffness\n"
    "Return an object of type st2tost2<d,StressType>\n"
    "tab_f: std::span<real> of volumic fractions of phases\n"
    "tab_C: std::span<st2tost2<d,StressType>> of stiffness tensors of\n"
    "phases";

static const char compute_hashin_shtrikman_bounds_doc[] =
    "This function gives the Hashin-Shtrikman bounds for a\n"
    "d-dimensional isotropic composite composed of N isotropic phases.\n"
    "The formulas can be found in Torquato (2002).\n"
    "Return a\n"
    "std::pair<std::pair<StressType,StressType>,std::pair<StressType,StressType>>.\n"
    "First element of the pair is the lower bound and second element is\n"
    "the upper bound. Each bound is a std::pair with bulk and shear\n"
    "moduli.\n"
    "tab_f: std::span<real> of volumic fractions of phases\n"
    "tab_K: std::span<StressType> of bulk moduli of phases\n"
    "tab_mu: std::span<StressType> of shear moduli of phases";

void declareLinearHomogenizationBounds(pybind11::module_& m) {
  m.def("computeVoigtStiffness3D",
        &computeVoigtStiffness<double, 3>,
        pybind11::arg("tab_f"), pybind11::arg("tab_C"),
        compute_voigt_stiffness_doc);
  m.def("computeReussStiffness3D",
        &computeReussStiffness<double, 3>,
        pybind11::arg("tab_f"), pybind11::arg("tab_C"),
        compute_reuss_stiffness_doc);
  m.def("computeIsotropicHashinShtrikmanBounds3D",
        &computeIsotropicHashinShtrikmanBounds<double, 3>,
        pybind11::arg("tab_f"), pybind11::arg("tab_K"),
        pybind11::arg("tab_mu"), compute_hashin_shtrikman_bounds_doc);
  m.def("computeVoigtStiffness2D",
        &computeVoigtStiffness<double, 2>,
        pybind11::arg("tab_f"), pybind11::arg("tab_C"),
        compute_voigt_stiffness_doc);
  m.def("computeReussStiffness2D",
        &computeReussStiffness<double, 2>,
        pybind11::arg("tab_f"), pybind11::arg("tab_C"),
        compute_reuss_stiffness_doc);
  m.def("computeIsotropicHashinShtrikmanBounds2D",
        &computeIsotropicHashinShtrikmanBounds<double, 2>,
        pybind11::arg("tab_f"), pybind11::arg("tab_K"),
        pybind11::arg("tab_mu"), compute_hashin_shtrikman_bounds_doc);
}
