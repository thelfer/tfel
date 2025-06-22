/*!
 * \file   include/TFEL/Math/Broyden2.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   05 avr 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_BROYDEN2_HXX_
#define LIB_TFEL_BROYDEN2_HXX_

#include <utility>

#include "TFEL/Math/tvector.hxx"

namespace tfel {

  namespace math {

    /*!
     * Find the root of a function using the second Broyden algorithm
     *
     * \param N  : size of the unknown
     * \param T  : numerical type
     * \param f  : function which root is sought after
     * \param x0 : initial guess
     * \param A  : initial approximation of the jacobian
     * \param e  : convergence criterium
     * \param n  : maximum number of iterations
     *
     * \return a pair whose first member is true is the algorithm
     * converged and whose second member contains the solution
     */
    template <unsigned short N,
              typename T,
              const tvector<N, T> (*f)(const tvector<N, T>&)>
    const std::pair<bool, tvector<N, T>> broyden2(const tvector<N, T>&,
                                                  tmatrix<N, N, T>&,
                                                  const T,
                                                  const unsigned short);

    /*!
     * Find the root of a function using the first Broyden algorithm.
     * The jacobian is initially approximated by the identity matrix.
     *
     * \param N  : size of the unknown
     * \param T  : numerical type
     * \param f  : function which root is sought after
     * \param x0 : initial guess
     * \param e  : convergence criterium
     * \param n  : maximum number of iterations
     *
     * \return a pair whose first member is true is the algorithm
     * converged and whose second member contains the solution
     */
    template <unsigned short N,
              typename T,
              const tvector<N, T> (*f)(const tvector<N, T>&)>
    const std::pair<bool, tvector<N, T>> broyden2(const tvector<N, T>&,
                                                  const T,
                                                  const unsigned short);

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/RootFinding/Broyden2.ixx"

#endif /* LIB_TFEL_BROYDEN2_HXX_ */
