/*!
 * \file   include/TFEL/Math/Tensor/TensorView.hxx
 * \author Thomas Helfer
 * \date   16 July 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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
    template <unsigned short N, typename T, bool is_const>
    struct TensorViewExpr {
    };  // end of struct TensorFromTinyMatrixColumnViewExpr

    /*!
     * \brief tensor view
     * \tparam N: space dimension
     * \tparam T: value type
     */
    template <unsigned short N, typename T, bool is_const>
    struct Expr<tensor<N, T>, TensorViewExpr<N, T, is_const>>
        : public TensorConcept<
              Expr<tensor<N, T>, TensorViewExpr<N, T, is_const>>>,
          public tensor_base<
              Expr<tensor<N, T>, TensorViewExpr<N, T, is_const>>> {
      //! \brief a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! \brief a simple alias
      using value_type = typename std::conditional<is_const, const T, T>::type;
      //! \brief a simple alias
      using pointer = typename std::conditional<is_const, const T*, T*>::type;
      //! \brief a simple alias
      using const_pointer = const T*;
      //! \brief a simple alias
      using reference = typename std::conditional<is_const, const T&, T&>::type;
      //! \brief a simple alias
      using const_reference = const T&;
      //! \brief a simple alias
      using size_type = typename tensor<N, T>::size_type;
      //! \brief a simple alias
      using difference_type = typename tensor<N, T>::difference_type;

      /*!
       * \brief constructor
       * \param[in] v : v_ values
       */
      explicit Expr(pointer v_) noexcept : v(v_) {}
      //! \brief move constructor
      Expr(Expr&&) noexcept = default;
      //! \brief copy constructor
      Expr(const Expr&) noexcept = default;

      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }

      const_reference operator()(const size_type i) const noexcept {
        return this->v[i];
      }  // end of operator() const

      reference operator()(const size_type i) noexcept {
        static_assert(!is_const, "invalid call for const view");
        return this->v[i];
      }  // end of operator()

      const_reference operator[](const size_type i) const noexcept {
        return this->v[i];
      }  // end of operator[] const

      reference operator[](const size_type i) noexcept {
        static_assert(!is_const, "invalid call for const view");
        return this->v[i];
      }  // end of operator[]

      //! \brief using tensor_base::operator=
      using tensor_base<Expr>::operator=;
      //! \brief assignement operator
      Expr& operator=(const Expr& src) {
        static_assert(!is_const, "invalid call for const view");
        tensor_base<Expr>::operator=(src);
        return *this;
      }

      /*!
       * \return the size of the symmetric tensor
       * (compatibility with vector)
       */
      constexpr TFEL_MATH_INLINE size_type size() const noexcept {
        return TensorDimeToSize<N>::value;
      }

     protected:
      const pointer v;

     private:
      Expr() = delete;
      //! \brief simple check
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));

    };  // end of struct Expr

    template <unsigned short N, typename T = double>
    using TensorView = Expr<tensor<N, T>, TensorViewExpr<N, T, false>>;

    template <unsigned short N, typename T = double>
    using ConstTensorView = Expr<tensor<N, T>, TensorViewExpr<N, T, true>>;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TENSOR_TENSORVIEW_HXX */
