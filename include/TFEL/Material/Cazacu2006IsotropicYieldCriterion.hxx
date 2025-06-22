/*!
 * \file   include/TFEL/Material/Cazacu2006IsotropicYieldCriterion.hxx
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

#ifndef LIB_TFEL_MATERIAL_CAZACU2006ISOTROPICYIELDCRITERION_HXX
#define LIB_TFEL_MATERIAL_CAZACU2006ISOTROPICYIELDCRITERION_HXX

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel::material {

  //! a simple alias
  template <typename StressStensor>
  using CazacuStressType = tfel::math::numeric_type<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using CazacuBaseType =
      tfel::typetraits::base_type<tfel::math::numeric_type<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using CazacuInvertStressType =
      tfel::math::result_type<CazacuBaseType<StressStensor>,
                              CazacuStressType<StressStensor>,
                              tfel::math::OpDiv>;
  //! a simple alias
  template <typename StressStensor>
  using CazacuStressNormalType =
      tfel::math::stensor<tfel::math::getSpaceDimension<StressStensor>(),
                          CazacuBaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using CazacuStressEigenTensorType =
      tfel::math::stensor<tfel::math::getSpaceDimension<StressStensor>(),
                          CazacuBaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using CazacuStressSecondDerivativeType =
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StressStensor>(),
                           CazacuInvertStressType<StressStensor>>;
  /*!
   * \brief compute the Cazacu 2006 yield stress defined as follows:
   * \f[
   * \phi\left(\underline{\bf sig}\right) =
   * \sqrt[a]{
   *   \left(\left|S_{1}|-k\,S_{1}\right)^{a}
   *   \left(\left|S_{1}|-k\,S_{2}\right)^{a}
   *   \left(\left|S_{3}|-k\,S_{3}\right)^{a}
   * }
   * \f]
   * where \f$S_{1}\f$, \f$S_{2}\f$, and \f$S_{3}\f$, are the
   * eigenvalues of the deviatoric part of the stress tensor.
   *
   * \tparam StressStensor: type of the stress tensor
   * \tparam CazacuExponentType: type of the hosford exponent
   * (could be a numeric type or an integer type)
   * \tparam es: eigen solver to be used
   * \param[in] sig: stress tensor
   * \param[in] a: Cazacu exponent
   * \param[in] k: Cazacu coefficient
   * \param[in] e: criterion used to check if the stress are null
   */
  template <typename StressStensor,
            typename CazacuExponentType,
            tfel::math::stensor_common::EigenSolver =
                tfel::math::stensor_common::TFELEIGENSOLVER>
  CazacuStressType<StressStensor> computeCazacu2006IsotropicStress(
      const StressStensor&,
      const CazacuExponentType,
      const CazacuBaseType<StressTensor>,
      const CazacuStressType<StressStensor>);
  /*!
   * \brief compute the Cazacu yield stress and the its first derivative
   * \tparam StressStensor: type of the stress tensor
   * \tparam CazacuExponentType: type of the hosford exponent
   * (could be a numeric type or an integer type)
   * \tparam es: eigen solver to be used
   * \param[in] sig: stress tensor
   * \param[in] a: Cazacu exponent
   * \param[in] k: Cazacu coefficient
   * \param[in] e: criterion used to check if the stress are null
   */
  template <typename StressStensor,
            typename CazacuExponentType,
            tfel::math::stensor_common::EigenSolver =
                tfel::math::stensor_common::TFELEIGENSOLVER>
  std::tuple<CazacuStressType<StressStensor>,
             CazacuStressNormalType<StressStensor>>
  computeCazacu2006IsotropicStressNormal(const StressStensor&,
                                         const CazacuExponentType,
                                         const CazacuBaseType<StressTensor>,
                                         const CazacuStressType<StressStensor>);
  /*!
   * \brief compute the Cazacu yield stress and its first and second derivatives
   * \tparam StressStensor: type of the stress tensor
   * \tparam CazacuExponentType: type of the hosford exponent
   * (could be a numeric type or an integer type)
   * \tparam es: eigen solver to be used
   * \param[in] sig: stress tensor
   * \param[in] a: Cazacu exponent
   * \param[in] k: Cazacu coefficient
   * \param[in] e: criterion used to check if the stress are null
   */
  template <typename StressStensor,
            typename CazacuExponentType,
            tfel::math::stensor_common::EigenSolver =
                tfel::math::stensor_common::TFELEIGENSOLVER>
  std::tuple<CazacuStressType<StressStensor>,
             CazacuStressNormalType<StressStensor>,
             CazacuStressSecondDerivativeType<StressStensor>>
  computeCazacu2006IsotropicStressSecondDerivative(
      const StressStensor&,
      const CazacuExponentType,
      const CazacuBaseType<StressTensor>,
      const CazacuStressType<StressStensor>);

}  // end of namespace tfel::material

#include "TFEL/Material/Cazacu2006IsotropicYieldCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL_CAZACU2006ISOTROPICYIELDCRITERION_HXX */
