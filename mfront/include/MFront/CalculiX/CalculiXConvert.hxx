/*!
 * \file   CalculiXConvert.hxx
 * \brief
 * \author Thomas Helfer
 * \date   22 mars 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CALCULIX_CALCULIXCONVERT_HXX
#define LIB_MFRONT_CALCULIX_CALCULIXCONVERT_HXX

#include "TFEL/FSAlgorithm/copy.hxx"
#include "TFEL/Math/General/MathConstants.hxx"

namespace calculix {

  /*!
   * \brief class defining the convertion from calculix to mfront for
   * driving variables
   */
  struct ImportDrivingVariables {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(tfel::math::stensor<3, T>& e,
                           const CalculiXReal* const v) {
      tfel::fsalgo::copy<6u>::exe(v, e.begin());
    }  // end of exe
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(tfel::math::tensor<3u, T>& F,
                           const CalculiXReal* const v) {
      tfel::fsalgo::copy<9u>::exe(v, F.begin());
    }  // end of exe
  };   // end of struct ImportDrivingVariables
  /*!
   * \brief class defining the convertion from calculix to mfront for
   * thermodynamic forces
   */
  struct ImportThermodynamicForces {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(tfel::math::stensor<3u, T>& s,
                           const CalculiXReal* const v) {
      tfel::fsalgo::copy<6u>::exe(v, s.begin());
    }  // end of exe
  };   // end of struct ImportThermodynamicForces
  /*!
   * \brief class defining the convertion from mfront to calculix for
   * thermodynamic forces
   */
  struct ExportThermodynamicForces {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] v: values
     * \param[in]  s: symmetric tensor to be exported
     */
    template <typename T>
    static inline void exe(CalculiXReal* const v,
                           const tfel::math::stensor<3u, T>& s) {
      tfel::fsalgo::copy<6u>::exe(s.begin(), v);
    }  // end of exe
  };   // end of struct ExportThermodynamicForces

}  // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIX_CALCULIXCONVERT_HXX */
