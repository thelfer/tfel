/*!
 * \file   include/TFEL/Material/Barlat2004YieldCriterion.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/11/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_BARLAT_HXX
#define LIB_TFEL_MATERIAL_BARLAT_HXX

#include <tuple>
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/OrthotropicAxesConvention.hxx"

namespace tfel::material {

  //! a simple alias
  template <typename StressStensor>
  using BarlatStressType = tfel::math::numeric_type<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using BarlatBaseType =
      tfel::typetraits::base_type<tfel::math::numeric_type<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using BarlatLinearTransformationType =
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StressStensor>(),
                           BarlatBaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using BarlatInvertStressType =
      tfel::math::result_type<BarlatBaseType<StressStensor>,
                              BarlatStressType<StressStensor>,
                              tfel::math::OpDiv>;
  //! a simple alias
  template <typename StressStensor>
  using BarlatStressNormalType =
      tfel::math::stensor<tfel::math::getSpaceDimension<StressStensor>(),
                          BarlatBaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using BarlatStressEigenTensorType =
      tfel::math::stensor<tfel::math::getSpaceDimension<StressStensor>(),
                          BarlatBaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using BarlatStressSecondDerivativeType =
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StressStensor>(),
                           BarlatInvertStressType<StressStensor>>;
  /*!
   * \brief an helper structure used to simplify the code
   * \tparam StressStensor: type of the stress tensor
   */
  template <typename StressStensor>
  struct BarlatStressAndDerivativesWithRespectToEigenvalues {
    //! Barlat stress
    BarlatStressType<StressStensor> Phi;
    /*!
     * Derivative of the Barlat stress with respect to the
     * eigenvalues of s1
     */
    tfel::math::tvector<3u, BarlatBaseType<StressStensor>> dPhi_dsvp1;
    /*!
     * Derivative of the Barlat stress with respect to the
     * eigenvalues of s2
     */
    tfel::math::tvector<3u, BarlatBaseType<StressStensor>> dPhi_dsvp2;
    /*!
     * Second derivative of the Barlat stress with respect to the
     * eigenvalues of s1
     */
    tfel::math::tvector<6u, BarlatInvertStressType<StressStensor>> d2Phi_dsvp12;
    /*!
     * Second derivative of the Barlat stress with respect to the
     * eigenvalues of s2
     */
    tfel::math::tvector<6u, BarlatInvertStressType<StressStensor>> d2Phi_dsvp22;
    /*!
     * Second derivative of the Barlat stress with respect to the
     * eigenvalues of s1 and s2
     */
    tfel::math::tvector<9u, BarlatInvertStressType<StressStensor>>
        d2Phi_dsvp1dsvp2;
  };  // end of struct BarlatStressAndDerivativesWithRespectToEigenvalues
      /*!
       * \tparam N:    space dimension
       * \tparam real: numerical type
       * \return a linear transformation of the stresses
       * \param[in] c12: coefficient of the linear transformation
       * \param[in] c21: coefficient of the linear transformation
       * \param[in] c13: coefficient of the linear transformation
       * \param[in] c31: coefficient of the linear transformation
       * \param[in] c23: coefficient of the linear transformation
       * \param[in] c32: coefficient of the linear transformation
       * \param[in] c44: coefficient of the linear transformation
       * \param[in] c55: coefficient of the linear transformation
       * \param[in] c66: coefficient of the linear transformation
       *
       * The linear transformation is defined as follows:
       * \f[
       * \underline{L}=
       * \frac{1}{3}\,
       * \begin{pmatrix}
       * 0 & -c_{12} & -c_{13} & 0 & 0 & 0 \\
       * -c_{21} & 0 & -c_{23} & 0 & 0 & 0 \\
       * -c_{31} & -c_{32} & 0 & 0 & 0 & 0 \\
       * 0 & 0 & 0 & c_{44} & 0 & 0 \\
       * 0 & 0 & 0 & 0 & c_{55} & 0 \\
       * 0 & 0 & 0 & 0 & 0 & c_{66} \\
       * \end{pmatrix}
       * \,
       * \cdot
       * \,
       * \begin{pmatrix}
       * 2  & -1 & -1 & 0 & 0 & 0 \\
       * -1 &  2 & -1 & 0 & 0 & 0 \\
       * -1 & -1 & 2 & 0 & 0 & 0 \\
       * 0 & 0 & 0 & 1 & 0 & 0 \\
       * 0 & 0 & 0 & 0 & 1 & 0 \\
       * 0 & 0 & 0 & 0 & 0 & 1 \\
       * \end{pmatrix}
       * \f]
       * \note Barlat uses the following convention for storing
       * symmetric tensors:
       * \f[
       * \begin{pmatrix}
       * xx & yy & zz & yz & zx & xy
       * \end{pmatrix}
       * \f]
       * which is not consistent with the
       * `TFEL`/`Cast3M`/`Abaqus`/`Ansys` conventions:
       * \f[
       * \begin{pmatrix}
       * xx & yy & zz & xy & xz & yz
       * \end{pmatrix}
       * \f]
       *
       * Therefore, if one wants to uses coeficients \f$c^{B}\f$ given
       * by Barlat, one shall call this function as follows:
       *
       * \code{.cpp}
       * const auto l1 =
       * makeBarlatLinearTransformation<3>(cB_12,cB_21,cB_13,cB_31,
       *                                                   cB_23,cB_32,cB_66,cBB_55,cBB_44);
       * \endcode
       */
  template <unsigned short N, typename real>
  TFEL_HOST_DEVICE constexpr tfel::math::st2tost2<N, real>
  makeBarlatLinearTransformation(const real,
                                 const real,
                                 const real,
                                 const real,
                                 const real,
                                 const real,
                                 const real,
                                 const real,
                                 const real);
  /*!
   * \tparam N:    space dimension
   * \tparam real: numerical type
   * \return a linear transformation of the stresses
   * \param[in] c: coefficients of the linear transformation
   *
   * The linear transformation is defined as follows:
   * \f[
   * \underline{L}=
   * \frac{1}{3}\,
   * \begin{pmatrix}
   * 0 & -c_{12} & -c_{13} & 0 & 0 & 0 \\
   * -c_{21} & 0 & -c_{23} & 0 & 0 & 0 \\
   * -c_{31} & -c_{32} & 0 & 0 & 0 & 0 \\
   * 0 & 0 & 0 & c_{44} & 0 & 0 \\
   * 0 & 0 & 0 & 0 & c_{55} & 0 \\
   * 0 & 0 & 0 & 0 & 0 & c_{66} \\
   * \end{pmatrix}
   * \,
   * \cdot
   * \,
   * \begin{pmatrix}
   * 2  & -1 & -1 & 0 & 0 & 0 \\
   * -1 &  2 & -1 & 0 & 0 & 0 \\
   * -1 & -1 & 2 & 0 & 0 & 0 \\
   * 0 & 0 & 0 & 1 & 0 & 0 \\
   * 0 & 0 & 0 & 0 & 1 & 0 \\
   * 0 & 0 & 0 & 0 & 0 & 1 \\
   * \end{pmatrix}
   * \f]
   */
  template <unsigned short N, typename real>
  TFEL_HOST_DEVICE constexpr tfel::math::st2tost2<N, real>
  makeBarlatLinearTransformation(const tfel::math::fsarray<9u, real>&);
  /*!
   * \tparam H: modelling hypothesis
   * \tparam c: orthotropic axis convention
   * \tparam real: numerical type
   * \return a linear transformation of the stresses
   * \param[in] c12: coefficient of the linear transformation
   * \param[in] c21: coefficient of the linear transformation
   * \param[in] c13: coefficient of the linear transformation
   * \param[in] c31: coefficient of the linear transformation
   * \param[in] c23: coefficient of the linear transformation
   * \param[in] c32: coefficient of the linear transformation
   * \param[in] c44: coefficient of the linear transformation
   * \param[in] c55: coefficient of the linear transformation
   * \param[in] c66: coefficient of the linear transformation
   *
   * \brief This function is barely a wrapper around the
   * `makeBarlatLinearTransformation<N,real>`, where `N` is
   * the space dimension associated with the modelling hypothesis
   * `H`.
   *
   * If the orthotropic axis convention is
   * `OrthotropicAxesConvention::DEFAULT`, then the arguments are
   * passed unchanged to the
   * `makeBarlatLinearTransformation<N,real>` function.
   *
   * If the orthotropic axis convention is
   * `OrthotropicAxesConvention::PIPE`, then the arguments passed to
   * the `makeBarlatLinearTransformation<N,real>` function depends
   * on the modelling hypothesis:
   * - in 1D, 2D axisymmetric, and 3D modelling hypotheses, the
   *   arguments are passed unchanged.
   * - in plane 2D modelling hypotheses (`PLANESTRAIN`,
   *   `PLANESTRESS`, `GENERALISEDPLANESTRAIN`), the coefficients
   *   relative to the second axis are exchanged with the
   *   coefficients related to the third axis.
   *
   * If the orthotropic axis convention is
   * `OrthotropicAxesConvention::PLATE`, then the arguments passed to
   * the `makeBarlatLinearTransformation<N,real>` function depends
   * on the modelling hypothesis:
   * - in 3D and in plane 2D modelling hypotheses (`PLANESTRAIN`,
   *   `PLANESTRESS`, `GENERALISEDPLANESTRAIN`), the coefficients
   *   relative to the second axis are exchanged with the
   *   coefficients related to the third axis.
   * - 1D, 2D axisymmetric modelling hypotheses are not available.
   */
  template <ModellingHypothesis::Hypothesis H,
            OrthotropicAxesConvention,
            typename real>
  TFEL_HOST_DEVICE constexpr tfel::math::
      st2tost2<ModellingHypothesisToSpaceDimension<H>::value, real>
      makeBarlatLinearTransformation(const real,
                                     const real,
                                     const real,
                                     const real,
                                     const real,
                                     const real,
                                     const real,
                                     const real,
                                     const real);
  /*!
   * \tparam H: modelling hypothesis
   * \tparam oac: orthotropic axis convention
   * \tparam real: numerical type
   * \return a linear transformation of the stresses
   * \param[in] c: coefficients of the linear transformation
   *
   * \brief This function is barely a wrapper around the
   * `makeBarlatLinearTransformation<N,real>`, where `N` is
   * the space dimension associated with the modelling hypothesis
   * `H`.
   *
   * If the orthotropic axis convention is
   * `OrthotropicAxesConvention::DEFAULT`, then the arguments are
   * passed unchanged to the
   * `makeBarlatLinearTransformation<N,real>` function.
   *
   * If the orthotropic axis convention is
   * `OrthotropicAxesConvention::PIPE`, then the arguments passed to
   * the `makeBarlatLinearTransformation<N,real>` function depends
   * on the modelling hypothesis:
   * - in 1D, 2D axisymmetric, and 3D modelling hypotheses, the
   *   arguments are passed unchanged.
   * - in plane 2D modelling hypotheses (`PLANESTRAIN`,
   *   `PLANESTRESS`, `GENERALISEDPLANESTRAIN`), the coefficients
   *   relative to the second axis are exchanged with the
   *   coefficients related to the third axis.
   *
   * If the orthotropic axis convention is
   * `OrthotropicAxesConvention::PLATE`, then the arguments passed to
   * the `makeBarlatLinearTransformation<N,real>` function depends
   * on the modelling hypothesis:
   * - in 3D and in plane 2D modelling hypotheses (`PLANESTRAIN`,
   *   `PLANESTRESS`, `GENERALISEDPLANESTRAIN`), the coefficients
   *   relative to the second axis are exchanged with the
   *   coefficients related to the third axis.
   * - 1D, 2D axisymmetric modelling hypotheses are not available.
   */
  template <ModellingHypothesis::Hypothesis H,
            OrthotropicAxesConvention,
            typename real>
  TFEL_HOST_DEVICE constexpr tfel::math::
      st2tost2<ModellingHypothesisToSpaceDimension<H>::value, real>
      makeBarlatLinearTransformation(const tfel::math::fsarray<9u, real>&);
  /*!
   * \brief This function computes the Barlat yield stress.
   * The Barlat yield stress is defined by:
   * \f[
   * \phi\left(\underline{\bf s}\right) =
   * \sqrt[a]{
   *   \frac{1}{4}\left(
   *   \sum_{i=0}^{3}
   *   \sum_{j=0}^{3}
   *   \left|s'_{i}-s''_{j}\right|^{a}
   *   \right)
   * }
   * \f]
   * where \f$s'_{i}\f$ and \f$s''_{j}\f$ are the eigenvalues
   * computed from two tensors \f$\underline{\bf s}'\f$ and
   * \f$\underline{\bf s}''\f$ resulting from a linear
   * transformation of the stress tensor:
   * \f[
   * \left\{
   * \begin{aligned}
   *  \underline{\bf s}'  &= \underline{\underline{L}}'
   * \,\colon\,\underline{\bf s} \\
   *  \underline{\bf s}'' &=
   * \underline{\underline{L}}''\,\colon\,\underline{\bf s} \\
   * \end{aligned}
   * \right.
   * \f]
   *
   * The linear transformations \f$\underline{\underline{L}}'\f$ and
   * \f$\underline{\underline{L}}''\f$ can be deduced from linear
   * transformations \f$\underline{\underline{C}}'\f$ and
   * \f$\underline{\underline{C}}''\f$. on the deviator of the
   * stress as follows:
   *
   * \f[
   * \begin{aligned}
   * \underline{L}'
   * &=\underline{\underline{C}}'\,\colon\,\underline{\underline{M}} \\
   * \underline{L}''
   * &=\underline{\underline{C}}'\,\colon\,\underline{\underline{M}} \\
   * \end{aligned}
   * \f]
   * where \f$\underline{\underline{M}}\f$ is the transformation of
   * the stress to its deviator:
   * \f[
   * \underline{\underline{M}}=\underline{\underline{I}}-\frac{1}{3}\underline{I}\,\otimes\,\underline{I}
   * \f]
   *
   * The linear transformations \f$\underline{\underline{C}}'\f$ and
   * \f$\underline{\underline{C}}''\f$ are defined by \f$9\f$
   * coefficients (each) which describe the material orthotropy, as
   * follows:
   *
   * \f[
   * \underline{\underline{C}}'=
   * \begin{pmatrix}
   * 0 & -c'_{12} & -c'_{13} & 0 & 0 & 0 \\
   * -c'_{21} & 0 & -c'_{23} & 0 & 0 & 0 \\
   * -c'_{31} & -c'_{32} & 0 & 0 & 0 & 0 \\
   * 0 & 0 & 0 & c'_{44} & 0 & 0 \\
   * 0 & 0 & 0 & 0 & c'_{55} & 0 \\
   * 0 & 0 & 0 & 0 & 0 & c'_{66} \\
   * \end{pmatrix}
   * \quad
   * \text{and}
   * \quad
   * \underline{\underline{C}}''=
   * \begin{pmatrix}
   * 0 & -c''_{12} & -c''_{13} & 0 & 0 & 0 \\
   * -c''_{21} & 0 & -c''_{23} & 0 & 0 & 0 \\
   * -c''_{31} & -c''_{32} & 0 & 0 & 0 & 0 \\
   * 0 & 0 & 0 & c''_{44} & 0 & 0 \\
   * 0 & 0 & 0 & 0 & c''_{55} & 0 \\
   * 0 & 0 & 0 & 0 & 0 & c''_{66} \\
   * \end{pmatrix}
   * \f]
   *
   * \see `makeBarlatLinearTransformation`
   * \tparam StressStensor: type of the stress tensor
   * \tparam BarlatExponentType: type of the hosford exponent
   * (could be a numeric type or an integer type)
   * \tparam es: eigen solver to be used
   * \param[in] s: stress tensor
   * \param[in] l1: first linear transformation
   * \param[in] l2: second linear transformation
   * \param[in] a: Barlat exponent
   * \param[in] e: criterion used to check if the von Mises stress is null
   * \return the Barlat yield stress \f$\phi\left(\underline{\bf s}\right)\f$
   */
  template <typename StressStensor,
            typename BarlatExponentType,
            tfel::math::stensor_common::EigenSolver =
                tfel::math::stensor_common::TFELEIGENSOLVER>
  TFEL_HOST_DEVICE BarlatStressType<StressStensor> computeBarlatStress(
      const StressStensor&,
      const BarlatLinearTransformationType<StressStensor>&,
      const BarlatLinearTransformationType<StressStensor>&,
      const BarlatExponentType,
      const BarlatStressType<StressStensor>);
  /*!
   * \brief compute the Barlat yield stress and the its first derivative
   * \tparam StressStensor: type of the stress tensor
   * \tparam BarlatExponentType: type of the hosford exponent
   * (could be a numeric type or an integer type)
   * \tparam es: eigen solver to be used
   * \param[in] s: stress tensor
   * \param[in] l1: first linear transformation
   * \param[in] l2: second linear transformation
   * \param[in] a: Barlat exponent
   * \param[in] e: criterion used to check if the von Mises stress is null.
   * \see `computeBarlatStress`
   */
  template <typename StressStensor,
            typename BarlatExponentType,
            tfel::math::stensor_common::EigenSolver =
                tfel::math::stensor_common::TFELEIGENSOLVER>
  TFEL_HOST_DEVICE std::tuple<BarlatStressType<StressStensor>,
                              BarlatStressNormalType<StressStensor>>
  computeBarlatStressNormal(
      const StressStensor&,
      const BarlatLinearTransformationType<StressStensor>&,
      const BarlatLinearTransformationType<StressStensor>&,
      const BarlatExponentType,
      const BarlatStressType<StressStensor>);
  /*!
   * \brief compute the Barlat yield stress and its first and second
   * derivatives with respect to the eigen values of the transformed
   * stress \f$s_{1}\f$ and \f$s_{2}\f$
   * \tparam StressStensor: type of the stress tensor
   * \tparam BarlatExponentType: type of the hosford exponent
   * (could be a numeric type or an integer type)
   * \param[in] vp1: eigen values of the first transformed stress
   * \param[in] vp2: eigen values of the second transformed stress
   * \param[in] seq: von Mises stress
   * \param[in] a: Barlat exponent
   * \see `computeBarlatStress`
   */
  template <typename StressStensor, typename BarlatExponentType>
  TFEL_HOST_DEVICE
      BarlatStressAndDerivativesWithRespectToEigenvalues<StressStensor>
      computeBarlatStressSecondDerivative(
          const tfel::math::tvector<3u, BarlatStressType<StressStensor>>&,
          const tfel::math::tvector<3u, BarlatStressType<StressStensor>>&,
          const BarlatStressType<StressStensor>,
          const BarlatExponentType);
  /*!
   * \brief compute the Barlat yield stress and its first and second
   * derivatives
   * \tparam StressStensor: type of the stress tensor
   * \tparam BarlatExponentType: type of the hosford exponent
   * (could be a numeric type or an integer type)
   * \tparam es: eigen solver to be used
   * \param[in] s: stress tensor
   * \param[in] l1: first linear transformation
   * \param[in] l2: second linear transformation
   * \param[in] a: Barlat exponent
   * \param[in] e: criterion used to check if the stress are null
   * \see `computeBarlatStress`
   */
  template <typename StressStensor,
            typename BarlatExponentType,
            tfel::math::stensor_common::EigenSolver =
                tfel::math::stensor_common::TFELEIGENSOLVER>
  TFEL_HOST_DEVICE std::tuple<BarlatStressType<StressStensor>,
                              BarlatStressNormalType<StressStensor>,
                              BarlatStressSecondDerivativeType<StressStensor>>
  computeBarlatStressSecondDerivative(
      const StressStensor&,
      const BarlatLinearTransformationType<StressStensor>&,
      const BarlatLinearTransformationType<StressStensor>&,
      const BarlatExponentType,
      const BarlatStressType<StressStensor>);

}  // end of namespace tfel::material

#include "TFEL/Material/Barlat2004YieldCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL_BARLAT_HXX */
