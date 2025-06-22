/*!
 * \file  include/TFEL/Math/Forward/matrix.hxx
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

#ifndef LIB_TFEL_MATH_FORWARD_MATRIX_HXX
#define LIB_TFEL_MATH_FORWARD_MATRIX_HXX

namespace tfel {

  namespace math {

    /*
     * \class matrix.
     * \brief a class representing tiny matrixs of fixed size.
     * \param unsigned short, the number of rows hold by the matrix.
     * \param unsigned short, the number of columns hold by the matrix.
     * \param typename T, the type hold by the matrix.
     */
    template <typename T>
    struct matrix;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_FORWARD_MATRIX_HXX */
