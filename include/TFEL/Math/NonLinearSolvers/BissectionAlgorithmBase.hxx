/*!
 * \file   include/TFEL/Math/NonLinearSolvers/BissectionAlgorithmBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10/10/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_BISSECTIONALGORITHMBASE_HXX
#define LIB_TFEL_MATH_BISSECTIONALGORITHMBASE_HXX

#include <limits>
#include <ostream>

namespace tfel::math {

  /*!
   * \brief a data structure holoding the current state of the bissection
   * algorithm.
   * \tparam NumericType: numeric type used
   */
  template <typename NumericType>
  struct BissectionAlgorithmData {
    //! \brief lower bound of the root
    NumericType xmin = std::numeric_limits<NumericType>::quiet_NaN();
    /*!
     * \brief value of the function associated with the lower bound of the
     * root.
     */
    NumericType fmin = std::numeric_limits<NumericType>::quiet_NaN();
    //! \brief upper bound of the root
    NumericType xmax = std::numeric_limits<NumericType>::quiet_NaN();
    /*!
     * \brief value of the function associated with the upper bound of the
     * root.
     */
    NumericType fmax = std::numeric_limits<NumericType>::quiet_NaN();
  };  // end of struct BissectionAlgorithmData

  /*!
   * \brief a base class to algorithms that extends a scalar root finding
   * algorithm, such as a fixed-point iteration, Newton-Raphson, Stephensen,
   * etc.. with the bissection algorithm.
   *
   * The main method provided by the `BissectionAlgorithmBase` is the
   * `iterate` class which must be called after each iteration of the
   * root finding algorithm.
   *
   * \tparam NumericType: numeric type used
   */
  template <typename NumericType>
  struct BissectionAlgorithmBase
      : protected BissectionAlgorithmData<NumericType> {
    /*!
     * \param[in] x: current estimate of the root
     * \param[in] f: value of the function associated current estimate of the
     * root
     */
    void updateBounds(const NumericType&, const NumericType&);
    /*!
     * \brief apply the bissection algorithm to the next extimate of the root.
     * \param[in] x: current estimate of the root
     */
    void iterate(NumericType&);
    /*!
     * \brief get an estimation of the root of the function
     * \param[out] x: estimate of the root of the function
     * \return a boolean stating if an estimate of the root has been given.
     */
    bool getNextRootEstimate(NumericType&) const;
    /*!
     * \brief return the current state of the bissection algorithm
     */
    const BissectionAlgorithmData<NumericType>& getData() const;

   private:
    /*!
     * \return true if the given numbers have the same sign
     * \param[in] a: first number
     * \param[in] b: second number
     */
    static bool haveSameSign(const NumericType, const NumericType);

  };  // end of struct BissectionAlgorithmBase

  /*!
   * \brief output stream
   * \param[out] os: output stream
   * \param[out] d: data
   */
  template <typename NumericType>
  std::ostream& operator<<(std::ostream&,
                           const BissectionAlgorithmData<NumericType>&);
  /*!
   * \brief output stream
   * \param[out] os: output stream
   * \param[out] a: acceleration algorithm
   */
  template <typename NumericType>
  std::ostream& operator<<(std::ostream&,
                           const BissectionAlgorithmBase<NumericType>&);

}  // end of namespace tfel::math

#include "TFEL/Math/NonLinearSolvers/BissectionAlgorithmBase.ixx"

#endif /* LIB_TFEL_MATH_BISSECTIONALGORITHMBASE_HXX */
