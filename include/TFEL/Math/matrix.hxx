/*!
 * \file  include/TFEL/Math/matrix.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_MATRIX_HXX
#define LIB_TFEL_MATH_MATRIX_HXX

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"
#include "TFEL/Math/Forward/matrix.hxx"

#ifdef NO_RUNTIME_CHECK_BOUNDS
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#endif /* LIB_TFEL_MATH_MATRIX_HXX */

namespace tfel {

  namespace math {

    template <typename T>
    struct MatrixProperties;

    //! Partial specialisation for matrixs.
    /*
     * This is a MatrixConcept requirement.
     * \see MatrixTraits.
     */
    template <typename T>
    struct TFEL_VISIBILITY_LOCAL MatrixTraits<matrix<T>> {
      //! the type holded by the tmatrix.
      typedef typename tfel::math::vector<T>::value_type NumType;
      //! the type of the index used by the tmatrix.
      typedef typename tfel::math::vector<T>::size_type IndexType;
      /*!
       * RunTime Properties
       */
#ifdef NO_RUNTIME_CHECK_BOUNDS
      typedef EmptyRunTimeProperties RunTimeProperties;
#else  /* NO_RUNTIME_CHECK_BOUNDS */
      typedef MatrixProperties<T> RunTimeProperties;
#endif /* LIB_TFEL_MATH_MATRIX_HXX */
    };

    template <typename T>
    struct TFEL_VISIBILITY_LOCAL MatrixProperties {
      //! a simple alias
      typedef typename MatrixTraits<matrix<T>>::IndexType IndexType;

      MatrixProperties(const IndexType, const IndexType);

      MatrixProperties(const MatrixProperties&);

      MatrixProperties& operator=(const MatrixProperties&);

      bool operator==(const MatrixProperties&) const;

      bool operator!=(const MatrixProperties&) const;

     protected:
      typename MatrixTraits<matrix<T>>::IndexType nb_rows;

      typename MatrixTraits<matrix<T>>::IndexType nb_cols;

    };  // end of MatrixProperties

    template <typename T>
    struct TFEL_VISIBILITY_LOCAL matrix : protected tfel::math::vector<T>,
                                          protected MatrixProperties<T>,
                                          public MatrixConcept<matrix<T>> {
      //! a simple alias
      using ConceptTag = typename MatrixConcept<matrix<T>>::ConceptTag;
      //! a simple alias
#ifdef NO_RUNTIME_CHECK_BOUNDS
      typedef EmptyRunTimeProperties RunTimeProperties;
#else  /* NO_RUNTIME_CHECK_BOUNDS */
      typedef MatrixProperties<T> RunTimeProperties;
#endif /* LIB_TFEL_MATH_MATRIX_HXX */
      /*!
       * type of the matrix's values.
       * (this i<s a stl requirement).
       */
      using typename tfel::math::vector<T>::value_type;
      /*!
       * type of a pointer to the value contained.
       * (this is a stl requirement).
       */
      using typename tfel::math::vector<T>::pointer;
      /*!
       * type of a const pointer to the value contained.
       * (this is a stl requirement).
       */
      using typename tfel::math::vector<T>::const_pointer;
      /*!
       * type of the matrix's iterator.
       * (provided for stl compatibility).
       */
      using typename tfel::math::vector<T>::iterator;
      /*!
       * type of the matrix's const iterator.
       * (provided for stl compatibility).
       */
      using typename tfel::math::vector<T>::const_iterator;
      /*!
       * type of the matrix's reverse iterator.
       * (provided for stl compatibility).
       */
      using typename tfel::math::vector<T>::const_reverse_iterator;
      /*!
       * type of the matrix's const reverse iterator.
       * (provided for stl compatibility).
       */
      using typename tfel::math::vector<T>::reverse_iterator;
      /*!
       * type of a reference to the value contained.
       * (this is a stl requirement).
       */
      using typename tfel::math::vector<T>::reference;
      /*!
       * type of a const reference to the value contained.
       * (this is a stl requirement).
       */
      using typename tfel::math::vector<T>::const_reference;
      /*!
       * type of the size of the container.
       * (this is a stl requirement).
       */
      typedef typename tfel::math::vector<T>::size_type size_type;
      /*!
       * type of the difference between two iterators.
       * (this is a stl requirement).
       */
      using typename tfel::math::vector<T>::difference_type;

      TFEL_MATH_INLINE2
      matrix();

      TFEL_MATH_INLINE2
      matrix(const size_type, const size_type);

      TFEL_MATH_INLINE2
      matrix(const size_type, const size_type, const T&);

      TFEL_MATH_INLINE2
      matrix(const matrix&);

      /*!
       * resize the matrix
       * \param[in] n : number of rows
       * \param[in] m : number of columns
       * \param[in] v : values of the newly inserted elements
       */
      void resize(const size_type, const size_type, const T& = T());

      /*!
       * clear the matrix
       */
      void clear();

      TFEL_MATH_INLINE T& operator()(const size_type, const size_type);

      TFEL_MATH_INLINE const T& operator()(size_type, size_type) const;

#ifdef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE const RunTimeProperties
#else
      TFEL_MATH_INLINE const RunTimeProperties&
#endif /* LIB_TFEL_MATH_MATRIX_HXX */
      getRunTimeProperties() const;

      /*
       * return an iterator to the first element of the matrix
       * (provided for stl compatibility)
       * \return iterator, an iterator to the first element
       */
      using tfel::math::vector<T>::begin;

      /*
       * return an iterator after the last element of the matrix
       * (provided for stl compatibility)
       * \return iterator, an iterator after the last element
       */
      using tfel::math::vector<T>::end;

      /*
       * return an reverse iterator to the last element of the matrix
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator to the last element
       */
      using tfel::math::vector<T>::rbegin;

      /*
       * return an  reverse iterator before the first element of the matrix
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator before the first element
       */
      using tfel::math::vector<T>::rend;

      /*
       * Assignement operator.
       * \param  const matrix&, the matrix to be copied.
       * \return matrix&, a reference to itself.
       */
      matrix& operator=(const matrix&);

      /*
       * Assignement operator.
       * \param  const matrix&, the matrix to be copied.
       * \return matrix&, a reference to itself.
       */
      matrix& operator+=(const matrix&);

      /*
       * Assignement operator.
       * \param  const matrix&, the matrix to be copied.
       * \return matrix&, a reference to itself.
       */
      matrix& operator-=(const matrix&);

      /*
       * Assignement operator
       * \param const expr : a matrix
       * expression based on matrix
       * \return matrix&, a reference to itself.
       */
      template <typename T2, typename Operation>
      TFEL_MATH_INLINE2
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  matrix<T>&>::type
          operator=(const Expr<matrix<T2>, Operation>&);

      /*
       * Assignement operator
       * \param const expr : a matrix
       * expression based on matrix
       * \return matrix&, a reference to itself.
       */
      template <typename T2, typename Operation>
      TFEL_MATH_INLINE2
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  matrix<T>&>::type
          operator+=(const Expr<matrix<T2>, Operation>&);

      /*
       * Assignement operator
       * \param const expr : a matrix
       * expression based on matrix
       * \return matrix&, a reference to itself.
       */
      template <typename T2, typename Operation>
      TFEL_MATH_INLINE2
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  matrix<T>&>::type
          operator-=(const Expr<matrix<T2>, Operation>&);

      void swap(matrix&);

      TFEL_MATH_INLINE size_type getNbRows() const;

      TFEL_MATH_INLINE size_type getNbCols() const;
    };

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Matrix/matrix.ixx"

#endif /* LIB_TFEL_MATH_MATRIX_HXX */
