/*! 
 * \file  tvector.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 sept. 2011
 */

#ifndef _LIB_TFEL_MATH_FORWARD_TVECTOR_H_
#define _LIB_TFEL_MATH_FORWARD_TVECTOR_H_ 

namespace tfel
{

  namespace math
  {

    /*!
     * \class tvector.
     * \brief a class representing tiny vectors of fixed size.
     * \param unsigned short, the number of values hold by the
     * tvector.
     * \param typename T, the type hold by the tvector.
     */
    template<unsigned short N, typename T>
    class tvector;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_TVECTOR_H */

