/*!
 * \file   include/TFEL/Math/Stensor/StensorView.hxx
 * \author Thomas Helfer
 * \date   16 July 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORVIEW_HXX
#define LIB_TFEL_MATH_STENSORVIEW_HXX

#include "TFEL/Metaprogramming/StaticAssert.hxx"

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel {

  namespace math {

    /*!
     *  Stensor From Tiny Vector
     * \param N  : space dimension
     * \param T  : value type
     */
    template <unsigned short N, typename T, bool is_const>
    struct StensorViewExpr {
    };  // end of struct StensorFromTinyMatrixColumnViewExpr

    /*!
     * \brief Structure exposing a raw pointer as a symmetric tensor
     * \param N  : space dimension
     * \param T  : value type
     */
    template <unsigned short N, typename T, bool is_const>
    struct Expr<stensor<N, T>, StensorViewExpr<N, T, is_const>>
        : public StensorConcept<
              Expr<stensor<N, T>, StensorViewExpr<N, T, is_const>>>,
          public stensor_base<
              Expr<stensor<N, T>, StensorViewExpr<N, T, is_const>>> {
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
      using size_type = typename stensor<N, T>::size_type;
      //! \brief a simple alias
      using difference_type = typename stensor<N, T>::difference_type;

      /*!
       * \param[in] v : v_ values
       */
      explicit Expr(pointer v_) noexcept : v(v_) {}  // end of Expr

      Expr(Expr&&) noexcept = default;

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

      //! using stensor_base::operator=
      using stensor_base<Expr>::operator=;
      //! assignement operator
      Expr& operator=(const Expr& src) {
        static_assert(!is_const, "invalid call for const view");
        stensor_base<Expr>::operator=(src);
        return *this;
      }
      /*!
       * \return the size of the symmetric tensor
       * (compatibility with vector)
       */
      constexpr TFEL_MATH_INLINE size_type size() const noexcept {
        return StensorDimeToSize<N>::value;
      }

     protected:
      //! underlying data
      const pointer v;

     private:
      Expr() = delete;
      //! simple check
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));

    };  // end of struct Expr

    template <unsigned short N, typename T = double>
    using StensorView = Expr<stensor<N, T>, StensorViewExpr<N, T, false>>;

    template <unsigned short N, typename T = double>
    using ConstStensorView = Expr<stensor<N, T>, StensorViewExpr<N, T, true>>;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_STENSORVIEW_HXX */
