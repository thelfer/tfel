/*!
 * \file   include/TFEL/Material/OrthotropicPlasticity.ixx
 * \brief This files implements the function declared in
 * `include/TFEL/Material/OrthotropicPlasticity.ixx`.
 * \author Thomas Helfer
 * \date   14/01/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ORTHOTROPICPLASTICITY_IXX
#define LIB_TFEL_MATERIAL_ORTHOTROPICPLASTICITY_IXX

namespace tfel::material {

  template <tfel::math::StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto computeJ2O(
      const StensorType& s, const J2OCoefficients<StensorType>& a) {
    return computeJ2O(s, a[0], a[1], a[2], a[3], a[4], a[5]);
  }

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 1u)  //
      TFEL_HOST_DEVICE
      constexpr auto computeJ2O(const StensorType& s,
                                const tfel::math::base_type<StensorType> a1,
                                const tfel::math::base_type<StensorType> a2,
                                const tfel::math::base_type<StensorType> a3,
                                const tfel::math::base_type<StensorType>,
                                const tfel::math::base_type<StensorType>,
                                const tfel::math::base_type<StensorType>) {
    return (a2 * (s[1] - s[2]) * (s[1] - s[2]) +
            a3 * (s[0] - s[2]) * (s[0] - s[2]) +
            a1 * (s[0] - s[1]) * (s[0] - s[1])) /
           6;
  }  // end of computeJ2O

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 2u)  //
      TFEL_HOST_DEVICE
      constexpr auto computeJ2O(const StensorType& s,
                                const tfel::math::base_type<StensorType> a1,
                                const tfel::math::base_type<StensorType> a2,
                                const tfel::math::base_type<StensorType> a3,
                                const tfel::math::base_type<StensorType> a4,
                                const tfel::math::base_type<StensorType>,
                                const tfel::math::base_type<StensorType>) {
    return (a2 * (s[1] - s[2]) * (s[1] - s[2]) +
            a3 * (s[0] - s[2]) * (s[0] - s[2]) +
            a1 * (s[0] - s[1]) * (s[0] - s[1])) /
               6 +
           a4 * s[3] * s[3] / 2;
  }  // end of computeJ2O

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 3u)  //
      TFEL_HOST_DEVICE
      constexpr auto computeJ2O(const StensorType& s,
                                const tfel::math::base_type<StensorType> a1,
                                const tfel::math::base_type<StensorType> a2,
                                const tfel::math::base_type<StensorType> a3,
                                const tfel::math::base_type<StensorType> a4,
                                const tfel::math::base_type<StensorType> a5,
                                const tfel::math::base_type<StensorType> a6) {
    return (a6 * s[5] * s[5] + a5 * s[4] * s[4] + a4 * s[3] * s[3]) / 2 +
           (a2 * (s[1] - s[2]) * (s[1] - s[2]) +
            a3 * (s[0] - s[2]) * (s[0] - s[2]) +
            a1 * (s[0] - s[1]) * (s[0] - s[1])) /
               6;
  }  // end of computeJ2O

  template <tfel::math::StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto computeJ2ODerivative(
      const StensorType& s, const J2OCoefficients<StensorType>& a) {
    return computeJ2ODerivative(s, a[0], a[1], a[2], a[3], a[4], a[5]);
  }  // end of computeJ2ODerivative

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 1u)  //
      TFEL_HOST_DEVICE constexpr auto computeJ2ODerivative(
          const StensorType& s,
          const tfel::math::base_type<StensorType> a1,
          const tfel::math::base_type<StensorType> a2,
          const tfel::math::base_type<StensorType> a3,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>) {
    using Result =
        tfel::math::stensor<tfel::math::getSpaceDimension<StensorType>(),
                            tfel::math::numeric_type<StensorType>>;
    return Result{(a3 * (s[0] - s[2])) / 3 + (a1 * (s[0] - s[1])) / 3,
                  (a2 * (s[1] - s[2])) / 3 - (a1 * (s[0] - s[1])) / 3,
                  (-(a2 * (s[1] - s[2])) / 3) - (a3 * (s[0] - s[2])) / 3};
  }  // end of computeJ2ODerivative

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 2u)  //
      TFEL_HOST_DEVICE constexpr auto computeJ2ODerivative(
          const StensorType& s,
          const tfel::math::base_type<StensorType> a1,
          const tfel::math::base_type<StensorType> a2,
          const tfel::math::base_type<StensorType> a3,
          const tfel::math::base_type<StensorType> a4,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>) {
    using Result =
        tfel::math::stensor<tfel::math::getSpaceDimension<StensorType>(),
                            tfel::math::numeric_type<StensorType>>;
    return Result{(a3 * (s[0] - s[2])) / 3 + (a1 * (s[0] - s[1])) / 3,
                  (a2 * (s[1] - s[2])) / 3 - (a1 * (s[0] - s[1])) / 3,
                  (-(a2 * (s[1] - s[2])) / 3) - (a3 * (s[0] - s[2])) / 3,
                  a4 * s[3]};
  }  // end of computeJ2ODerivative

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 3u)  //
      TFEL_HOST_DEVICE constexpr auto computeJ2ODerivative(
          const StensorType& s,
          const tfel::math::base_type<StensorType> a1,
          const tfel::math::base_type<StensorType> a2,
          const tfel::math::base_type<StensorType> a3,
          const tfel::math::base_type<StensorType> a4,
          const tfel::math::base_type<StensorType> a5,
          const tfel::math::base_type<StensorType> a6) {
    using Result =
        tfel::math::stensor<tfel::math::getSpaceDimension<StensorType>(),
                            tfel::math::numeric_type<StensorType>>;
    return Result{(a3 * (s[0] - s[2])) / 3 + (a1 * (s[0] - s[1])) / 3,
                  (a2 * (s[1] - s[2])) / 3 - (a1 * (s[0] - s[1])) / 3,
                  (-(a2 * (s[1] - s[2])) / 3) - (a3 * (s[0] - s[2])) / 3,
                  a4 * s[3],
                  a5 * s[4],
                  a6 * s[5]};
  }  // end of computeJ2ODerivative

  template <tfel::math::StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto computeJ2OSecondDerivative(
      const StensorType& s, const J2OCoefficients<StensorType>& a) {
    return computeJ2OSecondDerivative(s, a[0], a[1], a[2], a[3], a[4], a[5]);
  }  // end of computeJ2OSecondDerivative

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 1u)  //
      TFEL_HOST_DEVICE constexpr auto computeJ2OSecondDerivative(
          const StensorType&,
          const tfel::math::base_type<StensorType> a1,
          const tfel::math::base_type<StensorType> a2,
          const tfel::math::base_type<StensorType> a3,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>) {
    using Result = tfel::math::st2tost2<
        tfel::math::getSpaceDimension<StensorType>(),
        tfel::math::BinaryOperationResult<tfel::math::base_type<StensorType>,
                                          tfel::math::numeric_type<StensorType>,
                                          tfel::math::OpDiv>>;
    return Result{(a3 + a1) / 3, -a1 / 3, -a3 / 3, -a1 / 3,      (a2 + a1) / 3,
                  -a2 / 3,       -a3 / 3, -a2 / 3, (a2 + a3) / 3};
  }  // end of computeJ2OSecondDerivative

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 2u)  //
      TFEL_HOST_DEVICE constexpr auto computeJ2OSecondDerivative(
          const StensorType&,
          const tfel::math::base_type<StensorType> a1,
          const tfel::math::base_type<StensorType> a2,
          const tfel::math::base_type<StensorType> a3,
          const tfel::math::base_type<StensorType> a4,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>) {
    using Result = tfel::math::st2tost2<
        tfel::math::getSpaceDimension<StensorType>(),
        tfel::math::BinaryOperationResult<tfel::math::base_type<StensorType>,
                                          tfel::math::numeric_type<StensorType>,
                                          tfel::math::OpDiv>>;
    using real = tfel::math::base_type<StensorType>;
    constexpr auto zero = real{0};
    return Result{(a3 + a1) / 3, -a1 / 3,       -a3 / 3,       zero,
                  -a1 / 3,       (a2 + a1) / 3, -a2 / 3,       zero,
                  -a3 / 3,       -a2 / 3,       (a2 + a3) / 3, zero,
                  zero,          zero,          zero,          a4};
  }  // end of computeJ2OSecondDerivative

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 3u)  //
      TFEL_HOST_DEVICE constexpr auto computeJ2OSecondDerivative(
          const StensorType&,
          const tfel::math::base_type<StensorType> a1,
          const tfel::math::base_type<StensorType> a2,
          const tfel::math::base_type<StensorType> a3,
          const tfel::math::base_type<StensorType> a4,
          const tfel::math::base_type<StensorType> a5,
          const tfel::math::base_type<StensorType> a6) {
    using Result = tfel::math::st2tost2<
        tfel::math::getSpaceDimension<StensorType>(),
        tfel::math::BinaryOperationResult<tfel::math::base_type<StensorType>,
                                          tfel::math::numeric_type<StensorType>,
                                          tfel::math::OpDiv>>;
    using real = tfel::math::base_type<StensorType>;
    constexpr auto zero = real{0};
    return Result{(a3 + a1) / 3, -a1 / 3,       -a3 / 3,       zero, zero, zero,
                  -a1 / 3,       (a2 + a1) / 3, -a2 / 3,       zero, zero, zero,
                  -a3 / 3,       -a2 / 3,       (a2 + a3) / 3, zero, zero, zero,
                  zero,          zero,          zero,          a4,   zero, zero,
                  zero,          zero,          zero,          zero, a5,   zero,
                  zero,          zero,          zero,          zero, zero, a6};
  }  // end of computeJ2OSecondDerivative

  template <tfel::math::StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto computeJ3O(
      const StensorType& s, const J3OCoefficients<StensorType>& b) {
    return computeJ3O(s, b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8],
                      b[9], b[10]);
  }

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 1u)  //
      TFEL_HOST_DEVICE
      constexpr auto computeJ3O(const StensorType& s,
                                const tfel::math::base_type<StensorType> b1,
                                const tfel::math::base_type<StensorType> b2,
                                const tfel::math::base_type<StensorType> b3,
                                const tfel::math::base_type<StensorType> b4,
                                const tfel::math::base_type<StensorType>,
                                const tfel::math::base_type<StensorType>,
                                const tfel::math::base_type<StensorType>,
                                const tfel::math::base_type<StensorType>,
                                const tfel::math::base_type<StensorType>,
                                const tfel::math::base_type<StensorType>,
                                const tfel::math::base_type<StensorType>) {
    return ((2 * (b4 + b1) - b3 - b2) * s[2] * s[2] * s[2]) / 27 +
           ((b4 + b3) * s[1] * s[1] * s[1]) / 27 +
           ((b2 + b1) * s[0] * s[0] * s[0]) / 27 -
           (((b4 + b1 - b3) * s[1] + (b4 - b2 + b1) * s[0]) * s[2] * s[2]) / 9 -
           (s[1] * s[1] * (b3 * s[2] + b4 * s[0])) / 9 +
           (2 * (b4 + b1) * s[0] * s[1] * s[2]) / 9 -
           (s[0] * s[0] * (b1 * s[1] + b2 * s[2])) / 9;
  }

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 2u)  //
      TFEL_HOST_DEVICE
      constexpr auto computeJ3O(const StensorType& s,
                                const tfel::math::base_type<StensorType> b1,
                                const tfel::math::base_type<StensorType> b2,
                                const tfel::math::base_type<StensorType> b3,
                                const tfel::math::base_type<StensorType> b4,
                                const tfel::math::base_type<StensorType> b5,
                                const tfel::math::base_type<StensorType>,
                                const tfel::math::base_type<StensorType>,
                                const tfel::math::base_type<StensorType>,
                                const tfel::math::base_type<StensorType>,
                                const tfel::math::base_type<StensorType> b10,
                                const tfel::math::base_type<StensorType>) {
    return ((2 * (b4 + b1) - b3 - b2) * s[2] * s[2] * s[2]) / 27 +
           ((b4 + b3) * s[1] * s[1] * s[1]) / 27 +
           ((b2 + b1) * s[0] * s[0] * s[0]) / 27 -
           ((2 * b10 * s[2] - b5 * s[1] + (b5 - 2 * b10) * s[0]) * s[3] *
            s[3]) /
               6 -
           (((b4 + b1 - b3) * s[1] + (b4 - b2 + b1) * s[0]) * s[2] * s[2]) / 9 -
           (s[1] * s[1] * (b3 * s[2] + b4 * s[0])) / 9 +
           (2 * (b4 + b1) * s[0] * s[1] * s[2]) / 9 -
           (s[0] * s[0] * (b1 * s[1] + b2 * s[2])) / 9;
  }

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 3u)  //
      TFEL_HOST_DEVICE
      constexpr auto computeJ3O(const StensorType& s,
                                const tfel::math::base_type<StensorType> b1,
                                const tfel::math::base_type<StensorType> b2,
                                const tfel::math::base_type<StensorType> b3,
                                const tfel::math::base_type<StensorType> b4,
                                const tfel::math::base_type<StensorType> b5,
                                const tfel::math::base_type<StensorType> b6,
                                const tfel::math::base_type<StensorType> b7,
                                const tfel::math::base_type<StensorType> b8,
                                const tfel::math::base_type<StensorType> b9,
                                const tfel::math::base_type<StensorType> b10,
                                const tfel::math::base_type<StensorType> b11) {
    using real = tfel::math::base_type<StensorType>;
    constexpr const auto icste = tfel::math::Cste<real>::isqrt2;
    return ((2 * (b4 + b1) - b3 - b2) * s[2] * s[2] * s[2]) / 27 +
           ((b4 + b3) * s[1] * s[1] * s[1]) / 27 +
           ((b2 + b1) * s[0] * s[0] * s[0]) / 27 +
           (-((-b7 * s[2] - b6 * s[1] + (b7 + b6) * s[0]) * s[5] * s[5]) / 6) -
           (((-b8 * s[2]) + 2 * b9 * s[1] + (b8 - 2 * b9) * s[0]) * s[4] *
            s[4]) /
               6 -
           ((2 * b10 * s[2] - b5 * s[1] + (b5 - 2 * b10) * s[0]) * s[3] *
            s[3]) /
               6 -
           (((b4 + b1 - b3) * s[1] + (b4 - b2 + b1) * s[0]) * s[2] * s[2]) / 9 -
           (s[1] * s[1] * (b3 * s[2] + b4 * s[0])) / 9 +
           (2 * (b4 + b1) * s[0] * s[1] * s[2]) / 9 -
           (s[0] * s[0] * (b1 * s[1] + b2 * s[2])) / 9 +
           (b11 * s[3] * s[4] * s[5]) * icste;
  }

  template <tfel::math::StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto computeJ3ODerivative(
      const StensorType& s, const J3OCoefficients<StensorType>& b) {
    return computeJ3ODerivative(s, b[0], b[1], b[2], b[3], b[4], b[5], b[6],
                                b[7], b[8], b[9], b[10]);
  }  // end of computeJ3ODerivative

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 1u)  //
      TFEL_HOST_DEVICE constexpr auto computeJ3ODerivative(
          const StensorType& s,
          const tfel::math::base_type<StensorType> b1,
          const tfel::math::base_type<StensorType> b2,
          const tfel::math::base_type<StensorType> b3,
          const tfel::math::base_type<StensorType> b4,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>) {
    using namespace tfel::math;
    auto dJ3O =
        stensor<1u,
                UnaryOperationResult<numeric_type<StensorType>, Power<2>>>{};
    dJ3O[0] = -((b4 - b2 + b1) * s[2] * s[2]) / 9 +
              (2 * (b4 + b1) * s[1] * s[2]) / 9 - (b4 * s[1] * s[1]) / 9 +
              (-2 * s[0] * (b1 * s[1] + b2 * s[2])) / 9 +
              ((b2 + b1) * s[0] * s[0]) / 9;
    dJ3O[1] = -((b4 + b1 - b3) * s[2] * s[2]) / 9 +
              (-2 * s[1] * (b3 * s[2] + b4 * s[0])) / 9 +
              (2 * (b4 + b1) * s[0] * s[2]) / 9 +
              ((b4 + b3) * s[1] * s[1]) / 9 - (b1 * s[0] * s[0]) / 9;
    dJ3O[2] =
        ((2 * (b4 + b1) - b3 - b2) * s[2] * s[2]) / 9 +
        (-2 * ((b4 + b1 - b3) * s[1] + (b4 - b2 + b1) * s[0]) * s[2]) / 9 -
        (b3 * s[1] * s[1]) / 9 + (2 * (b4 + b1) * s[0] * s[1]) / 9 -
        (b2 * s[0] * s[0]) / 9;
    return dJ3O;
  }  // end of computeJ3ODerivative

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 2u)  //
      TFEL_HOST_DEVICE constexpr auto computeJ3ODerivative(
          const StensorType& s,
          const tfel::math::base_type<StensorType> b1,
          const tfel::math::base_type<StensorType> b2,
          const tfel::math::base_type<StensorType> b3,
          const tfel::math::base_type<StensorType> b4,
          const tfel::math::base_type<StensorType> b5,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType> b10,
          const tfel::math::base_type<StensorType>) {
    using namespace tfel::math;
    auto dJ3O =
        stensor<2u,
                UnaryOperationResult<numeric_type<StensorType>, Power<2>>>{};
    dJ3O[0] = -((b5 - 2 * b10) * s[3] * s[3]) / 6 -
              ((b4 - b2 + b1) * s[2] * s[2]) / 9 +
              (2 * (b4 + b1) * s[1] * s[2]) / 9 - (b4 * s[1] * s[1]) / 9 +
              (-2 * s[0] * (b1 * s[1] + b2 * s[2])) / 9 +
              ((b2 + b1) * s[0] * s[0]) / 9;
    dJ3O[1] = (b5 * s[3] * s[3]) / 6 - ((b4 + b1 - b3) * s[2] * s[2]) / 9 +
              (-2 * s[1] * (b3 * s[2] + b4 * s[0])) / 9 +
              (2 * (b4 + b1) * s[0] * s[2]) / 9 +
              ((b4 + b3) * s[1] * s[1]) / 9 - (b1 * s[0] * s[0]) / 9;
    dJ3O[2] =
        -(b10 * s[3] * s[3]) / 3 +
        ((2 * (b4 + b1) - b3 - b2) * s[2] * s[2]) / 9 +
        (-2 * ((b4 + b1 - b3) * s[1] + (b4 - b2 + b1) * s[0]) * s[2]) / 9 -
        (b3 * s[1] * s[1]) / 9 + (2 * (b4 + b1) * s[0] * s[1]) / 9 -
        (b2 * s[0] * s[0]) / 9;
    dJ3O[3] =
        -((2 * b10 * s[2] - b5 * s[1] + (b5 - 2 * b10) * s[0]) * s[3]) / 3;
    return dJ3O;
  }  // end of computeJ3ODerivative

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 3u)  //
      constexpr auto computeJ3ODerivative(
          const StensorType& s,
          const tfel::math::base_type<StensorType> b1,
          const tfel::math::base_type<StensorType> b2,
          const tfel::math::base_type<StensorType> b3,
          const tfel::math::base_type<StensorType> b4,
          const tfel::math::base_type<StensorType> b5,
          const tfel::math::base_type<StensorType> b6,
          const tfel::math::base_type<StensorType> b7,
          const tfel::math::base_type<StensorType> b8,
          const tfel::math::base_type<StensorType> b9,
          const tfel::math::base_type<StensorType> b10,
          const tfel::math::base_type<StensorType> b11) {
    using namespace tfel::math;
    using real = base_type<StensorType>;
    constexpr const auto icste = Cste<real>::isqrt2;
    auto dJ3O =
        stensor<3u,
                UnaryOperationResult<numeric_type<StensorType>, Power<2>>>{};
    dJ3O[0] =
        (-((b7 + b6) * s[5] * s[5]) / 6) - ((b8 - 2 * b9) * s[4] * s[4]) / 6 -
        ((b5 - 2 * b10) * s[3] * s[3]) / 6 -
        ((b4 - b2 + b1) * s[2] * s[2]) / 9 + (2 * (b4 + b1) * s[1] * s[2]) / 9 -
        (b4 * s[1] * s[1]) / 9 + (-2 * s[0] * (b1 * s[1] + b2 * s[2])) / 9 +
        ((b2 + b1) * s[0] * s[0]) / 9;
    dJ3O[1] = (b6 * s[5] * s[5]) / 6 - (b9 * s[4] * s[4]) / 3 +
              (b5 * s[3] * s[3]) / 6 - ((b4 + b1 - b3) * s[2] * s[2]) / 9 +
              (-2 * s[1] * (b3 * s[2] + b4 * s[0])) / 9 +
              (2 * (b4 + b1) * s[0] * s[2]) / 9 +
              ((b4 + b3) * s[1] * s[1]) / 9 - (b1 * s[0] * s[0]) / 9;
    dJ3O[2] =
        (b7 * s[5] * s[5]) / 6 + (b8 * s[4] * s[4]) / 6 -
        (b10 * s[3] * s[3]) / 3 +
        ((2 * (b4 + b1) - b3 - b2) * s[2] * s[2]) / 9 +
        (-2 * ((b4 + b1 - b3) * s[1] + (b4 - b2 + b1) * s[0]) * s[2]) / 9 -
        (b3 * s[1] * s[1]) / 9 + (2 * (b4 + b1) * s[0] * s[1]) / 9 -
        (b2 * s[0] * s[0]) / 9;
    dJ3O[3] = (b11 * s[4] * s[5]) * icste -
              ((2 * b10 * s[2] - b5 * s[1] + (b5 - 2 * b10) * s[0]) * s[3]) / 3;
    dJ3O[4] =
        (b11 * s[3] * s[5]) * icste -
        (((-b8 * s[2]) + 2 * b9 * s[1] + (b8 - 2 * b9) * s[0]) * s[4]) / 3;
    dJ3O[5] = (b11 * s[3] * s[4]) * icste -
              (((-b7 * s[2]) - b6 * s[1] + (b7 + b6) * s[0]) * s[5]) / 3;
    return dJ3O;
  }  // end of computeJ3ODerivative

  template <tfel::math::StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto computeJ3OSecondDerivative(
      const StensorType& s, const J3OCoefficients<StensorType>& b) {
    return computeJ3OSecondDerivative(s, b[0], b[1], b[2], b[3], b[4], b[5],
                                      b[6], b[7], b[8], b[9], b[10]);
  }  // end of computeJ3OSecondDerivative

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 1u)  //
      TFEL_HOST_DEVICE constexpr auto computeJ3OSecondDerivative(
          const StensorType& s,
          const tfel::math::base_type<StensorType> b1,
          const tfel::math::base_type<StensorType> b2,
          const tfel::math::base_type<StensorType> b3,
          const tfel::math::base_type<StensorType> b4,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>) {
    auto d2J3O =
        tfel::math::st2tost2<1u, tfel::math::numeric_type<StensorType>>{};
    d2J3O(0, 0) =
        (-2 * (b2 * s[2] + b1 * s[1])) / 9 + (2 * (b2 + b1) * s[0]) / 9;
    d2J3O(0, 1) = (2 * (b4 + b1) * s[2]) / 9 + (-2 * b4 * s[1]) / 9 +
                  (-2 * b1 * s[0]) / 9;
    d2J3O(0, 2) = (-2 * (b4 - b2 + b1) * s[2]) / 9 +
                  (2 * (b4 + b1) * s[1]) / 9 + (-2 * b2 * s[0]) / 9;
    d2J3O(1, 0) = (2 * (b4 + b1) * s[2]) / 9 + (-2 * b4 * s[1]) / 9 +
                  (-2 * b1 * s[0]) / 9;
    d2J3O(1, 1) =
        (-2 * (b3 * s[2] + b4 * s[0])) / 9 + (2 * (b4 + b3) * s[1]) / 9;
    d2J3O(1, 2) = (-2 * (b4 + b1 - b3) * s[2]) / 9 + (-2 * b3 * s[1]) / 9 +
                  (2 * (b4 + b1) * s[0]) / 9;
    d2J3O(2, 0) = (-2 * (b4 - b2 + b1) * s[2]) / 9 +
                  (2 * (b4 + b1) * s[1]) / 9 + (-2 * b2 * s[0]) / 9;
    d2J3O(2, 1) = (-2 * (b4 + b1 - b3) * s[2]) / 9 + (-2 * b3 * s[1]) / 9 +
                  (2 * (b4 + b1) * s[0]) / 9;
    d2J3O(2, 2) = (2 * (2 * (b4 + b1) - b3 - b2) * s[2]) / 9 +
                  (-2 * ((b4 + b1 - b3) * s[1] + (b4 - b2 + b1) * s[0])) / 9;
    return d2J3O;
  }  // end of computeJ3OSecondDerivative

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 2u)  //
      TFEL_HOST_DEVICE constexpr auto computeJ3OSecondDerivative(
          const StensorType& s,
          const tfel::math::base_type<StensorType> b1,
          const tfel::math::base_type<StensorType> b2,
          const tfel::math::base_type<StensorType> b3,
          const tfel::math::base_type<StensorType> b4,
          const tfel::math::base_type<StensorType> b5,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType>,
          const tfel::math::base_type<StensorType> b10,
          const tfel::math::base_type<StensorType>) {
    auto d2J3O =
        tfel::math::st2tost2<2u, tfel::math::numeric_type<StensorType>>{};
    d2J3O(0, 0) =
        (-2 * (b2 * s[2] + b1 * s[1])) / 9 + (2 * (b2 + b1) * s[0]) / 9;
    d2J3O(0, 1) = (2 * (b4 + b1) * s[2]) / 9 + (-2 * b4 * s[1]) / 9 +
                  (-2 * b1 * s[0]) / 9;
    d2J3O(0, 2) = (-2 * (b4 - b2 + b1) * s[2]) / 9 +
                  (2 * (b4 + b1) * s[1]) / 9 + (-2 * b2 * s[0]) / 9;
    d2J3O(0, 3) = -((b5 - 2 * b10) * s[3]) / 3;
    d2J3O(1, 0) = (2 * (b4 + b1) * s[2]) / 9 + (-2 * b4 * s[1]) / 9 +
                  (-2 * b1 * s[0]) / 9;
    d2J3O(1, 1) =
        (-2 * (b3 * s[2] + b4 * s[0])) / 9 + (2 * (b4 + b3) * s[1]) / 9;
    d2J3O(1, 2) = (-2 * (b4 + b1 - b3) * s[2]) / 9 + (-2 * b3 * s[1]) / 9 +
                  (2 * (b4 + b1) * s[0]) / 9;
    d2J3O(1, 3) = (b5 * s[3]) / 3;
    d2J3O(2, 0) = (-2 * (b4 - b2 + b1) * s[2]) / 9 +
                  (2 * (b4 + b1) * s[1]) / 9 + (-2 * b2 * s[0]) / 9;
    d2J3O(2, 1) = (-2 * (b4 + b1 - b3) * s[2]) / 9 + (-2 * b3 * s[1]) / 9 +
                  (2 * (b4 + b1) * s[0]) / 9;
    d2J3O(2, 2) = (2 * (2 * (b4 + b1) - b3 - b2) * s[2]) / 9 +
                  (-2 * ((b4 + b1 - b3) * s[1] + (b4 - b2 + b1) * s[0])) / 9;
    d2J3O(2, 3) = (-2 * b10 * s[3]) / 3;
    d2J3O(3, 0) = -((b5 - 2 * b10) * s[3]) / 3;
    d2J3O(3, 1) = (b5 * s[3]) / 3;
    d2J3O(3, 2) = (-2 * b10 * s[3]) / 3;
    d2J3O(3, 3) = -(2 * b10 * s[2] - b5 * s[1] + (b5 - 2 * b10) * s[0]) / 3;
    return d2J3O;
  }  // end of computeJ3OSecondDerivative

  template <tfel::math::StensorConcept StensorType>
  requires(tfel::math::getSpaceDimension<StensorType>() == 3u)  //
      TFEL_HOST_DEVICE constexpr auto computeJ3OSecondDerivative(
          const StensorType& s,
          const tfel::math::base_type<StensorType> b1,
          const tfel::math::base_type<StensorType> b2,
          const tfel::math::base_type<StensorType> b3,
          const tfel::math::base_type<StensorType> b4,
          const tfel::math::base_type<StensorType> b5,
          const tfel::math::base_type<StensorType> b6,
          const tfel::math::base_type<StensorType> b7,
          const tfel::math::base_type<StensorType> b8,
          const tfel::math::base_type<StensorType> b9,
          const tfel::math::base_type<StensorType> b10,
          const tfel::math::base_type<StensorType> b11) {
    using namespace tfel::math;
    using real = base_type<StensorType>;
    constexpr const auto icste = Cste<real>::isqrt2;
    auto d2J3O = st2tost2<3u, numeric_type<StensorType>>{};
    d2J3O(0, 0) =
        (-2 * (b2 * s[2] + b1 * s[1])) / 9 + (2 * (b2 + b1) * s[0]) / 9;
    d2J3O(0, 1) = (2 * (b4 + b1) * s[2]) / 9 + (-2 * b4 * s[1]) / 9 +
                  (-2 * b1 * s[0]) / 9;
    d2J3O(0, 2) = (-2 * (b4 - b2 + b1) * s[2]) / 9 +
                  (2 * (b4 + b1) * s[1]) / 9 + (-2 * b2 * s[0]) / 9;
    d2J3O(0, 3) = -((b5 - 2 * b10) * s[3]) / 3;
    d2J3O(0, 4) = -((b8 - 2 * b9) * s[4]) / 3;
    d2J3O(0, 5) = -((b7 + b6) * s[5]) / 3;
    d2J3O(1, 0) = (2 * (b4 + b1) * s[2]) / 9 + (-2 * b4 * s[1]) / 9 +
                  (-2 * b1 * s[0]) / 9;
    d2J3O(1, 1) =
        (-2 * (b3 * s[2] + b4 * s[0])) / 9 + (2 * (b4 + b3) * s[1]) / 9;
    d2J3O(1, 2) = (-2 * (b4 + b1 - b3) * s[2]) / 9 + (-2 * b3 * s[1]) / 9 +
                  (2 * (b4 + b1) * s[0]) / 9;
    d2J3O(1, 3) = (b5 * s[3]) / 3;
    d2J3O(1, 4) = (-2 * b9 * s[4]) / 3;
    d2J3O(1, 5) = (b6 * s[5]) / 3;
    d2J3O(2, 0) = (-2 * (b4 - b2 + b1) * s[2]) / 9 +
                  (2 * (b4 + b1) * s[1]) / 9 + (-2 * b2 * s[0]) / 9;
    d2J3O(2, 1) = (-2 * (b4 + b1 - b3) * s[2]) / 9 + (-2 * b3 * s[1]) / 9 +
                  (2 * (b4 + b1) * s[0]) / 9;
    d2J3O(2, 2) = (2 * (2 * (b4 + b1) - b3 - b2) * s[2]) / 9 +
                  (-2 * ((b4 + b1 - b3) * s[1] + (b4 - b2 + b1) * s[0])) / 9;
    d2J3O(2, 3) = (-2 * b10 * s[3]) / 3;
    d2J3O(2, 4) = (b8 * s[4]) / 3;
    d2J3O(2, 5) = (b7 * s[5]) / 3;
    d2J3O(3, 0) = -((b5 - 2 * b10) * s[3]) / 3;
    d2J3O(3, 1) = (b5 * s[3]) / 3;
    d2J3O(3, 2) = (-2 * b10 * s[3]) / 3;
    d2J3O(3, 3) = -(2 * b10 * s[2] - b5 * s[1] + (b5 - 2 * b10) * s[0]) / 3;
    d2J3O(3, 4) = (b11 * s[5]) * icste;
    d2J3O(3, 5) = (b11 * s[4]) * icste;
    d2J3O(4, 0) = -((b8 - 2 * b9) * s[4]) / 3;
    d2J3O(4, 1) = (-2 * b9 * s[4]) / 3;
    d2J3O(4, 2) = (b8 * s[4]) / 3;
    d2J3O(4, 3) = (b11 * s[5]) * icste;
    d2J3O(4, 4) = -((-b8 * s[2]) + 2 * b9 * s[1] + (b8 - 2 * b9) * s[0]) / 3;
    d2J3O(4, 5) = (b11 * s[3]) * icste;
    d2J3O(5, 0) = -((b7 + b6) * s[5]) / 3;
    d2J3O(5, 1) = (b6 * s[5]) / 3;
    d2J3O(5, 2) = (b7 * s[5]) / 3;
    d2J3O(5, 3) = (b11 * s[4]) * icste;
    d2J3O(5, 4) = (b11 * s[3]) * icste;
    d2J3O(5, 5) = -((-b7 * s[2]) - b6 * s[1] + (b7 + b6) * s[0]) / 3;
    return d2J3O;
  }  // end of computeJ3OSecondDerivative

}  // end of namespace tfel::material

#endif /* LIBTFEL_MATERIAL_ORTHOTROPICPLASTICITY_IXX */
