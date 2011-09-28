/*! 
 * \file  tmatrix.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 sept. 2011
 */

#ifndef _LIB_TFEL_MATH_FORWARD_TMATRIX_H_
#define _LIB_TFEL_MATH_FORWARD_TMATRIX_H_ 

namespace tfel
{

  namespace math
  {

    /*!
     * \class tmatrix.
     * \brief a class representing tiny matrixs of fixed size.
     * \param unsigned short, the number of rows hold by the tmatrix.
     * \param unsigned short, the number of columns hold by the tmatrix.
     * \param typename T, the type hold by the tmatrix.
     */
    template<unsigned short N,
	     unsigned short M,
	     typename T>
    class tmatrix;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_TMATRIX_H */

