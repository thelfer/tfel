/*!
 * \file   TinyPermutation.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   01 Aug 2006
 */

#ifndef _LIB_TFEL_MATH_TINYPERMUTATION_H_
#define _LIB_TFEL_MATH_TINYPERMUTATION_H_ 

#include"TFEL/Math/tvector.hxx"

namespace tfel{

  namespace math{

    template<unsigned short N>
    class TinyPermutation
      : public tvector<N,unsigned short>
    {
      TinyPermutation(const TinyPermutation&);
      TinyPermutation& operator=(const TinyPermutation&);

    public:
      
      TFEL_MATH_INLINE TinyPermutation();

      template<typename T>
      TFEL_MATH_INLINE void exe(tvector<N,T>&) const;
      
    };

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/LU/TinyPermutation.ixx"

#endif /* _LIB_TFEL_TINYPERMUTATION_H */

