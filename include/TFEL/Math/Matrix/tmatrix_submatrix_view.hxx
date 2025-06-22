/*!
 * \file  include/TFEL/Math/Matrix/tmatrix_submatrix_view.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 27 déc. 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TMATRIX_SUBMATRIX_VIEW_HXX
#define LIB_TFEL_MATH_TMATRIX_SUBMATRIX_VIEW_HXX

namespace tfel {

  namespace math {

    /*!
     * submatrix view
     * \param[in] N : number of rows of the underlying matrix
     * \param[in] M : number of columns of the underlying matrix
     * \param[in] I : starting row
     * \param[in] J : starting column
     * \param[in] R : number of rows    of the submatrix
     * \param[in] C : number of columns of the submatrix
     */
    template <unsigned short N,
              unsigned short M,
              unsigned short I,
              unsigned short J,
              unsigned short R,
              unsigned short C,
              typename T>
    struct tmatrix_submatrix_view_expr {
      //! a simple typedef to the tmatrix runtime properties
      /*
       * This is a MatrixConcept requirement.
       */
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * type of the tmatrix's values.
       * (this is a MatrixConcept requirement).
       */
      typedef T NumType;
      /*!
       * type of the tmatrix's values.
       * (this is a stl requirement).
       */
      typedef T value_type;
      /*!
       * type of a pointer to the value contained.
       * (this is a stl requirement).
       */
      typedef value_type* pointer;
      /*!
       * type of a const pointer to the value contained.
       * (this is a stl requirement).
       */
      typedef const value_type* const_pointer;
      /*!
       * type of a reference to the value contained.
       * (this is a stl requirement).
       */
      typedef value_type& reference;
      /*!
       * type of a const reference to the value contained.
       * (this is a stl requirement).
       */
      typedef const value_type& const_reference;
      /*!
       * type of the size of the container.
       * (this is a stl requirement).
       */
      typedef unsigned short size_type;
      /*!
       * type of the difference between two iterators.
       * (this is a stl requirement).
       */
      typedef ptrdiff_t difference_type;
      /*!
       * constructor
       * \param[in] m_ : the underlying matrix
       */
      TFEL_MATH_INLINE
      tmatrix_submatrix_view_expr(tmatrix<N, M, T>&);
      //! Return the RunTimeProperties of the tmatrix.
      /*
       * This is a MatrixConcept requirement.
       * \return const RunTimeProperties, the runtime properties of
       * the tmatrix
       */
      TFEL_MATH_INLINE
      RunTimeProperties getRunTimeProperties() const;
      /*!
       * \brief index operator.
       * This is a matrix concept requirement.
       * \param[in] i : index.
       * \return a reference to the ith element of the submatrix.
       */
      TFEL_MATH_INLINE
      const T& operator()(const unsigned short, const unsigned short) const;
      /*!
       * \brief index operator.
       * \param[in] i : index.
       * \return a reference to the ith element of the submatrix.
       */
      TFEL_MATH_INLINE
      T& operator()(const unsigned short, const unsigned short);

     private:
      //! reference to the underlying matrix
      tmatrix<N, M, T>& m;
    };

    template <unsigned short N,
              unsigned short M,
              unsigned short I,
              unsigned short J,
              unsigned short R,
              unsigned short C,
              typename T>
    struct tmatrix_submatrix_view
        : public Expr<tmatrix<R, C, T>,
                      tmatrix_submatrix_view_expr<N, M, I, J, R, C, T>>,
          public tmatrix_base<tmatrix_submatrix_view<N, M, I, J, R, C, T>,
                              R,
                              C,
                              T> {
      /*!
       * constructor
       * \param[in] m_ : the underlying matrix
       */
      TFEL_MATH_INLINE
      tmatrix_submatrix_view(tmatrix<N, M, T>&);
      //! using tmatrix::operator=
      using tmatrix_base<tmatrix_submatrix_view, R, C, T>::operator=;
      //! using tmatrix::operator+=
      using tmatrix_base<tmatrix_submatrix_view, R, C, T>::operator+=;
      //! using tmatrix::operator-=
      using tmatrix_base<tmatrix_submatrix_view, R, C, T>::operator-=;
      //! using tmatrix::operator*=
      using tmatrix_base<tmatrix_submatrix_view, R, C, T>::operator*=;
      //! using tmatrix::operator/=
      using tmatrix_base<tmatrix_submatrix_view, R, C, T>::operator/=;
    };

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Matrix/tmatrix_submatrix_view.ixx"

#endif /* LIB_TFEL_MATH_TMATRIX_SUBMATRIX_VIEW_HXX */
