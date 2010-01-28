/*!
 * \file   Permutation.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   01 Aug 2006
 */

#ifndef _LIB_TFEL_MATH_PERMUTATION_H_
#define _LIB_TFEL_MATH_PERMUTATION_H_ 

#include<cstdlib>

#include"TFEL/Math/vector.hxx"

namespace tfel{

  namespace math{

    template<typename T>
    struct Permutation
      : public vector<T>
    {
      
      TFEL_MATH_INLINE
      Permutation(const typename vector<T>::size_type);

    private:

      Permutation(const Permutation&);

      Permutation&
      operator=(const Permutation&);

    };

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/LU/Permutation.ixx"

#endif /* _LIB_TFEL_PERMUTATION_H */

