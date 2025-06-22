/*!
 * \file  include/TFEL/Math/Forward/tmatrix.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 19 sept. 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_TMATRIX_HXX
#define LIB_TFEL_MATH_FORWARD_TMATRIX_HXX

#include "TFEL/TypeTraits/BaseType.hxx"

namespace tfel {

  namespace math {

    /*
     * \class tmatrix.
     * \brief a class representing tiny matrixs of fixed size.
     * \param unsigned short, the number of rows hold by the tmatrix.
     * \param unsigned short, the number of columns hold by the tmatrix.
     * \param typename T, the type hold by the tmatrix.
     */
    template <unsigned short N, unsigned short M, typename T>
    struct tmatrix;

    //! a simple alias
    template <typename T>
    using rotation_matrix = tmatrix<3, 3, tfel::typetraits::base_type<T>>;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_FORWARD_TMATRIX_HXX */
