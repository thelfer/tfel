/*!
 * \file   include/TFEL/Math/General/ComputeObjectTag.ixx
 * \brief  This file declares the ComputeObjectTag class
 * \author Helfer Thomas
 * \date   16 Oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_COMPUTEOBJECTTAG_I_
#define LIB_TFEL_COMPUTEOBJECTTAG_I_

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsUnaryOperator.hxx"

namespace tfel {

  namespace math {

    namespace internals {

      template <typename A>
      class HasConceptTag {
        /*!
         * \brief A first type.
         */
        typedef char Small;
        /*!
         * \brief A Second type which size is higher than Small
         */
        class Big {
          Small dummy[2];
        };
        /*
         * \brief a substitute for classes that have only protected constructors
         */
        template <typename B>
        struct Subs {};

       protected:
        template <typename B>
        static typename std::enable_if<sizeof(typename B::ConceptTag) != 0,
                                       Small>::type
        Test(const Subs<B>);

        static Big Test(...);

        /*!
         * \brief A function returning a T.
         * A small trick for classes that are not default constructible.
         */
        static Subs<A> MakeSubsA(void);

       public:
        static constexpr bool cond = sizeof(Test(MakeSubsA())) == sizeof(Small);

      };  // end of class HasConceptTag

    }  // end of namespace internals

    struct ScalarTag {};  // end of ScalarTag

    struct UnaryOperatorTag {};  // end of UnaryOperatorTag

    /*!
     * \class   ComputeObjectTag_
     * \brief   An helper metafunction for tag computing.
     * \warning This class shall not be used directly.
     * \see     ComputeObjectTag
     * \param   typename Type, type tested.
     * \param   bool b1, true if scalar.
     * \param   bool b2, true if operator.
     * \param   bool b3, true if the object has a ConceptTag typedef.
     * \return  type, the tag searched. By default, return InvalidType.
     */
    template <typename Type, bool b1 = false, bool b2 = false, bool b3 = false>
    struct ComputeObjectTag_ {
      /*!
       * Result
       */
      typedef tfel::meta::InvalidType type;
    };  // end of ComputeObjectTag_

    /*!
     * \brief Partial specialisation for scalars.
     * \see   ComputeObjectTag_
     */
    template <typename Type>
    struct ComputeObjectTag_<Type, true> {
      /*!
       * Result
       */
      typedef ScalarTag type;
    };  // end of ComputeObjectTag_<true,false,false,false,false,false,false>

    /*!
     * \brief Partial specialisation for unary operators.
     * \see   ComputeObjectTag_
     */
    template <typename Type>
    struct ComputeObjectTag_<Type, false, true> {
      /*!
       * Result
       */
      typedef UnaryOperatorTag type;
    };  // end of ComputeObjectTag_<true,false,false,false,false,false,false>

    /*!
     * \brief Partial specialisation for functions.
     * \see   ComputeObjectTag_
     */
    template <typename Type>
    struct ComputeObjectTag_<Type, false, false, true> {
      /*!
       * Result
       */
      typedef typename Type::ConceptTag type;
    };  // end of ComputeObjectTag_<false,false,true>

    template <typename T>
    struct ComputeObjectTag {
     private:
      /*!
       * \brief tells if T is a scalar.
       */
      static constexpr bool IsTScalar = tfel::typetraits::IsScalar<T>::cond;
      /*!
       * \brief tells if T is an unary operator.
       */
      static constexpr bool IsTUnaryOperator =
          tfel::typetraits::IsUnaryOperator<T>::cond;
      /*!
       * \brief tells if T has a ConceptTag typedef.
       */
      static constexpr bool HasConceptTag =
          tfel::math::internals::HasConceptTag<T>::cond;

     public:
      /*!
       * The result type, which is computed by the auxiliary
       * metafunction ComputeObjectTag_
       */
      typedef typename ComputeObjectTag_<T,
                                         IsTScalar,
                                         IsTUnaryOperator,
                                         HasConceptTag>::type type;
    };  // end of ComputeObjectTag

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_COMPUTEOBJECTTAG_I_ */
