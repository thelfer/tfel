/*! 
 * \file  matrix.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 */

#ifndef _LIB_TFEL_MATH_MATRIX_H_
#define _LIB_TFEL_MATH_MATRIX_H_ 

#include"Math/vector.hxx"
#include"Math/Matrix/MatrixConcept.hxx"

namespace tfel
{

  namespace math
  {

    template<typename T>
    struct matrix;

    //! Partial specialisation for matrixs.
    /*
     * This is a MatrixConcept requirement.
     * \see MatrixTraits.
     */
    template<typename T>
    struct MatrixTraits<matrix<T> >{
      //! the type holded by the tmatrix.
      typedef typename tfel::math::vector<T>::value_type NumType;
      //! the type of the index used by the tmatrix.
      typedef typename tfel::math::vector<T>::size_type IndexType;
    };

    template<typename T>
    struct matrix
      : protected tfel::math::vector<T>
    {
      /*!
       * type of the matrix's values.
       * (this i<s a stl requirement).
       */
      using std::vector<T>::value_type;
      /*!
       * type of a pointer to the value contained.
       * (this is a stl requirement).
       */
      using std::vector<T>::pointer;
      /*!
       * type of a const pointer to the value contained.
       * (this is a stl requirement).
       */
      using std::vector<T>::const_pointer;
      /*!
       * type of the matrix's iterator.
       * (provided for stl compatibility).
       */
      using std::vector<T>::iterator;
      /*!
       * type of the matrix's const iterator.
       * (provided for stl compatibility).
       */
      using std::vector<T>::const_iterator;
      /*!
       * type of the matrix's reverse iterator.
       * (provided for stl compatibility).
       */
      using std::vector<T>::const_reverse_iterator;
      /*!
       * type of the matrix's const reverse iterator.
       * (provided for stl compatibility).
       */
      using std::vector<T>::reverse_iterator;
      /*!
       * type of a reference to the value contained.
       * (this is a stl requirement).
       */
      using std::vector<T>::reference;
      /*!
       * type of a const reference to the value contained.
       * (this is a stl requirement).
       */
      using std::vector<T>::const_reference;
      /*!
       * type of the size of the container.
       * (this is a stl requirement).
       */
      typedef typename std::vector<T>::size_type size_type;
      /*!
       * type of the difference between two iterators.
       * (this is a stl requirement).
       */
      using std::vector<T>::difference_type;

      matrix(size_type r,
	     size_type c, const T& v = T())
	: tfel::math::vector<T>(r*c,v),
	  nb_rows(r),
	  nb_cols(c)
      {}

      T&
      operator()(size_type i,
		 size_type j)
      {
	return std::vector<T>::operator[](i*(this->nb_cols)+j);
      }

      const T&
      operator()(size_type i,
		 size_type j) const
      {
	return std::vector<T>::operator[](i*(this->nb_cols)+j);
      }

      /*
       * return an iterator to the first element of the matrix
       * (provided for stl compatibility)
       * \return iterator, an iterator to the first element
       */
      using std::vector<T>::begin;
      
      /*
       * return an iterator after the last element of the matrix
       * (provided for stl compatibility)
       * \return iterator, an iterator after the last element
       */
      using std::vector<T>::end;

      /*
       * return an reverse iterator to the last element of the matrix
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator to the last element
       */
      using std::vector<T>::rbegin;
      
      /*
       * return an  reverse iterator before the first element of the matrix
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator before the first element
       */
      using std::vector<T>::rend;

      size_type
      getNbRows() const
      {
	return nb_rows;
      }

      size_type
      getNbCols() const
      {
	return nb_cols;
      }

    private:
      
      size_type nb_rows;
      size_type nb_cols;

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_MATRIX_H */

