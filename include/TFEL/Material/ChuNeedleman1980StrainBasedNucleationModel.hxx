/*!
 * \file   include/TFEL/Material/ChuNeedleman1980StrainBasedNucleationModel.hxx
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

#ifndef LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRAINBASEDNUCLEATIONMODEL_HXX
#define LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRAINBASEDNUCLEATIONMODEL_HXX

#include <tuple>
#include <ostream>

namespace tfel::material {

  /*!
   * \brief parameters of the strain version of the nucleation model
   * proposed by Chu and Needleman.
   *
   * Chu, C. C., and A. Needleman. 1980.
   * “Void Nucleation Effects in Biaxially Stretched Sheets.”
   * Journal of Engineering Materials and Technology 102 (3): 249–56
   *
   * \tparam real: numeric type
   */
  template <typename real>
  struct ChuNeedleman1980StrainBasedNucleationModelParameters {
    //! \brief factor in front of the gaussian distribution
    real fn;
    //! \brief mean value of the gaussian distribution
    real en;
    //! \brief standard deviation of the gaussian distribution
    real sn;
  };  // end of struct ChuNeedleman1980StrainBasedNucleationModelParameters

  /*!
   * \brief stream operator
   * \return the output stream
   * \param[in] os: output stream
   * \param[in] p: parameters
   */
  template <typename real>
  std::ostream& operator<<(
      std::ostream&,
      const ChuNeedleman1980StrainBasedNucleationModelParameters<real>&);

  /*!
   * \brief computes the porosity rate factor \f$A_{n}\f$ as defined by
   * the nucleation model of Chu and Needleman.
   *
   * \f[
   * A_{n} = \frac{f_N}{s_N \sqrt{2\pi}}
   *         \exp\left(-\frac{1}{2}
   *                    \left(\frac{p - \epsilon_N}{s_N}\right)^2\right)
   * \f]
   *
   * Chu, C. C., and A. Needleman. 1980.
   * “Void Nucleation Effects in Biaxially Stretched Sheets.”
   * Journal of Engineering Materials and Technology 102 (3): 249–56
   *
   * \tparam real: numeric type
   *
   * \param[in] params: parameters
   * \param[in] p: equivalent plastic strain
   */
  template <typename real>
  TFEL_HOST_DEVICE real
  computeChuNeedleman1980StrainBasedNucleationModelPorosityRateFactor(
      const ChuNeedleman1980StrainBasedNucleationModelParameters<real>&,
      const real);

  /*!
   * \brief computes the porosity rate factor \f$A_{n}\f$ as defined by
   * the nucleation model of Chu and Needleman and its derivative
   * \f$\frac{\partial A_{n}}{\partial p}\f$ with respect to the
   * equivalent plastic strain \f$p\f$.
   *
   * \f[
   * A_{n} = \frac{f_N}{s_N \sqrt{2\pi}}
   *         \exp\left(-\frac{1}{2}
   *                    \left(\frac{p - \epsilon_N}{s_N}\right)^2\right)
   * \f]
   *
   * Chu, C. C., and A. Needleman. 1980.
   * “Void Nucleation Effects in Biaxially Stretched Sheets.”
   * Journal of Engineering Materials and Technology 102 (3): 249–56
   *
   * \tparam real: numeric type
   *
   * \param[in] params: parameters
   * \param[in] p: equivalent plastic strain
   */
  template <typename real>
  TFEL_HOST_DEVICE std::tuple<real, real>
  computeChuNeedleman1980StrainBasedNucleationModelPorosityRateFactorAndDerivative(
      const ChuNeedleman1980StrainBasedNucleationModelParameters<real>&,
      const real);

  /*!
   * \brief computes the porosity increment \f$A_{n}\f$ as defined by
   * the nucleation model of Chu and Needleman.
   *
   * Chu, C. C., and A. Needleman. 1980.
   * “Void Nucleation Effects in Biaxially Stretched Sheets.”
   * Journal of Engineering Materials and Technology 102 (3): 249–56
   *
   * \tparam real: numeric type
   *
   * \param[in] params: parameters
   * \param[in] p: equivalent plastic strain
   */
  template <typename real>
  TFEL_HOST_DEVICE real
  computeChuNeedleman1980StrainBasedNucleationModelPorosityIncrement(
      const ChuNeedleman1980StrainBasedNucleationModelParameters<real>&,
      const real);

  /*!
   * \brief computes the porosity increment defined by
   * the nucleation model of Chu and Needleman and its derivative
   * with respect to the increment of the equivalent plastic strain \f$p\f$.
   *
   * Chu, C. C., and A. Needleman. 1980.
   * “Void Nucleation Effects in Biaxially Stretched Sheets.”
   * Journal of Engineering Materials and Technology 102 (3): 249–56
   *
   * \tparam real: numeric type
   *
   * \param[in] params: parameters
   * \param[in] p: equivalent plastic strain
   */
  template <typename real>
  TFEL_HOST_DEVICE std::tuple<real, real>
  computeChuNeedleman1980StrainBasedNucleationModelPorosityIncrementAndDerivative(
      const ChuNeedleman1980StrainBasedNucleationModelParameters<real>&,
      const real);

}  // end of namespace tfel::material

#include "TFEL/Material/ChuNeedleman1980StrainBasedNucleationModel.ixx"

#endif /* LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRAINBASEDNUCLEATIONMODEL_HXX */
