/*!
 * \file   include/TFEL/Math/T2toT2/T2toT2Concept.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOT2CONCEPT_HXX
#define LIB_TFEL_MATH_T2TOT2CONCEPT_HXX 1

#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/Forward/T2toT2Concept.hxx"

namespace tfel {

  namespace math {

    template <class T>
    struct T2toT2Traits {
      typedef tfel::meta::InvalidType NumType;
      static constexpr unsigned short dime = 0u;
    };
    //! a simple alias
    template <class T>
    using T2toT2NumType = typename T2toT2Traits<T>::NumType;
    /*!
     * \class T2toT2Tag
     * \brief Helper class to characterise t2tot2.
     */
    struct T2toT2Tag {};

    template <class T>
    struct T2toT2Concept {
      typedef T2toT2Tag ConceptTag;

      typename T2toT2Traits<T>::NumType operator()(const unsigned short,
                                                   const unsigned short) const;

     protected:
      T2toT2Concept() = default;
      T2toT2Concept(T2toT2Concept&&) = default;
      T2toT2Concept(const T2toT2Concept&) = default;
      T2toT2Concept& operator=(const T2toT2Concept&) = default;
      ~T2toT2Concept() = default;
    };

    //! paratial specialisation for symmetric tensors
    template <typename Type>
    struct ConceptRebind<T2toT2Tag, Type> {
      using type = T2toT2Concept<Type>;
    };

    template <typename T>
    struct T2toT2Type {
      typedef T type;
    };

    template <typename T2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toT2Type, T2toT2Concept>::cond,
        typename tfel::typetraits::AbsType<
            typename T2toT2Traits<T2toT2Type>::NumType>::type>::type
    abs(const T2toT2Type&);
    /*!
     * \return the determinant of a `st2tost2`
     * \param[in] s: fourth order tensor
     */
    template <typename T2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toT2Type, T2toT2Concept>::cond &&
            (T2toT2Traits<T2toT2Type>::dime == 1u) &&
            tfel::typetraits::IsScalar<T2toT2NumType<T2toT2Type>>::cond,
        typename ComputeUnaryResult<T2toT2NumType<T2toT2Type>,
                                    Power<3>>::Result>::type
    det(const T2toT2Type&);
    /*!
     * \return the determinant of a `st2tost2`
     * \param[in] s: fourth order tensor
     */
    template <typename T2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toT2Type, T2toT2Concept>::cond &&
            ((T2toT2Traits<T2toT2Type>::dime == 2u) ||
             (T2toT2Traits<T2toT2Type>::dime == 3u)) &&
            tfel::typetraits::IsScalar<T2toT2NumType<T2toT2Type>>::cond,
        typename ComputeUnaryResult<
            T2toT2NumType<T2toT2Type>,
            Power<T2toT2Traits<T2toT2Type>::dime>>::Result>::type
    det(const T2toT2Type&);

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/T2toT2/T2toT2Concept.ixx"

#endif /* LIB_TFEL_MATH_T2TOT2CONCEPT_HXX */
