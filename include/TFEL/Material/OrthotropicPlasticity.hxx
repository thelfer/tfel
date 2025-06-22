/*!
 * \file   OrthotropicPlasticity.hxx
 * \brief
 * \author Thomas Helfer
 * \date   14/01/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ORTHOTROPICPLASTICITY_HXX
#define LIB_TFEL_MATERIAL_ORTHOTROPICPLASTICITY_HXX

#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel::material {

  //! a simple alias
  template <typename StensorType>
  using J2OCoefficients =
      tfel::math::tvector<6u, tfel::math::base_type<StensorType>>;
  /*!
   * \brief compute \f$J_{2}^{O}\f$ the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * J_{2}^{O}=
   * a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+
   * \frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] a: coefficients
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond,
      tfel::math::UnaryOperationResult<tfel::math::numeric_type<StensorType>,
                                       tfel::math::Power<2>>>::type
  computeJ2O(const StensorType&, const J2OCoefficients<StensorType>&);
  /*!
   * \brief compute \f$J_{2}^{O}\f$ the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * J_{2}^{O}=
   * a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+
   * \frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] a1: first coefficient
   * \param[in] a2: second coefficient
   * \param[in] a3: third coefficient
   * \param[in] a4: fourth coefficient
   * \param[in] a5: fifth coefficient
   * \param[in] a6: sixth coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 1u,
      tfel::math::UnaryOperationResult<tfel::math::numeric_type<StensorType>,
                                       tfel::math::Power<2>>>::type
  computeJ2O(const StensorType&,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute \f$J_{2}^{O}\f$ the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * J_{2}^{O}=
   * a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+
   * \frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] a1: first coefficient
   * \param[in] a2: second coefficient
   * \param[in] a3: third coefficient
   * \param[in] a4: fourth coefficient
   * \param[in] a5: fifth coefficient
   * \param[in] a6: sixth coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 2u,
      tfel::math::UnaryOperationResult<tfel::math::numeric_type<StensorType>,
                                       tfel::math::Power<2>>>::type
  computeJ2O(const StensorType&,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute \f$J_{2}^{O}\f$ the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * J_{2}^{O}=
   * a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+
   * \frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] a1: first coefficient
   * \param[in] a2: second coefficient
   * \param[in] a3: third coefficient
   * \param[in] a4: fourth coefficient
   * \param[in] a5: fifth coefficient
   * \param[in] a6: sixth coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 3u,
      tfel::math::UnaryOperationResult<tfel::math::numeric_type<StensorType>,
                                       tfel::math::Power<2>>>::type
  computeJ2O(const StensorType&,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute the derivative \f$\frac{\partial
   * J_{2}^{O}}{\partial \underline{s}}\f$ of \f$J_{2}^{O}\f$ with respect
   * to the stress \(\underline{s}\).
   *
   * \f$J_{2}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * J_{2}^{O}=
   * a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+
   * \frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] a: coefficients
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond,
      tfel::math::stensor<tfel::math::getSpaceDimension<StensorType>(),
                          tfel::math::numeric_type<StensorType>>>::type
  computeJ2ODerivative(const StensorType&, const J2OCoefficients<StensorType>&);
  /*!
   * \brief compute the derivative \f$\frac{\partial
   * J_{2}^{O}}{\partial \underline{s}}\f$ of \f$J_{2}^{O}\f$ with respect
   * to the stress \(\underline{s}\).
   *
   * \f$J_{2}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * J_{2}^{O}=
   * a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+
   * \frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] a1: first coefficient
   * \param[in] a2: second coefficient
   * \param[in] a3: third coefficient
   * \param[in] a4: fourth coefficient
   * \param[in] a5: fifth coefficient
   * \param[in] a6: sixth coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 1u,
      tfel::math::stensor<tfel::math::getSpaceDimension<StensorType>(),
                          tfel::math::numeric_type<StensorType>>>::type
  computeJ2ODerivative(const StensorType&,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute the derivative \f$\frac{\partial
   * J_{2}^{O}}{\partial \underline{s}}\f$ of \f$J_{2}^{O}\f$ with respect
   * to the stress \(\underline{s}\).
   *
   * \f$J_{2}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * J_{2}^{O}=
   * a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+
   * \frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] a1: first coefficient
   * \param[in] a2: second coefficient
   * \param[in] a3: third coefficient
   * \param[in] a4: fourth coefficient
   * \param[in] a5: fifth coefficient
   * \param[in] a6: sixth coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 2u,
      tfel::math::stensor<tfel::math::getSpaceDimension<StensorType>(),
                          tfel::math::numeric_type<StensorType>>>::type
  computeJ2ODerivative(const StensorType&,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute the derivative \f$\frac{\partial
   * J_{2}^{O}}{\partial \underline{s}}\f$ of \f$J_{2}^{O}\f$ with respect
   * to the stress \(\underline{s}\).
   *
   * \f$J_{2}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * J_{2}^{O}=
   * a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+
   * \frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] a1: first coefficient
   * \param[in] a2: second coefficient
   * \param[in] a3: third coefficient
   * \param[in] a4: fourth coefficient
   * \param[in] a5: fifth coefficient
   * \param[in] a6: sixth coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 3u,
      tfel::math::stensor<tfel::math::getSpaceDimension<StensorType>(),
                          tfel::math::numeric_type<StensorType>>>::type
  computeJ2ODerivative(const StensorType&,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute the derivative \f$\frac{\partial^{2}
   * J_{2}^{O}}{\partial^{2} \underline{s}}\f$ of \f$J_{2}^{O}\f$ with respect
   * to the stress \(\underline{s}\).
   *
   * \f$J_{2}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * J_{2}^{O}=
   * a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+
   * \frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] a: coefficients
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StensorType>(),
                           tfel::math::BinaryOperationResult<
                               tfel::math::base_type<StensorType>,
                               tfel::math::numeric_type<StensorType>,
                               tfel::math::OpDiv>>>::type
  computeJ2OSecondDerivative(const StensorType&,
                             const J2OCoefficients<StensorType>&);
  /*!
   * \brief compute the derivative \f$\frac{\partial^{2}
   * J_{2}^{O}}{\partial^{2} \underline{s}}\f$ of \f$J_{2}^{O}\f$ with respect
   * to the stress \(\underline{s}\).
   *
   * \f$J_{2}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * J_{2}^{O}=
   * a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+
   * \frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] a1: first coefficient
   * \param[in] a2: second coefficient
   * \param[in] a3: third coefficient
   * \param[in] a4: fourth coefficient
   * \param[in] a5: fifth coefficient
   * \param[in] a6: sixth coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 1u,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StensorType>(),
                           tfel::math::BinaryOperationResult<
                               tfel::math::base_type<StensorType>,
                               tfel::math::numeric_type<StensorType>,
                               tfel::math::OpDiv>>>::type
  computeJ2OSecondDerivative(const StensorType&,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute the derivative \f$\frac{\partial^{2}
   * J_{2}^{O}}{\partial^{2} \underline{s}}\f$ of \f$J_{2}^{O}\f$ with respect
   * to the stress \(\underline{s}\).
   *
   * \f$J_{2}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * J_{2}^{O}=
   * a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+
   * \frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] a1: first coefficient
   * \param[in] a2: second coefficient
   * \param[in] a3: third coefficient
   * \param[in] a4: fourth coefficient
   * \param[in] a5: fifth coefficient
   * \param[in] a6: sixth coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 2u,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StensorType>(),
                           tfel::math::BinaryOperationResult<
                               tfel::math::base_type<StensorType>,
                               tfel::math::numeric_type<StensorType>,
                               tfel::math::OpDiv>>>::type
  computeJ2OSecondDerivative(const StensorType&,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute the derivative \f$\frac{\partial^{2}
   * J_{2}^{O}}{\partial^{2} \underline{s}}\f$ of \f$J_{2}^{O}\f$ with respect
   * to the stress \(\underline{s}\).
   *
   * \f$J_{2}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * J_{2}^{O}=
   * a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+
   * \frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] a1: first coefficient
   * \param[in] a2: second coefficient
   * \param[in] a3: third coefficient
   * \param[in] a4: fourth coefficient
   * \param[in] a5: fifth coefficient
   * \param[in] a6: sixth coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 3u,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StensorType>(),
                           tfel::math::BinaryOperationResult<
                               tfel::math::base_type<StensorType>,
                               tfel::math::numeric_type<StensorType>,
                               tfel::math::OpDiv>>>::type
  computeJ2OSecondDerivative(const StensorType&,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>);

  //! a simple alias
  template <typename StensorType>
  using J3OCoefficients =
      tfel::math::tvector<11u, tfel::math::base_type<StensorType>>;
  /*!
   * \brief compute \f$J_{3}^{O}\f$ the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * \begin{aligned}
   * J_{3}^{O}=
   * &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
   * &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
   * &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
   * &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
   * &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
   * &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
   * &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
   * &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
   * &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
   * \end{aligned}
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] b: coefficients
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond,
      tfel::math::UnaryOperationResult<tfel::math::numeric_type<StensorType>,
                                       tfel::math::Power<3>>>::type
  computeJ3O(const StensorType&, const J3OCoefficients<StensorType>&);
  /*!
   * \brief compute \f$J_{3}^{O}\f$ the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * \begin{aligned}
   * J_{3}^{O}=
   * &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
   * &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
   * &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
   * &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
   * &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
   * &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
   * &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
   * &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
   * &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
   * \end{aligned}
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] b1: first coefficient
   * \param[in] b2: second coefficient
   * \param[in] b3: third coefficient
   * \param[in] b4: fourth coefficient
   * \param[in] b5: fifth coefficient
   * \param[in] b6: sixth coefficient
   * \param[in] b8: seventh coefficient
   * \param[in] b9: ninth coefficient
   * \param[in] b10: tenth coefficient
   * \param[in] b11: eleventh coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 1u,
      tfel::math::UnaryOperationResult<tfel::math::numeric_type<StensorType>,
                                       tfel::math::Power<3>>>::type
  computeJ3O(const StensorType&,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute \f$J_{3}^{O}\f$ the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * \begin{aligned}
   * J_{3}^{O}=
   * &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
   * &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
   * &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
   * &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
   * &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
   * &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
   * &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
   * &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
   * &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
   * \end{aligned}
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] b1: first coefficient
   * \param[in] b2: second coefficient
   * \param[in] b3: third coefficient
   * \param[in] b4: fourth coefficient
   * \param[in] b5: fifth coefficient
   * \param[in] b6: sixth coefficient
   * \param[in] b8: seventh coefficient
   * \param[in] b9: ninth coefficient
   * \param[in] b10: tenth coefficient
   * \param[in] b11: eleventh coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 2u,
      tfel::math::UnaryOperationResult<tfel::math::numeric_type<StensorType>,
                                       tfel::math::Power<3>>>::type
  computeJ3O(const StensorType&,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute \f$J_{3}^{O}\f$ the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * \begin{aligned}
   * J_{3}^{O}=
   * &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
   * &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
   * &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
   * &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
   * &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
   * &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
   * &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
   * &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
   * &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
   * \end{aligned}
   * \f]
   *
   * \param[in] s: stress tensor
   * \param[in] b1: first coefficient
   * \param[in] b2: second coefficient
   * \param[in] b3: third coefficient
   * \param[in] b4: fourth coefficient
   * \param[in] b5: fifth coefficient
   * \param[in] b6: sixth coefficient
   * \param[in] b8: seventh coefficient
   * \param[in] b9: ninth coefficient
   * \param[in] b10: tenth coefficient
   * \param[in] b11: eleventh coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 3u,
      tfel::math::UnaryOperationResult<tfel::math::numeric_type<StensorType>,
                                       tfel::math::Power<3>>>::type
  computeJ3O(const StensorType&,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>,
             const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute the derivative \f$\frac{\partial
   * J_{3}^{O}}{\partial \underline{s}}\f$ of \f$J_{3}^{O}\f$ with
   * respect to the stress tensor \f$\underline{s}\f$.
   *
   * \f$J_{3}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * \begin{aligned}
   * J_{3}^{O}=
   * &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
   * &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
   * &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
   * &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
   * &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
   * &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
   * &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
   * &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
   * &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
   * \end{aligned}
   * \f]
   *
   * \return \f$\frac{\partial J_{3}^{O}}{\partial \underline{s}}\f$
   * \param[in] s: stress tensor
   * \param[in] b: coefficients
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond,
      tfel::math::stensor<tfel::math::getSpaceDimension<StensorType>(),
                          tfel::math::UnaryOperationResult<
                              tfel::math::numeric_type<StensorType>,
                              tfel::math::Power<2>>>>::type
  computeJ3ODerivative(const StensorType&, const J3OCoefficients<StensorType>&);
  /*!
   * \brief compute the derivative \f$\frac{\partial
   * J_{3}^{O}}{\partial \underline{s}}\f$ of \f$J_{3}^{O}\f$ with
   * respect to the stress tensor \f$\underline{s}\f$.
   *
   * \f$J_{3}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * \begin{aligned}
   * J_{3}^{O}=
   * &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
   * &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
   * &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
   * &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
   * &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
   * &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
   * &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
   * &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
   * &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
   * \end{aligned}
   * \f]
   *
   * \return \f$\frac{\partial J_{3}^{O}}{\partial \underline{s}}\f$
   * \param[in] s: stress tensor
   * \param[in] s: stress tensor
   * \param[in] b1: first coefficient
   * \param[in] b2: second coefficient
   * \param[in] b3: third coefficient
   * \param[in] b4: fourth coefficient
   * \param[in] b5: fifth coefficient
   * \param[in] b6: sixth coefficient
   * \param[in] b8: seventh coefficient
   * \param[in] b9: ninth coefficient
   * \param[in] b10: tenth coefficient
   * \param[in] b11: eleventh coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 1u,
      tfel::math::stensor<tfel::math::getSpaceDimension<StensorType>(),
                          tfel::math::UnaryOperationResult<
                              tfel::math::numeric_type<StensorType>,
                              tfel::math::Power<2>>>>::type
  computeJ3ODerivative(const StensorType&,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute the derivative \f$\frac{\partial
   * J_{3}^{O}}{\partial \underline{s}}\f$ of \f$J_{3}^{O}\f$ with
   * respect to the stress tensor \f$\underline{s}\f$.
   *
   * \f$J_{3}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * \begin{aligned}
   * J_{3}^{O}=
   * &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
   * &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
   * &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
   * &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
   * &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
   * &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
   * &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
   * &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
   * &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
   * \end{aligned}
   * \f]
   *
   * \return \f$\frac{\partial J_{3}^{O}}{\partial \underline{s}}\f$
   * \param[in] s: stress tensor
   * \param[in] s: stress tensor
   * \param[in] b1: first coefficient
   * \param[in] b2: second coefficient
   * \param[in] b3: third coefficient
   * \param[in] b4: fourth coefficient
   * \param[in] b5: fifth coefficient
   * \param[in] b6: sixth coefficient
   * \param[in] b8: seventh coefficient
   * \param[in] b9: ninth coefficient
   * \param[in] b10: tenth coefficient
   * \param[in] b11: eleventh coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 2u,
      tfel::math::stensor<tfel::math::getSpaceDimension<StensorType>(),
                          tfel::math::UnaryOperationResult<
                              tfel::math::numeric_type<StensorType>,
                              tfel::math::Power<2>>>>::type
  computeJ3ODerivative(const StensorType&,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute the derivative \f$\frac{\partial
   * J_{3}^{O}}{\partial \underline{s}}\f$ of \f$J_{3}^{O}\f$ with
   * respect to the stress tensor \f$\underline{s}\f$.
   *
   * \f$J_{3}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * \begin{aligned}
   * J_{3}^{O}=
   * &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
   * &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
   * &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
   * &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
   * &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
   * &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
   * &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
   * &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
   * &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
   * \end{aligned}
   * \f]
   *
   * \return \f$\frac{\partial J_{3}^{O}}{\partial \underline{s}}\f$
   * \param[in] s: stress tensor
   * \param[in] s: stress tensor
   * \param[in] b1: first coefficient
   * \param[in] b2: second coefficient
   * \param[in] b3: third coefficient
   * \param[in] b4: fourth coefficient
   * \param[in] b5: fifth coefficient
   * \param[in] b6: sixth coefficient
   * \param[in] b8: seventh coefficient
   * \param[in] b9: ninth coefficient
   * \param[in] b10: tenth coefficient
   * \param[in] b11: eleventh coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 3u,
      tfel::math::stensor<tfel::math::getSpaceDimension<StensorType>(),
                          tfel::math::UnaryOperationResult<
                              tfel::math::numeric_type<StensorType>,
                              tfel::math::Power<2>>>>::type
  computeJ3ODerivative(const StensorType&,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>,
                       const tfel::math::base_type<StensorType>);

  /*!
   * \brief compute the second derivative \f$\frac{\partial^{2}
   * J_{3}^{O}}{\partial^{2} \underline{s}}\f$ of \f$J_{3}^{O}\f$ with
   * respect to the stress tensor \f$\underline{s}\f$.
   *
   * \f$J_{3}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * \begin{aligned}
   * J_{3}^{O}=
   * &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
   * &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
   * &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
   * &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
   * &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
   * &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
   * &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
   * &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
   * &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
   * \end{aligned}
   * \f]
   *
   * \return \f$\frac{\partial^{2} J_{3}^{O}}{\partial^{2} \underline{s}}\f$
   * \param[in] s: stress tensor
   * \param[in] b: coefficients
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StensorType>(),
                           tfel::math::numeric_type<StensorType>>>::type
  computeJ3OSecondDerivative(const StensorType&,
                             const J3OCoefficients<StensorType>&);
  /*!
   * \brief compute the derivative \f$\frac{\partial^{2}
   * J_{3}^{O}}{\partial^{2} \underline{s}}\f$ of \f$J_{3}^{O}\f$ with
   * respect to the stress tensor \f$\underline{s}\f$.
   *
   * \f$J_{3}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * \begin{aligned}
   * J_{3}^{O}=
   * &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
   * &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
   * &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
   * &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
   * &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
   * &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
   * &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
   * &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
   * &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
   * \end{aligned}
   * \f]
   *
   * \return \f$\frac{\partial^{2} J_{3}^{O}}{\partial^{2} \underline{s}}\f$
   * \param[in] s: stress tensor
   * \param[in] s: stress tensor
   * \param[in] b1: first coefficient
   * \param[in] b2: second coefficient
   * \param[in] b3: third coefficient
   * \param[in] b4: fourth coefficient
   * \param[in] b5: fifth coefficient
   * \param[in] b6: sixth coefficient
   * \param[in] b8: seventh coefficient
   * \param[in] b9: ninth coefficient
   * \param[in] b10: tenth coefficient
   * \param[in] b11: eleventh coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 1u,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StensorType>(),
                           tfel::math::numeric_type<StensorType>>>::type
  computeJ3OSecondDerivative(const StensorType&,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute the derivative \f$\frac{\partial^{2}
   * J_{3}^{O}}{\partial^{2} \underline{s}}\f$ of \f$J_{3}^{O}\f$ with
   * respect to the stress tensor \f$\underline{s}\f$.
   *
   * \f$J_{3}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * \begin{aligned}
   * J_{3}^{O}=
   * &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
   * &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
   * &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
   * &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
   * &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
   * &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
   * &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
   * &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
   * &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
   * \end{aligned}
   * \f]
   *
   * \return \f$\frac{\partial^{2} J_{3}^{O}}{\partial^{2} \underline{s}}\f$
   * \param[in] s: stress tensor
   * \param[in] s: stress tensor
   * \param[in] b1: first coefficient
   * \param[in] b2: second coefficient
   * \param[in] b3: third coefficient
   * \param[in] b4: fourth coefficient
   * \param[in] b5: fifth coefficient
   * \param[in] b6: sixth coefficient
   * \param[in] b8: seventh coefficient
   * \param[in] b9: ninth coefficient
   * \param[in] b10: tenth coefficient
   * \param[in] b11: eleventh coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 2u,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StensorType>(),
                           tfel::math::numeric_type<StensorType>>>::type
  computeJ3OSecondDerivative(const StensorType&,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>);
  /*!
   * \brief compute the derivative \f$\frac{\partial^{2}
   * J_{3}^{O}}{\partial^{2} \underline{s}}\f$ of \f$J_{3}^{O}\f$ with
   * respect to the stress tensor \f$\underline{s}\f$.
   *
   * \f$J_{3}^{O}\f$ is the orthotropic generalization
   * of the third invariant of the deviatoric part of the stress
   * tensor, as introduced by Cazacu and Barlat.
   *
   * \f[
   * \begin{aligned}
   * J_{3}^{O}=
   * &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
   * &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
   * &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
   * &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
   * &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
   * &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
   * &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
   * &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
   * &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
   * \end{aligned}
   * \f]
   *
   * \return \f$\frac{\partial^{2} J_{3}^{O}}{\partial^{2} \underline{s}}\f$
   * \param[in] s: stress tensor
   * \param[in] s: stress tensor
   * \param[in] b1: first coefficient
   * \param[in] b2: second coefficient
   * \param[in] b3: third coefficient
   * \param[in] b4: fourth coefficient
   * \param[in] b5: fifth coefficient
   * \param[in] b6: sixth coefficient
   * \param[in] b8: seventh coefficient
   * \param[in] b9: ninth coefficient
   * \param[in] b10: tenth coefficient
   * \param[in] b11: eleventh coefficient
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::math::getSpaceDimension<StensorType>() == 3u,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StensorType>(),
                           tfel::math::numeric_type<StensorType>>>::type
  computeJ3OSecondDerivative(const StensorType&,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>,
                             const tfel::math::base_type<StensorType>);

}  // end of namespace tfel::material

#include "TFEL/Material/OrthotropicPlasticity.ixx"

#endif /* LIB_TFEL_MATERIAL_ORTHOTROPICPLASTICITY_HXX */
