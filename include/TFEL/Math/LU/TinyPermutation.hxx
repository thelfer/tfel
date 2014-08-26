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
    struct TinyPermutation
      : protected tvector<N,unsigned short>
    {
      
      TFEL_MATH_INLINE TinyPermutation();

      using tvector<N,unsigned short>::size;
      
      template<typename T>
      TFEL_MATH_INLINE void exe(tvector<N,T>&) const;

      TFEL_MATH_INLINE const unsigned short&
      operator[](const unsigned short) const;

      TFEL_MATH_INLINE const unsigned short&
      operator()(const unsigned short) const;
      
      TFEL_MATH_INLINE void
      swap(const unsigned short,
	   const unsigned short);

      TFEL_MATH_INLINE bool
      isIdentity() const;
      
    protected:

      TFEL_MATH_INLINE TinyPermutation(const TinyPermutation&);
      
      TFEL_MATH_INLINE TinyPermutation&
      operator=(const TinyPermutation&);

      bool is_identity;

    };

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/LU/TinyPermutation.ixx"

#endif /* _LIB_TFEL_TINYPERMUTATION_H */

