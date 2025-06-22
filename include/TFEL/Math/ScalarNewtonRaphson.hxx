/*!
 * \file   include/TFEL/Math/NewtonRaphson.hxx
 * \brief
 * \author Thomas Helfer
 * \date   31/03/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_SCALARNEWTONRAPHSON_HXX
#define LIB_TFEL_MATH_SCALARNEWTONRAPHSON_HXX

#include <tuple>
#include <limits>

namespace tfel::math {

  /*!
   * \brief structure holding parameters of the `scalarNewtonRaphson`
   * function.
   * \tparam NumericType: type of the root
   * \tparam IndexType: type of the iteration counter
   */
  template <typename NumericType, typename IndexType>
  struct ScalarNewtonRaphsonParameters {
    //! \brief initial guess
    NumericType x0 = NumericType{};
    //! \brief maximum number of iterations
    IndexType im = IndexType{100};
    //! \brief initial guess of the lower bound of the solution
    NumericType xmin0 = std::numeric_limits<NumericType>::quiet_NaN();
    //! \brief initial guess of the upper bound of the solution
    NumericType xmax0 = std::numeric_limits<NumericType>::quiet_NaN();
  };  // end of struct ScalarNewtonRaphsonParameters

  /*!
   * \brief find the root of the given function.
   * \tparam Function: type of a generalised function returning the
   * value of the objective function and its derivative
   * \tparam Criterion: type of a function which computes the stopping
   * criterion
   * \tparam NumericType: type of the root
   * \tparam IndexType: type of the iteration counter
   * \param[in] f: generalised function
   * \param[in] c: criterion
   * \param[in] x0: initial guess
   * \param[in] im: maximum number of iterations
   * \return a tuple whose first field is a boolean giving the convergence
   * status and the second field the last estimate of the solution.
   */
  template <typename Function,
            typename Criterion,
            typename NumericType,
            typename IndexType>
  std::tuple<bool, NumericType, IndexType> scalarNewtonRaphson(
      const Function&, const Criterion&, const NumericType, const IndexType);

  /*!
   * \brief find the root of the given function.
   * \tparam Function: type of a generalised function returning the
   * value of the objective function and its derivative
   * \tparam Criterion: type of a function which computes the stopping
   * criterion
   * \tparam NumericType: type of the root
   * \tparam IndexType: type of the iteration counter
   * \param[in] f: generalised function
   * \param[in] c: criterion
   * \param[in] p: newton parameters
   * \return a tuple whose first field is a boolean giving the convergence
   * status and the second field the last estimate of the solution.
   */
  template <typename Function,
            typename Criterion,
            typename NumericType,
            typename IndexType>
  std::tuple<bool, NumericType, IndexType> scalarNewtonRaphson(
      const Function&,
      const Criterion&,
      const ScalarNewtonRaphsonParameters<NumericType, IndexType>&);

}  // end of namespace tfel::math

#include "TFEL/Math/NonLinearSolvers/ScalarNewtonRaphson.ixx"

#endif /* LIB_TFEL_MATH_SCALARNEWTONRAPHSON_HXX */
