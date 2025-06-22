/*!
 * \file   include/TFEL/Math/Stensor/StensorConcept.hxx
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef STENSOR_CONCEPT_LIB_
#define STENSOR_CONCEPT_LIB_ 1

#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"

#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/General/Abs.hxx"

namespace tfel {

  namespace math {

    template <class T>
    struct StensorTraits {
      typedef tfel::meta::InvalidType NumType;
      typedef unsigned short IndexType;
      static constexpr unsigned short dime = 0u;
    };

    /*!
     * \class StensorTag
     * \brief Helper class to characterise stensors.
     */
    struct StensorTag {};  // end of StensorTag

    template <typename T>
    struct StensorType {
      typedef T type;
    };

    template <typename T>
    struct StensorConcept {
      typedef StensorTag ConceptTag;

      typename StensorTraits<T>::NumType operator()(const unsigned short) const;

      typename StensorTraits<T>::NumType operator[](const unsigned short) const;

     protected:
      StensorConcept() = default;
      StensorConcept(StensorConcept&&) = default;
      StensorConcept(const StensorConcept&) = default;
      StensorConcept& operator=(const StensorConcept&) = default;
      ~StensorConcept() = default;
    };

    //! paratial specialisation for symmetric tensors
    template <typename Type>
    struct ConceptRebind<StensorTag, Type> {
      using type = StensorConcept<Type>;
    };

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        typename tfel::typetraits::AbsType<
            typename StensorTraits<StensorType>::NumType>::type>::type
    abs(const StensorType&);

    template <typename T>
    TFEL_MATH_INLINE
        typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond,
                                typename StensorTraits<T>::NumType>::type
        trace(const T& s);

    template <typename T>
    TFEL_MATH_INLINE2
        typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond,
                                typename StensorTraits<T>::NumType>::type
        sigmaeq(const T& s);

    template <typename T>
    typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond &&
                                StensorTraits<T>::dime == 1u,
                            typename StensorType<T>::type>::type
    deviator(const T&);

    template <typename T>
    typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond &&
                                StensorTraits<T>::dime == 2u,
                            typename StensorType<T>::type>::type
    deviator(const T&);

    template <typename T>
    typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond &&
                                StensorTraits<T>::dime == 3u,
                            typename StensorType<T>::type>::type
    deviator(const T&);
    /*!
     * \brief return the size of a symmetric tensor for the given
     * dimension
     * \param[in] d: space dimension
     */
    TFELMATH_VISIBILITY_EXPORT unsigned short getStensorSize(
        const unsigned short);

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Stensor/StensorConcept.ixx"

#endif /* STENSOR_CONCEPT_LIB_ */
