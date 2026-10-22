/*!
 * \file   Barlat.cxx
 * \brief
 * \author Thomas Helfer
 * \date 19/12/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Material/Barlat2004YieldCriterion.hxx"

template <unsigned short N>
static tfel::math::st2tost2<N, double> makeBarlatLinearTransformation(
    const double c12,
    const double c21,
    const double c13,
    const double c31,
    const double c23,
    const double c32,
    const double c44,
    const double c55,
    const double c66) {
  return tfel::material::makeBarlatLinearTransformation<N>(
      c12, c21, c13, c31, c23, c32, c44, c55, c66);
}

template <unsigned short N>
static double computeBarlatStress(const tfel::math::stensor<N, double>& s,
                                  const tfel::math::st2tost2<N, double>& l1,
                                  const tfel::math::st2tost2<N, double>& l2,
                                  const double a,
                                  const double e) {
  return tfel::material::computeBarlatStress(s, l1, l2, a, e);
}  // end of computeBarlatStress

void declareBarlat(pybind11::module_&);

static const char make_barlat_linear_transformation_doc[] =
    "Return a linear transformation of the stresses\n"
    "c12: coefficient of the linear transformation\n"
    "c21: coefficient of the linear transformation\n"
    "c13: coefficient of the linear transformation\n"
    "c31: coefficient of the linear transformation\n"
    "c23: coefficient of the linear transformation\n"
    "c32: coefficient of the linear transformation\n"
    "c44: coefficient of the linear transformation\n"
    "c55: coefficient of the linear transformation\n"
    "c66: coefficient of the linear transformation\n"
    "The linear transformation is defined as follows:\n"
    "Note: Barlat uses the following convention for storing symmetric\n"
    "tensors:\n"
    "xx yy zz yz zx xy\n"
    "which is not consistent with the TFEL/Cast3M/Abaqus/Ansys\n"
    "conventions:\n"
    "xx yy zz xy xz yz\n"
    "Therefore, if one wants to uses coeficients c^B given by Barlat,\n"
    "one shall call this function as follows:\n"
    "const auto l1 =\n"
    "makeBarlatLinearTransformation<3>(cB_12,cB_21,cB_13,cB_31,\n"
    "cB_23,cB_32,cB_66,cBB_55,cBB_44);";

static const char compute_barlat_stress_doc[] =
    "This function computes the Barlat yield stress.\n"
    "s: stress tensor\n"
    "l1: first linear transformation\n"
    "l2: second linear transformation\n"
    "a: Barlat exponent\n"
    "e: criterion used to check if the von Mises stress is null\n"
    "Return the Barlat yield stress\n"
    "See `makeBarlatLinearTransformation`";

void declareBarlat(pybind11::module_& m) {
  m.def("makeBarlatLinearTransformation1D",
        &makeBarlatLinearTransformation<1u>,
        pybind11::arg("c12"), pybind11::arg("c21"), pybind11::arg("c13"),
        pybind11::arg("c31"), pybind11::arg("c23"), pybind11::arg("c32"),
        pybind11::arg("c44"), pybind11::arg("c55"), pybind11::arg("c66"),
        make_barlat_linear_transformation_doc);
  m.def("makeBarlatLinearTransformation2D",
        &makeBarlatLinearTransformation<2u>,
        pybind11::arg("c12"), pybind11::arg("c21"), pybind11::arg("c13"),
        pybind11::arg("c31"), pybind11::arg("c23"), pybind11::arg("c32"),
        pybind11::arg("c44"), pybind11::arg("c55"), pybind11::arg("c66"),
        make_barlat_linear_transformation_doc);
  m.def("makeBarlatLinearTransformation3D",
        &makeBarlatLinearTransformation<3u>,
        pybind11::arg("c12"), pybind11::arg("c21"), pybind11::arg("c13"),
        pybind11::arg("c31"), pybind11::arg("c23"), pybind11::arg("c32"),
        pybind11::arg("c44"), pybind11::arg("c55"), pybind11::arg("c66"),
        make_barlat_linear_transformation_doc);
  m.def("computeBarlatStress",
        &computeBarlatStress<1u>,
        pybind11::arg("s"), pybind11::arg("l1"), pybind11::arg("l2"),
        pybind11::arg("a"), pybind11::arg("e"), compute_barlat_stress_doc);
  m.def("computeBarlatStress",
        &computeBarlatStress<2u>,
        pybind11::arg("s"), pybind11::arg("l1"), pybind11::arg("l2"),
        pybind11::arg("a"), pybind11::arg("e"), compute_barlat_stress_doc);
  m.def("computeBarlatStress",
        &computeBarlatStress<3u>,
        pybind11::arg("s"), pybind11::arg("l1"), pybind11::arg("l2"),
        pybind11::arg("a"), pybind11::arg("e"), compute_barlat_stress_doc);
}
