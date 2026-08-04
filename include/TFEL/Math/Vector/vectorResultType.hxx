/*!
 * \file   include/TFEL/Math/Vector/vectorResultType.hxx
 * \brief
 * \author Thomas Helfer
 * \date   06 Oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_VECTORRESULTTYPE_HXX
#define LIB_TFEL_MATH_VECTORRESULTTYPE_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/matrix.hxx"

namespace tfel::math {

  template <typename T>
  struct matrix;

  /*
   * Partial specialisation for negation.
   */
  template <typename T>
  struct UnaryResultType<vector<T>, OpNeg> {
    using ResBase_ = typename UnaryResultType<T, OpNeg>::type;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tvector and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename T, typename Scal>
  class ComputeBinaryOperationResult<VectorTag,
                                     ScalarTag,
                                     vector<T>,
                                     Scal,
                                     OpMult> {
    using ResBase_ = result_type<T, Scal, OpMult>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tvector and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename Scal, typename T>
  class ComputeBinaryOperationResult<ScalarTag,
                                     VectorTag,
                                     Scal,
                                     vector<T>,
                                     OpMult> {
    using ResBase_ = result_type<Scal, T, OpMult>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for vector
   * \see   ResultType
   */
  template <typename T, typename T2>
  class ResultType<vector<T>, vector<T2>, OpDotProduct> {
    using ResBase_ = result_type<T, T2, OpMult>;
    static constexpr bool isValid = !isInvalid<ResBase_>();

   public:
    using type = std::conditional_t<isValid, ResBase_, tfel::meta::InvalidType>;
  };  // end of class ResultType<vector<T>,vector<T2>,OpDotProduct>

  /*!
   * \brief Partial specialisation for vector and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename T, typename Scal>
  class ComputeBinaryOperationResult<VectorTag,
                                     ScalarTag,
                                     vector<T>,
                                     Scal,
                                     OpDiv> {
    using ResBase_ = result_type<T, Scal, OpDiv>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for vector
   * \see   ResultType
   */
  template <typename T, typename T2>
  class ResultType<vector<T>, vector<T2>, OpDiadicProduct> {
    using ResBase_ = result_type<T, T2, OpMult>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    matrix<ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for vector
   * \see   ResultType
   */
  template <typename T, typename T2>
  class ResultType<vector<T>, vector<T2>, OpPlus> {
    using ResBase_ = result_type<T, T2, OpPlus>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for vector
   * \see   ResultType
   */
  template <typename T, typename T2>
  class ResultType<vector<T>, vector<T2>, OpMinus> {
    using ResBase_ = result_type<T, T2, OpMinus>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for vector
   * \see   ResultType
   */
  template <typename T, typename T2>
  class ResultType<vector<T>, vector<T2>, OpMult> {
    using ResBase_ = result_type<T, T2, OpMult>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_VECTORRESULTTYPE_HXX */
