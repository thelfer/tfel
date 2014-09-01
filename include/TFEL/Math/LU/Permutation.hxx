/*!
 * \file   include/TFEL/Math/LU/Permutation.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   01 Aug 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_PERMUTATION_H_
#define _LIB_TFEL_MATH_PERMUTATION_H_ 

#include<cstdlib>

#include"TFEL/Math/vector.hxx"

namespace tfel{

  namespace math{

    template<typename T>
    struct Permutation
      : protected vector<T>
    {
      
      /*!
       * default constructor
       */
      TFEL_MATH_INLINE
      Permutation();

      /*!
       * default constructor
       */
      TFEL_MATH_INLINE
      Permutation(const typename vector<T>::size_type);

      /*!
       * \brief set this permutation vector to be equal to the
       * identity
       */
      TFEL_MATH_INLINE
      void reset(void);

      TFEL_MATH_INLINE const T&
      operator[](const typename vector<T>::size_type) const;

      TFEL_MATH_INLINE const T&
      operator()(const typename vector<T>::size_type) const;
      
      TFEL_MATH_INLINE void
      swap(const typename vector<T>::size_type,
	   const typename vector<T>::size_type);

      TFEL_MATH_INLINE bool
      isIdentity() const;

      using vector<T>::size;
      using vector<T>::clear;

      /*!
       * \brief resize the permutation
       * This automatically calls the reset method
       */
      TFEL_MATH_INLINE
      void resize(const typename vector<T>::size_type);

    private:

      Permutation(const Permutation&);

      Permutation&
      operator=(const Permutation&);

      bool is_identity;

    };

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/LU/Permutation.ixx"

#endif /* _LIB_TFEL_PERMUTATION_H */

