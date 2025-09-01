/*!
 * \file   include/TFEL/Math/General/ComputeObjectTag.ixx
 * \brief  This file declares the ComputeObjectTag class
 * \author Thomas Helfer
 * \date   16 Oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_COMPUTEOBJECTTAG_IXX
#define LIB_TFEL_MATH_COMPUTEOBJECTTAG_IXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsUnaryOperator.hxx"

namespace tfel::math {

  namespace internals {

    template <typename T, typename = void>
    struct HasConceptTag : std::false_type {};

    template <typename T>
    struct HasConceptTag<T, std::void_t<typename T::ConceptTag>>
        : std::true_type {};

  }  // end of namespace internals

  struct ScalarTag {};  // end of ScalarTag

  struct UnaryOperatorTag {};  // end of UnaryOperatorTag

  /*!
   * \class   ComputeObjectTag_
   * \brief   An helper metafunction for tag computing.
   * \warning This class shall not be used directly.
   * \see     ComputeObjectTag
   * \tparam   typename Type, type tested.
   * \tparam   bool b1, true if scalar.
   * \tparam   bool b2, true if operator.
   * \tparam   bool b3, true if the object has a ConceptTag typedef.
   * \return  type, the tag searched. By default, return InvalidType.
   */
  template <typename Type, bool b1 = false, bool b2 = false, bool b3 = false>
  struct ComputeObjectTag_ {
    //! \brief Result
    typedef tfel::meta::InvalidType type;
  };  // end of ComputeObjectTag_

  /*!
   * \brief Partial specialisation for scalars.
   * \see   ComputeObjectTag_
   */
  template <typename Type>
  struct ComputeObjectTag_<Type, true> {
    //! \brief Result
    typedef ScalarTag type;
  };  // end of ComputeObjectTag_<true,false,false,false,false,false,false>

  /*!
   * \brief Partial specialisation for unary operators.
   * \see   ComputeObjectTag_
   */
  template <typename Type>
  struct ComputeObjectTag_<Type, false, true> {
    //! \brief Result
    typedef UnaryOperatorTag type;
  };  // end of ComputeObjectTag_<true,false,false,false,false,false,false>

  /*!
   * \brief Partial specialisation for functions.
   * \see   ComputeObjectTag_
   */
  template <typename Type>
  struct ComputeObjectTag_<Type, false, false, true> {
    //! \brief Result
    using type = typename Type::ConceptTag;
  };  // end of ComputeObjectTag_<false,false,true>

  template <typename T>
  struct ComputeObjectTag {
   private:
    //! \brief tells if T is a scalar.
    static constexpr bool IsTScalar = tfel::typetraits::isScalar<T>();
    //! \brief tells if T is an unary operator.
    static constexpr bool IsTUnaryOperator =
        tfel::typetraits::isUnaryOperator<T>();

   public:
    /*!
     * \brief The result type, which is computed by the auxiliary
     * metafunction ComputeObjectTag_
     */
    using type = typename ComputeObjectTag_<T,
                                            IsTScalar,
                                            IsTUnaryOperator,
                                            hasConceptTag<T>()>::type;
  };  // end of ComputeObjectTag

  template <typename T>
  constexpr bool hasConceptTag() {
    return tfel::math::internals::HasConceptTag<std::remove_cv_t<T>>::value;
  }  // end of hasConceptTag

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_COMPUTEOBJECTTAG_IXX */
