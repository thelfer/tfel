/*!
 * \file   include/TFEL/Math/Matrix/TVectorTMatrixExpr.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   06 avr 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TVECTORTMATRIXEXPR_HXX
#define LIB_TFEL_MATH_TVECTORTMATRIXEXPR_HXX

#include <cstddef>
#include <iterator>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/FSAlgorithm/inner_product.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/RunTimeCheck.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel::math {

  template <unsigned short N, unsigned short M, typename A, typename B>
  struct TVectorTMatrixExpr : public ExprBase {
    static_assert(implementsVectorConcept<A>());
    static_assert(implementsMatrixConcept<B>());

    typedef EmptyRunTimeProperties RunTimeProperties;

   protected:
    typedef BinaryOperationResult<A, B, OpMult> Result;

    ArgumentStorage<A> a;
    ArgumentStorage<B> b;

    TVectorTMatrixExpr() = delete;

    struct ColumnConstIterator {
      using MType = std::decay_t<B>;
      using NumType = numeric_type<MType>;
      TFEL_MATH_INLINE ColumnConstIterator(const MType& m_,
                                           const unsigned short j_)
          : m(m_), i(0), j(j_) {}
      TFEL_MATH_INLINE ColumnConstIterator& operator++() {
        ++i;
        return *this;
      }  // end of operator++
      TFEL_MATH_INLINE NumType operator*() const { return m(i, j); }

     private:
      const MType& m;
      unsigned short i;
      const unsigned short j;
    };  // end of struc ColumnConstIterator

    struct VectorConstIterator {
      using VType = std::decay_t<A>;
      using NumType = numeric_type<VType>;
      TFEL_MATH_INLINE VectorConstIterator(const VType& v_) : v(v_), i(0) {}
      TFEL_MATH_INLINE VectorConstIterator& operator++() {
        ++i;
        return *this;
      }  // end of operator++
      TFEL_MATH_INLINE NumType operator*() const { return this->v(this->i); }

     private:
      const VType& v;
      unsigned short i;
    };  // end of struc VectorConstIterator

    using NumType = numeric_type<Result>;
    using IndexType = numeric_type<Result>;

    typedef NumType value_type;
    typedef NumType* pointer;
    typedef const NumType* const_pointer;
    typedef NumType& reference;
    typedef const NumType& const_reference;
    typedef IndexType size_type;
    typedef ptrdiff_t difference_type;

    TFEL_MATH_INLINE TVectorTMatrixExpr(const A& l, const B& r) : a(l), b(r) {}

    TFEL_MATH_INLINE TVectorTMatrixExpr(
        const TVectorTMatrixExpr<N, M, A, B>& src)
        : a(src.a), b(src.b) {}

    TFEL_MATH_INLINE const NumType operator()(const IndexType i) const {
      using namespace tfel::fsalgo;
      return inner_product<N>::template exe<NumType>(VectorConstIterator(a),
                                                     ColumnConstIterator(b, i));
    }

   public:
    TFEL_MATH_INLINE const RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TVECTORTMATRIXEXPR_HXX */
