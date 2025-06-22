/*!
 * \file   include/TFEL/Material/ChuNeedleman1980StressBasedNucleationModel.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04/04/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRESSBASEDNUCLEATIONMODEL_HXX
#define LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRESSBASEDNUCLEATIONMODEL_HXX

#include <tuple>
#include <ostream>
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/stensor.hxx"

namespace tfel::material {

  //! \brief a simple alias
  template <typename StressStensorType>
  using ChuNeedleman1980StressBasedNucleationModelStressType =
      tfel::math::numeric_type<StressStensorType>;
  //! \brief a simple alias
  template <typename StressStensorType>
  using ChuNeedleman1980StressBasedNucleationModelRealType =
      tfel::typetraits::base_type<tfel::math::numeric_type<StressStensorType>>;
  //! \brief a simple alias
  template <typename StressStensorType>
  using ChuNeedleman1980StressBasedNucleationModelInverseStressType =
      tfel::math::result_type<
          ChuNeedleman1980StressBasedNucleationModelRealType<StressStensorType>,
          ChuNeedleman1980StressBasedNucleationModelStressType<
              StressStensorType>,
          tfel::math::OpDiv>;
  //! \brief a simple alias
  template <typename StressStensorType>
  using ChuNeedleman1980StressBasedNucleationModeDerivativeType =
      tfel::math::stensor<
          tfel::math::getSpaceDimension<StressStensorType>(),
          ChuNeedleman1980StressBasedNucleationModelInverseStressType<
              StressStensorType>>;

  /*!
   * \brief parameters of the strain version of the nucleation model
   * proposed by Chu and Needleman.
   *
   * Chu, C. C., and A. Needleman. 1980.
   * “Void Nucleation Effects in Biaxially Stretched Sheets.”
   * Journal of Engineering Materials and Technology 102 (3): 249–56
   */
  template <typename StressStensorType>
  struct ChuNeedleman1980StressBasedNucleationModelParameters {
    //! \brief factor in front of the gaussian distribution
    ChuNeedleman1980StressBasedNucleationModelStressType<StressStensorType> fn;
    //! \brief mean value of the gaussian distribution
    ChuNeedleman1980StressBasedNucleationModelStressType<StressStensorType>
        sigm;
    //! \brief standard deviation of the gaussian distribution
    ChuNeedleman1980StressBasedNucleationModelStressType<StressStensorType> sn;
  };  // end of struct ChuNeedleman1980StressBasedNucleationModelParameters

  /*!
   * \brief stream operator
   * \return the output stream
   * \tparam real: numeric type
   * \tparam use_qt: flag stating if quantities are used
   * \param[in] os: output stream
   * \param[in] p: parameters
   */
  template <typename StressStensorType>
  std::ostream& operator<<(
      std::ostream&,
      const ChuNeedleman1980StressBasedNucleationModelParameters<
          StressStensorType>&);
  /*!
   * \brief computes the porosity rate factor \f$A_{n}\f$ as defined by
   * the nucleation model of Chu and Needleman.
   *
   * \f[
   * A_{n} = \frac{f_N}{s_N \sqrt{2\pi}}
   *         \exp\left(-\frac{1}{2}
   *                    \left(\frac{\sigma_{I} -
   * \epsilon_N}{s_N}\right)^2\right) \f]
   *
   * Chu, C. C., and A. Needleman. 1980.
   * “Void Nucleation Effects in Biaxially Stretched Sheets.”
   * Journal of Engineering Materials and Technology 102 (3): 249–56
   *
   * \param[in] params: parameters
   * \param[in] sig: stress tensor
   * \param[in] p: equivalent plastic strain (unused)
   */
  template <typename StressStensorType>
  TFEL_HOST_DEVICE
      ChuNeedleman1980StressBasedNucleationModelRealType<StressStensorType>
      computeChuNeedleman1980StressBasedNucleationModelPorosityRateFactor(
          const ChuNeedleman1980StressBasedNucleationModelParameters<
              StressStensorType>&,
          const StressStensorType&,
          const ChuNeedleman1980StressBasedNucleationModelRealType<
              StressStensorType>&);

  /*!
   * \brief computes the porosity rate factor \f$A_{n}\f$ as defined by
   * the nucleation model of Chu and Needleman and its derivative
   * \f$\frac{\partial A_{n}}{\partial \underline{\sigma}}\f$ with respect to
   * the stress.
   *
   * \f[
   * A_{n} = \frac{f_N}{s_N \sqrt{2\pi}}
   *         \exp\left(-\frac{1}{2}
   *                    \left(\frac{\sigma_{I} -
   * \epsilon_N}{s_N}\right)^2\right) \f]
   *
   * Chu, C. C., and A. Needleman. 1980.
   * “Void Nucleation Effects in Biaxially Stretched Sheets.”
   * Journal of Engineering Materials and Technology 102 (3): 249–56
   *
   * \param[in] params: parameters
   * \param[in] sig: stress tensor
   * \param[in] p: equivalent plastic strain (unused)
   */
  template <typename StressStensorType>
  TFEL_HOST_DEVICE std::tuple<
      ChuNeedleman1980StressBasedNucleationModelRealType<StressStensorType>,
      ChuNeedleman1980StressBasedNucleationModeDerivativeType<
          StressStensorType>>
  computeChuNeedleman1980StressBasedNucleationModelPorosityRateFactorAndDerivative(
      const ChuNeedleman1980StressBasedNucleationModelParameters<
          StressStensorType>&,
      const StressStensorType&,
      const ChuNeedleman1980StressBasedNucleationModelRealType<
          StressStensorType>&);

}  // end of namespace tfel::material

#include "TFEL/Material/ChuNeedleman1980StressBasedNucleationModel.ixx"

#endif /* LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRESSBASEDNUCLEATIONMODEL_HXX */
