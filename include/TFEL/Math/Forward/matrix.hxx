/*! 
 * \file  include/TFEL/Math/Forward/matrix.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 sept. 2011
 */

#ifndef _LIB_TFEL_MATH_FORWARD_MATRIX_H_
#define _LIB_TFEL_MATH_FORWARD_MATRIX_H_ 

namespace tfel
{

  namespace math
  {

    /*
     * \class matrix.
     * \brief a class representing tiny matrixs of fixed size.
     * \param unsigned short, the number of rows hold by the matrix.
     * \param unsigned short, the number of columns hold by the matrix.
     * \param typename T, the type hold by the matrix.
     */
    template<typename T>
    struct matrix;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_MATRIX_H */

