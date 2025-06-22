/*!
 * \file   OrthotropicStressLinearTransformation.hxx
 * \brief
 * \author Thomas Helfer
 * \date   13/01/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ORTHOTROPICSTRESSLINEARTRANSFORMATION_HXX
#define LIB_TFEL_MATERIAL_ORTHOTROPICSTRESSLINEARTRANSFORMATION_HXX

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/OrthotropicAxesConvention.hxx"

namespace tfel::material {

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
   */
  template <unsigned short N, typename real>
  tfel::math::st2tost2<N, real> makeOrthotropicStressLinearTransformation(
      const real,
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
  tfel::math::st2tost2<N, real> makeOrthotropicStressLinearTransformation(
      const tfel::math::tvector<9u, real>&);
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
   * `makeOrthotropicStressLinearTransformation<N,real>`, where `N` is
   * the space dimension associated with the modelling hypothesis
   * `H`.
   *
   * If the orthotropic axis convention is
   * `OrthotropicAxesConvention::DEFAULT`, then the arguments are
   * passed unchanged to the
   * `makeOrthotropicStressLinearTransformation<N,real>` function.
   *
   * If the orthotropic axis convention is
   * `OrthotropicAxesConvention::PIPE`, then the arguments passed to
   * the `makeOrthotropicStressLinearTransformation<N,real>` function depends
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
   * the `makeOrthotropicStressLinearTransformation<N,real>` function depends
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
  tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value, real>
  makeOrthotropicStressLinearTransformation(const real,
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
   * `makeOrthotropicStressLinearTransformation<N,real>`, where `N` is
   * the space dimension associated with the modelling hypothesis
   * `H`.
   *
   * If the orthotropic axis convention is
   * `OrthotropicAxesConvention::DEFAULT`, then the arguments are
   * passed unchanged to the
   * `makeOrthotropicStressLinearTransformation<N,real>` function.
   *
   * If the orthotropic axis convention is
   * `OrthotropicAxesConvention::PIPE`, then the arguments passed to
   * the `makeOrthotropicStressLinearTransformation<N,real>` function depends
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
   * the `makeOrthotropicStressLinearTransformation<N,real>` function depends
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
  tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value, real>
  makeOrthotropicStressLinearTransformation(
      const tfel::math::tvector<9u, real>&);

}  // end of namespace tfel::material

#include "TFEL/Material/OrthotropicStressLinearTransformation.ixx"

#endif /* LIB_TFEL_MATERIAL_ORTHOTROPICSTRESSLINEARTRANSFORMATION_HXX */
