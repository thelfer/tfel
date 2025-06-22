/*!
 * \file   include/TFEL/Math/Tensor/TensorView.hxx
 * \author Thomas Helfer
 * \date   16 July 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSOR_TENSORVIEW_HXX
#define LIB_TFEL_MATH_TENSOR_TENSORVIEW_HXX

#include "TFEL/Metaprogramming/StaticAssert.hxx"

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel {

  namespace math {

    /*!
     * \brief tensor view expression
     * \tparam N: space dimension
     * \tparam T: value type
     */
    template <unsigned short N, typename T>
    struct TensorViewExpr {
    };  // end of struct TensorFromTinyMatrixColumnViewExpr

    /*!
     * \brief tensor view
     * \tparam N: space dimension
     * \tparam T: value type
     */
    template <unsigned short N, typename T>
    struct Expr<tensor<N, T>, TensorViewExpr<N, T>>
        : public TensorConcept<Expr<tensor<N, T>, TensorViewExpr<N, T>>>,
          public tensor_base<Expr<tensor<N, T>, TensorViewExpr<N, T>>> {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename tensor<N, T>::value_type value_type;
      typedef typename tensor<N, T>::pointer pointer;
      typedef typename tensor<N, T>::const_pointer const_pointer;
      typedef typename tensor<N, T>::reference reference;
      typedef typename tensor<N, T>::const_reference const_reference;
      typedef typename tensor<N, T>::size_type size_type;
      typedef typename tensor<N, T>::difference_type difference_type;

      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }

      /*!
       * \param[in] v : v_ values
       */
      explicit Expr(T* const v_) noexcept : v(v_) {}  // end of Expr

      Expr(Expr&&) noexcept = default;

      Expr(const Expr&) noexcept = default;

      const T& operator()(const unsigned short i) const noexcept {
        return this->v[i];
      }  // end of operator() const

      T& operator()(const unsigned short i) noexcept {
        return this->v[i];
      }  // end of operator()

      const T& operator[](const unsigned short i) const noexcept {
        return this->v[i];
      }  // end of operator[] const

      T& operator[](const unsigned short i) noexcept {
        return this->v[i];
      }  // end of operator[]

      //! using tensor_base::operator=
      using tensor_base<Expr>::operator=;
      //! assignement operator
      Expr& operator=(const Expr& src) {
        tensor_base<Expr>::operator=(src);
        return *this;
      }

      /*!
       * size of the symmetric tensor
       * (compatibility with vector)
       */
      constexpr TFEL_MATH_INLINE size_type size() const noexcept {
        return TensorDimeToSize<N>::value;
      }

     protected:
      T* const v;

     private:
      Expr() = delete;
      //! simple check
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));

    };  // end of struct Expr

    template <unsigned short N, typename T = double>
    using TensorView = Expr<tensor<N, T>, TensorViewExpr<N, T>>;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TENSOR_TENSORVIEW_HXX */
