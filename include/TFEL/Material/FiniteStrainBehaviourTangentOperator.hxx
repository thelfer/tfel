/*! 
 * \file   FiniteStrainBehaviourTangentOperator.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  11 juin 2014
 */

#ifndef _LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_H_
#define _LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_H_ 

#include<vector>
#include<string>
#include<utility>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Math/ST2toST2/ST2toST2Expr.hxx"
#include"TFEL/Math/Forward/st2tost2.hxx"
#include"TFEL/Math/T2toST2/T2toST2Expr.hxx"
#include"TFEL/Math/Forward/t2tost2.hxx"

#include"TFEL/Utilities/GenTypeBase.hxx"
#include"TFEL/Material/FiniteStrainBehaviourTangentOperatorBase.hxx"

namespace tfel
{

  namespace material
  {

    /*!
     * \return a list of supported tangent operator
     */
    TFELMATERIAL_VISIBILITY_EXPORT
    std::vector<FiniteStrainBehaviourTangentOperatorBase::Flag>
    getFiniteStrainBehaviourTangentOperatorFlags(void);
    /*!
     * \return a string describing the given tangent operator type 
     */
    TFELMATERIAL_VISIBILITY_EXPORT std::string
    getFiniteStrainBehaviourTangentOperatorDescription(const FiniteStrainBehaviourTangentOperatorBase::Flag);
    /*!
     * \return a string representation of the flag
     */
    TFELMATERIAL_VISIBILITY_EXPORT
    std::string
    convertFiniteStrainBehaviourTangentOperatorFlagToString(const FiniteStrainBehaviourTangentOperatorBase::Flag);
    /*!
     * \return the type associated with this tangent operator (t2tost2 or st2tost2)
     */
    TFELMATERIAL_VISIBILITY_EXPORT
    std::string
    getFiniteStrainBehaviourTangentOperatorFlagType(const FiniteStrainBehaviourTangentOperatorBase::Flag);
    /*!
     * a structure that can hold all the possible consistent tangent
     * operators for a given space dimension
     * \param N : space dimension
     * \param StressType : type of the stresses
     */
    template<unsigned short N,
	     typename StressType>
    struct TFEL_VISIBILITY_LOCAL FiniteStrainBehaviourTangentOperator
      : public FiniteStrainBehaviourTangentOperatorBase,
	public tfel::utilities::GenTypeBase<typename tfel::meta::GenerateTypeList<tfel::math::t2tost2<N,StressType>,
										  tfel::math::st2tost2<N,StressType>,
										  tfel::math::t2tost2<N,StressType> *,
										  tfel::math::st2tost2<N,StressType> *>::type>
    {
      //! supported tangent operator types;
      typedef typename tfel::meta::GenerateTypeList<tfel::math::t2tost2<N,StressType>,
						    tfel::math::st2tost2<N,StressType>,
						    tfel::math::t2tost2<N,StressType>*,
						    tfel::math::st2tost2<N,StressType>* >::type TOTypes;
      typedef tfel::utilities::GenTypeBase<TOTypes> GenType;
      /*!
       * default constructor
       */
      FiniteStrainBehaviourTangentOperator()
      {}
      FiniteStrainBehaviourTangentOperator(const FiniteStrainBehaviourTangentOperator& src)
	: GenType(src)
      {}
      FiniteStrainBehaviourTangentOperator(const tfel::math::t2tost2<N,StressType>& D)
	: GenType(D)
      {}
      FiniteStrainBehaviourTangentOperator(tfel::math::t2tost2<N,StressType>* const D)
	: GenType(D)
      {}
      FiniteStrainBehaviourTangentOperator(const tfel::math::st2tost2<N,StressType>& D)
	: GenType(D)
      {}
      FiniteStrainBehaviourTangentOperator(tfel::math::st2tost2<N,StressType>* const D)
	: GenType(D)
      {}
      /*!
       * \param const T1&, the value affected to the GenType.
       * \pre   T1 must be a type that the GenType can hold.
       */
      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,TOTypes>::value==1, 
	void >::type 
      set_uninitialised(void){
	GenType::template set_uninitialised<T1>();
      }
      /*!
       * assignement operator
       */
      FiniteStrainBehaviourTangentOperator&
      operator = (const tfel::math::t2tost2<N,StressType>& e){
	using namespace tfel::math;
	if(this->template is<t2tost2<N,StressType>* >()){
	  *(this->template get<t2tost2<N,StressType>* >()) = e;
	} else {
	  if(!this->template is<t2tost2<N,StressType> >()){
	    this->template set_uninitialised<t2tost2<N,StressType> >();
	  }
	  this->template get<t2tost2<N,StressType> >() = e;
	}
	return *this;
      }
      /*!
       * assignement operator
       */
      template<typename Expr>
      FiniteStrainBehaviourTangentOperator&
      operator = (const tfel::math::T2toST2Expr<tfel::math::t2tost2<N,StressType>,Expr>& e){
	using namespace tfel::math;
	if(this->template is<t2tost2<N,StressType>* >()){
	  *(this->template get<t2tost2<N,StressType>* >()) = e;
	} else {
	  if(!this->template is<t2tost2<N,StressType> >()){
	    this->template set_uninitialised<t2tost2<N,StressType> >();
	  }
	  this->template get<t2tost2<N,StressType> >() = e;
	}
	return *this;
      }
      /*!
       * assignement operator
       */
      template<typename Expr>
      FiniteStrainBehaviourTangentOperator&
      operator = (const tfel::math::st2tost2<N,StressType>& e){
	using namespace tfel::math;
	if(this->template is<st2tost2<N,StressType>* >()){
	  *(this->template get<st2tost2<N,StressType>* >()) = e;
	} else {
	  if(!this->template is<st2tost2<N,StressType> >()){
	    this->template set_uninitialised<st2tost2<N,StressType> >();
	  }
	  this->template get<st2tost2<N,StressType> >() = e;
	}
	return *this;
      }
      /*!
       * assignement operator
       */
      template<typename Expr>
      FiniteStrainBehaviourTangentOperator&
      operator = (const tfel::math::ST2toST2Expr<tfel::math::st2tost2<N,StressType>,Expr>& e){
	using namespace tfel::math;
	if(this->template is<st2tost2<N,StressType>* >()){
	  *(this->template get<st2tost2<N,StressType>* >()) = e;
	} else {
	  if(!this->template is<st2tost2<N,StressType> >()){
	    this->template set_uninitialised<st2tost2<N,StressType> >();
	  }
	  this->template get<st2tost2<N,StressType> >() = e;
	}
	return *this;
      }
    }; // end of struct FiniteStrainBehaviourTangentOperator

    /*!
     * a meta function given the TFEL type associated with a
     * FiniteStrainBehaviourTangentOperatorBase::Flag
     */
    template<FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType,
	     unsigned short N,
	     typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType;

    /*!
     * partial specialisation for the derivative of the Cauchy stress with respect to
     * the deformation gradient
     */
    template<unsigned short N,typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DSIG_DF,N,StressType>
    {
      //! the result of the meta function
      typedef tfel::math::t2tost2<N,StressType> type;
    }; // end of struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DSIG_DF,N,StressType>

    /*!
     * partial specialisation for the derivative of the Cauchy stress
     * with respect to the deformation gradient increment
     */
    template<unsigned short N,typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DSIG_DDF,N,StressType>
    {
      //! the result of the meta function
      typedef tfel::math::t2tost2<N,StressType> type;
    }; // end of struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DSIG_DDF,N,StressType>

    /*!
     * partial specialisation for the derivative of the Cauchy stress
     * with respect to the deformation gradient increment
     */
    template<unsigned short N,typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DSIG_DD,N,StressType>
    {
      //! the result of the meta function
      typedef tfel::math::st2tost2<N,StressType> type;
    }; // end of struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DSIG_DD,N,StressType>

    /*!
     * partial specialisation for the derivative of the Kirchhoff
     * stress with respect to the deformation gradient
     */
    template<unsigned short N,typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DTAU_DF,N,StressType>
    {
      //! the result of the meta function
      typedef tfel::math::t2tost2<N,StressType> type;
    }; // end of struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DTAU_DF,N,StressType>

    /*!
     * partial specialisation for the derivative of the Kirchhoff
     * stress with respect to the deformation gradient increment
     */
    template<unsigned short N,typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DTAU_DDF,N,StressType>
    {
      //! the result of the meta function
      typedef tfel::math::t2tost2<N,StressType> type;
    }; // end of struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DTAU_DDF,N,StressType>

    /*!
     * partial specialisation for the derivative of the second
     * Piola-Kirchhoff stress with respect to the deformation gradient
     */
    template<unsigned short N,typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DS_DF,N,StressType>
    {
      //! the result of the meta function
      typedef tfel::math::t2tost2<N,StressType> type;
    }; // end of struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DS_DF,N,StressType>

    /*!
     * partial specialisation for the derivative of the second
     * Piola-Kirchhoff stress with respect to the deformation gradient
     * increment
     */
    template<unsigned short N,typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DS_DDF,N,StressType>
    {
      //! the result of the meta function
      typedef tfel::math::t2tost2<N,StressType> type;
    }; // end of struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DS_DDF,N,StressType>

    /*!
     * partial specialisation for the derivative of the second
     * Piola-Kirchhoff stress with respect to the right Cauchy-Green
     * deformation gradient
     */
    template<unsigned short N,typename StressType>
    struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DS_DC,N,StressType>
    {
      //! the result of the meta function
      typedef tfel::math::st2tost2<N,StressType> type;
    }; // end of struct FiniteStrainBehaviourTangentOperatorType<FiniteStrainBehaviourTangentOperatorBase::DS_DC,N,StressType>

  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_H */

