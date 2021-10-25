/*!
 * \file  include/TFEL/Math/Forward/VectorConcept.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 12 janv. 2012
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_VECTORCONCEPT_HXX
#define LIB_TFEL_MATH_FORWARD_VECTORCONCEPT_HXX

namespace tfel {

  namespace math {

    template <class T>
    struct VectorConcept;

    template <unsigned short N, unsigned short M, typename A, typename B>
    struct TMatrixTVectorExpr;

    template <unsigned short N, unsigned short M, typename A, typename B>
    struct TVectorTMatrixExpr;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_FORWARD_VECTORCONCEPT_HXX */
