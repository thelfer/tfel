/*! 
 * \file  tensor.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 04 juil. 2013
 */

#ifndef _LIB_TFEL_MATH_FORWARD_TENSOR_H_
#define _LIB_TFEL_MATH_FORWARD_TENSOR_H_ 

namespace tfel
{

  namespace math
  {

    /*
     * \class tensor
     * \brief finite size symmetric tensor.
     * \param N, the spatial dimension, see TensorDimeToSize for details. 
     * \param T, numerical type used, by default, double
     * \pre   This class is only defined for N=1u,2u and 3u.
     * \see   TensorDimeToSize and TensorSizeToDime. 
     */
    template<unsigned short N,typename T=double>
    class tensor;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_TENSOR_H */

