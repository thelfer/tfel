/*!
 * \file   include/TFEL/Math/NewtonRaphson.hxx
 * \brief
 * \author Thomas Helfer
 * \date   31/03/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_SCALARNEWTONRAPHSON_HXX
#define LIB_TFEL_MATH_SCALARNEWTONRAPHSON_HXX

#include <tuple>

namespace tfel {

  namespace math {

    /*!
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

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Newton-Raphson/ScalarNewtonRaphson.ixx"

#endif /* LIB_TFEL_MATH_SCALARNEWTONRAPHSON_HXX */
