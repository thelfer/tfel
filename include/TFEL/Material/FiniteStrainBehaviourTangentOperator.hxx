/*!
 * \file   include/TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  11 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_HXX
#define LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_HXX

#include <vector>
#include <string>
#include <utility>

#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/Forward/st2tost2.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2Concept.hxx"
#include "TFEL/Math/Forward/t2tost2.hxx"
#include "TFEL/Math/T2toST2/T2toST2Concept.hxx"

#include "TFEL/Utilities/GenTypeBase.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperatorBase.hxx"

namespace tfel {

  namespace material {

    /*!
     * \return a list of supported tangent operator
     */
    TFELMATERIAL_VISIBILITY_EXPORT
    std::vector<FiniteStrainBehaviourTangentOperatorBase::Flag>
    getFiniteStrainBehaviourTangentOperatorFlags();
    /*!
     * \return a string describing the given tangent operator type
     */
    TFELMATERIAL_VISIBILITY_EXPORT std::string
    getFiniteStrainBehaviourTangentOperatorDescription(
        const FiniteStrainBehaviourTangentOperatorBase::Flag);
    /*!
     * \return a string representation of the flag
     */
    TFELMATERIAL_VISIBILITY_EXPORT
    std::string convertFiniteStrainBehaviourTangentOperatorFlagToString(
        const FiniteStrainBehaviourTangentOperatorBase::Flag);
    /*!
     * \return the type associated with this tangent operator (t2tost2 or
     * st2tost2)
     */
    TFELMATERIAL_VISIBILITY_EXPORT
    std::string getFiniteStrainBehaviourTangentOperatorFlagType(
        const FiniteStrainBehaviourTangentOperatorBase::Flag);
    /*!
     * a structure that can hold all the possible consistent tangent
     * operators for a given space dimension
     * \param N : space dimension
     * \param StressType : type of the stresses
     */
    template <unsigned short N, typename StressType>
    struct TFEL_VISIBILITY_LOCAL FiniteStrainBehaviourTangentOperator
        : public FiniteStrainBehaviourTangentOperatorBase,
          public tfel::utilities::GenTypeBase<
              typename tfel::meta::GenerateTypeList<
                  tfel::math::t2tost2<N, StressType>,
                  tfel::math::st2tost2<N, StressType>,
                  tfel::math::t2tost2<N, StressType>*,
                  tfel::math::st2tost2<N, StressType>*>::type> {
      //! supported tangent operator types;
      typedef typename tfel::meta::GenerateTypeList<
          tfel::math::t2tost2<N, StressType>,
          tfel::math::st2tost2<N, StressType>,
          tfel::math::t2tost2<N, StressType>*,
          tfel::math::st2tost2<N, StressType>*>::type TOTypes;
      typedef tfel::utilities::GenTypeBase<TOTypes> GenType;
      /*!
       * default constructor
       */
      FiniteStrainBehaviourTangentOperator() {}
      FiniteStrainBehaviourTangentOperator(
          const FiniteStrainBehaviourTangentOperator& src)
          : GenType(src) {}
      FiniteStrainBehaviourTangentOperator(
          const tfel::math::t2tost2<N, StressType>& D)
          : GenType(D) {}
      FiniteStrainBehaviourTangentOperator(
          tfel::math::t2tost2<N, StressType>* const D)
          : GenType(D) {}
      FiniteStrainBehaviourTangentOperator(
          const tfel::math::st2tost2<N, StressType>& D)
          : GenType(D) {}
      FiniteStrainBehaviourTangentOperator(
          tfel::math::st2tost2<N, StressType>* const D)
          : GenType(D) {}
      FiniteStrainBehaviourTangentOperator& operator=(
          const FiniteStrainBehaviourTangentOperator& src) {
        GenType::operator=(src);
        return *this;
      }
      /*!
       * \param const T1&, the value affected to the GenType.
       * \pre   T1 must be a type that the GenType can hold.
       */
      template <typename T1>
      TFEL_INLINE typename std::enable_if<
          tfel::meta::TLCountNbrOfT<T1, TOTypes>::value == 1,
          void>::type
      set_uninitialised() {
        GenType::template set_uninitialised<T1>();
      }
      /*!
       * assignement operator
       */
      template <typename T>
      typename std::enable_if<
          tfel::meta::Implements<T, tfel::math::T2toST2Concept>::cond &&
              tfel::math::T2toST2Traits<T>::dime == N &&
              std::is_same<typename tfel::math::T2toST2Traits<T>::NumType,
                           StressType>::value,
          FiniteStrainBehaviourTangentOperator&>::type
      operator=(const T& e) {
        using namespace tfel::math;
        if (this->template is<t2tost2<N, StressType>*>()) {
          *(this->template get<t2tost2<N, StressType>*>()) = e;
        } else {
          if (!this->template is<t2tost2<N, StressType>>()) {
            this->template set_uninitialised<t2tost2<N, StressType>>();
          }
          this->template get<t2tost2<N, StressType>>() = e;
        }
        return *this;
      }
      /*!
       * assignement operator
       */
      template <typename T>
      typename std::enable_if<
          tfel::meta::Implements<T, tfel::math::ST2toST2Concept>::cond &&
              tfel::math::ST2toST2Traits<T>::dime == N &&
              std::is_same<typename tfel::math::ST2toST2Traits<T>::NumType,
                           StressType>::value,
          FiniteStrainBehaviourTangentOperator&>::type
      operator=(const T& e) {
        using namespace tfel::math;
        if (this->template is<st2tost2<N, StressType>*>()) {
          *(this->template get<st2tost2<N, StressType>*>()) = e;
        } else {
          if (!this->template is<st2tost2<N, StressType>>()) {
            this->template set_uninitialised<st2tost2<N, StressType>>();
          }
          this->template get<st2tost2<N, StressType>>() = e;
        }
        return *this;
      }
    };  // end of struct FiniteStrainBehaviourTangentOperator

    /*!
     * a meta function given the TFEL type associated with a
     * FiniteStrainBehaviourTangentOperatorBase::Flag
     */
    template <FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType,
              unsigned short N,
              typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType;

    /*!
     * partial specialisation for the derivative of the Cauchy stress with
     * respect to the deformation gradient
     */
    template <unsigned short N, typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<
        FiniteStrainBehaviourTangentOperatorBase::DSIG_DF,
        N,
        StressType> {
      //! the result of the meta function
      using type = tfel::math::t2tost2<N, StressType>;
    };  // end of struct
        // FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DSIG_DF,N,StressType>

    /*!
     * partial specialisation for the derivative of the Cauchy stress
     * with respect to the deformation gradient increment
     */
    template <unsigned short N, typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<
        FiniteStrainBehaviourTangentOperatorBase::DSIG_DDF,
        N,
        StressType> {
      //! the result of the meta function
      using type = tfel::math::t2tost2<N, StressType>;
    };  // end of struct
        // FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DSIG_DDF,N,StressType>

    /*!
     * partial specialisation for the derivative of the Cauchy stress
     * with respect to the deformation gradient increment
     */
    template <unsigned short N, typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<
        FiniteStrainBehaviourTangentOperatorBase::C_TRUESDELL,
        N,
        StressType> {
      //! the result of the meta function
      using type = tfel::math::st2tost2<N, StressType>;
    };  // end of struct
        // FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::C_TRUESDELL,N,StressType>

    /*!
     * \brief partial specialisation for the tangent moduli associated
     * with the Jaumann rate of the Kirchhoff stress divided by J.
     */
    template <unsigned short N, typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<
        FiniteStrainBehaviourTangentOperatorBase::ABAQUS,
        N,
        StressType> {
      //! the result of the meta function
      using type = tfel::math::st2tost2<N, StressType>;
    };
    /*!
     * \brief partial specialisation for the tangent moduli associated
     * with the Jaumann rate of the Kirchhoff stress divided by J.
     */
    template <unsigned short N, typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<
        FiniteStrainBehaviourTangentOperatorBase::C_TAU_JAUMANN,
        N,
        StressType> {
      //! the result of the meta function
      using type = tfel::math::st2tost2<N, StressType>;
    };
    /*!
     * \brief partial specialisation for spatial moduli
     */
    template <unsigned short N, typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<
        FiniteStrainBehaviourTangentOperatorBase::SPATIAL_MODULI,
        N,
        StressType> {
      //! the result of the meta function
      using type = tfel::math::st2tost2<N, StressType>;
    };

    /*!
     * partial specialisation for the derivative of the Kirchhoff
     * stress with respect to the deformation gradient
     */
    template <unsigned short N, typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<
        FiniteStrainBehaviourTangentOperatorBase::DTAU_DF,
        N,
        StressType> {
      //! the result of the meta function
      using type = tfel::math::t2tost2<N, StressType>;
    };  // end of struct
        // FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DTAU_DF,N,StressType>

    /*!
     * partial specialisation for the derivative of the Kirchhoff
     * stress with respect to the deformation gradient increment
     */
    template <unsigned short N, typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<
        FiniteStrainBehaviourTangentOperatorBase::DTAU_DDF,
        N,
        StressType> {
      //! the result of the meta function
      using type = tfel::math::t2tost2<N, StressType>;
    };  // end of struct
        // FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DTAU_DDF,N,StressType>

    /*!
     * partial specialisation for the derivative of the second
     * Piola-Kirchhoff stress with respect to the deformation gradient
     */
    template <unsigned short N, typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<
        FiniteStrainBehaviourTangentOperatorBase::DS_DF,
        N,
        StressType> {
      //! the result of the meta function
      using type = tfel::math::t2tost2<N, StressType>;
    };  // end of struct
        // FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DS_DF,N,StressType>

    /*!
     * partial specialisation for the derivative of the second
     * Piola-Kirchhoff stress with respect to the deformation gradient
     * increment
     */
    template <unsigned short N, typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<
        FiniteStrainBehaviourTangentOperatorBase::DS_DDF,
        N,
        StressType> {
      //! the result of the meta function
      using type = tfel::math::t2tost2<N, StressType>;
    };  // end of struct
        // FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DS_DDF,N,StressType>
    /*!
     * partial specialisation for the derivative of the second
     * Piola-Kirchhoff stress with respect to the right Cauchy-Green
     * deformation gradient
     */
    template <unsigned short N, typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<
        FiniteStrainBehaviourTangentOperatorBase::DS_DC,
        N,
        StressType> {
      //! the result of the meta function
      using type = tfel::math::st2tost2<N, StressType>;
    };  // end of struct
        // FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DS_DC,N,StressType>
    /*!
     * partial specialisation for the derivative of the second
     * Piola-Kirchhoff stress with respect to the Green-Lagrange
     * strain
     */
    template <unsigned short N, typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<
        FiniteStrainBehaviourTangentOperatorBase::DS_DEGL,
        N,
        StressType> {
      //! the result of the meta function
      using type = tfel::math::st2tost2<N, StressType>;
    };  // end of struct
        // FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DS_DEGL,N,StressType>
    /*!
     * partial specialisation for the dual of the lagrangian Hencky strain with
     * respect to the lagrangian Hencky strain
     */
    template <unsigned short N, typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<
        FiniteStrainBehaviourTangentOperatorBase::DT_DELOG,
        N,
        StressType> {
      //! the result of the meta function
      using type = tfel::math::st2tost2<N, StressType>;
    };  // end of struct
        // FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DSIG_DF,N,StressType>
    //! a simple alias
    template <FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType,
              unsigned short N,
              typename StressType>
    using tangent_operator =
        typename FiniteStrainBehaviourTangentOperatorType<TangenOperatorType,
                                                          N,
                                                          StressType>::type;
    /*!
     * A generic structure for convertion between tangent operators.
     * This structure must be specialised for each supported case.
     */
    template <FiniteStrainBehaviourTangentOperatorBase::Flag ResultFlag,
              FiniteStrainBehaviourTangentOperatorBase::Flag SourceFlag>
    struct FiniteStrainBehaviourTangentOperatorConverter;
    /*!
     * \brief a small wrapper around the
     * FiniteStrainBehaviourTangentOperatorConverter structure \return the
     * result of the convertion \param[in] K:  the initial stiffness tensor
     * \param[in] F0: the deformation gradient
     * \param[in] F1: the deformation gradient
     * \param[in] s:  the Cauchy stress tensor
     * \tparam ResultFlag: Flag describing the result of the conversion
     * \tparam SourceFlag: Flag describing the result of the conversion
     */
    template <FiniteStrainBehaviourTangentOperatorBase::Flag ResultFlag,
              FiniteStrainBehaviourTangentOperatorBase::Flag SourceFlag,
              unsigned short N,
              typename StressType>
    TFEL_MATERIAL_INLINE tangent_operator<ResultFlag, N, StressType> convert(
        const tangent_operator<SourceFlag, N, StressType>&,
        const tfel::math::tensor<N, tfel::typetraits::base_type<StressType>>&,
        const tfel::math::tensor<N, tfel::typetraits::base_type<StressType>>&,
        const tfel::math::stensor<N, StressType>&);
    /*!
     * \brief a small wrapper around the
     * FiniteStrainBehaviourTangentOperatorConverter structure \param[out] Kr:
     * the result of the convertion \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     * \tparam ResultFlag: Flag describing the result of the conversion
     * \tparam SourceFlag: Flag describing the result of the conversion
     */
    template <FiniteStrainBehaviourTangentOperatorBase::Flag ResultFlag,
              FiniteStrainBehaviourTangentOperatorBase::Flag SourceFlag,
              unsigned short N,
              typename StressType>
    TFEL_MATERIAL_INLINE void convert(
        tangent_operator<ResultFlag, N, StressType>&,
        const tangent_operator<SourceFlag, N, StressType>&,
        const tfel::math::tensor<N, tfel::typetraits::base_type<StressType>>&,
        const tfel::math::tensor<N, tfel::typetraits::base_type<StressType>>&,
        const tfel::math::stensor<N, StressType>&);

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.ixx"

#endif /* LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_HXX */
