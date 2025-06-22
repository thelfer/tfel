/*!
 * \file
 * include/TFEL/Math/AccelerationAlgorithms/AitkenAccelerationAlgorithm.hxx
 * \brief
 * \author Thomas Helfer
 * \date   28/05/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ACCELERATIONALGORITHMS_AITKENACCELERATIONALGORITHM_HXX
#define LIB_TFEL_MATH_ACCELERATIONALGORITHMS_AITKENACCELERATIONALGORITHM_HXX

#include <ostream>

namespace tfel {

  namespace math {

    /*!
     * \brief an implementation of the Aitken \f$\Delta^{2}\f$ algorithm
     * \tparam NumericType: numeric type used
     * \tparam IndexType: integer used to store the iteration number
     */
    template <typename NumericType, typename IndexType = unsigned int>
    struct AitkenAccelerationAlgorithm {
      /*!
       * \brief this method shall be called before starting a new resolution
       * \param[in] x0: initial guess
       */
      void initialize(const NumericType);
      /*!
       * \brief perform the acceleration
       * \param[in,out] x: on input, the result of the Picard iteration. On
       * output, an accelerated estimate.
       */
      void accelerate(NumericType&);

     private:
      //! \brief previous iterate
      NumericType x0;
      //! \brief previous iterate
      NumericType x1;
      //! \brief iteration number
      IndexType i = 0;
    };  // end of struct AitkenAccelerationAlgorithm

    /*!
     * \brief output stream operator
     * \param[in] os: output operator
     * \param[in] a: algorithm
     */
    template <typename NumericType, typename IndexType>
    std::ostream& operator<<(
        std::ostream&,
        const AitkenAccelerationAlgorithm<NumericType, IndexType>&);

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/AccelerationAlgorithms/AitkenAccelerationAlgorithm.ixx"

#endif /* LIB_TFEL_MATH_ACCELERATIONALGORITHMS_AITKENACCELERATIONALGORITHM_HXX \
        */
