/*!
 * \file   include/TFEL/Math/Vector/tvectorResultType.hxx
 * \brief  this file implements partial specialisations of the ResultType.
 * \brief
 * \author Thomas Helfer
 * \date   09 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINY_VECTOR_RESULT_TYPE_HXX
#define LIB_TFEL_MATH_TINY_VECTOR_RESULT_TYPE_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/DotProduct.hxx"
#include "TFEL/Math/tvector.hxx"

namespace tfel::math {

  /*
   * Partial specialisation for negation.
   */
  template <unsigned short N, typename T>
  struct UnaryResultType<tvector<N, T>, OpNeg> {
    typedef typename UnaryResultType<T, OpNeg>::type ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tvector and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<VectorTag,
                                     ScalarTag,
                                     tvector<N, T>,
                                     Scal,
                                     OpMult> {
    typedef result_type<T, Scal, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tvector and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<VectorTag,
                                     ScalarTag,
                                     tvector<N, T>,
                                     Scal,
                                     OpDiv> {
    typedef result_type<T, Scal, OpDiv> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tvector and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename Scal, unsigned short N, typename T>
  class ComputeBinaryOperationResult<ScalarTag,
                                     VectorTag,
                                     Scal,
                                     tvector<N, T>,
                                     OpMult> {
    typedef result_type<Scal, T, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tvector
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<tvector<N, T>, tvector<N, T2>, OpPlus> {
    typedef result_type<T, T2, OpPlus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tvector
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<tvector<N, T>, tvector<N, T2>, OpMinus> {
    typedef result_type<T, T2, OpMinus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tvector
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<tvector<N, T>, tvector<N, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tvector
   * \see   ResultType
   */
  template <unsigned short N, typename T, unsigned short M, typename T2>
  class ResultType<tvector<N, T>, tvector<M, T2>, OpDiadicProduct> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tvector
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  struct ResultType<tvector<N, T>, tvector<N, T2>, OpDotProduct> {
   private:
    typedef result_type<T, T2, OpMult> ResBase_;
    static constexpr bool isValid = isInvalid<ResBase_>();

   public:
    using type = std::conditional_t<isValid, tfel::meta::InvalidType, ResBase_>;
  };  // end of class ResultType<tvector<N,T>,tvector<M,T2>,OpDotProduct>

  template <unsigned short N, typename T, typename T2>
  struct VectorDotProductHandle<tvector<N, T>, tvector<N, T2>> {
    typedef DotProduct<N, 0u> type;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINY_VECTOR_RESULT_TYPE_HXX */
