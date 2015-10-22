/*! 
 * \file  mfront/include/MFront/Abaqus/AbaqusTangentOperator.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 févr. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_ABAQUS_ABAQUSTANGENTOPERATOR_H_
#define LIB_MFRONT_ABAQUS_ABAQUSTANGENTOPERATOR_H_ 

#include<type_traits>

#include"TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include"TFEL/Math/General/ConstExprMathFunctions.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Material/MechanicalBehaviourTraits.hxx"

#include"MFront/Abaqus/Abaqus.hxx"
#include"MFront/Abaqus/AbaqusTraits.hxx"
#include"MFront/Abaqus/AbaqusConfig.hxx"

namespace abaqus
{

  template<AbaqusBehaviourType btype,unsigned short N>
  struct AbaqusTangentOperatorType
  {
    using type      = tfel::math::st2tost2<N,AbaqusReal>;
    using view_type = tfel::math::ST2toST2View<N,AbaqusReal>;
  };
  
  /*!
   * \brief an helper structure introduced to normalise the tangent
   * operator to follow the umat interface
   */
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT AbaqusTangentOperator
  {
    /*!
     * \brief normalize the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::ST2toST2View<1u,AbaqusReal>&);
    /*!
     * \brief normalize the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::ST2toST2View<2u,AbaqusReal>&);
    /*!
     * \brief normalize the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::ST2toST2View<3u,AbaqusReal>&);
    /*!
     * \brief transpose the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::ST2toST2View<1u,AbaqusReal>&);
    /*!
     * \brief transpose the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::ST2toST2View<2u,AbaqusReal>&);
    /*!
     * \brief transpose the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::ST2toST2View<3u,AbaqusReal>&);
  }; // end of struct AbaqusTangentOperator

  /*!
   * \brief this class extracts and normalise (convert to
   * abaqus/fortran conventions) the tangent operator
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H>
  struct ExtractAndConvertTangentOperator
  {
    template<typename Behaviour>
    static void exe(const Behaviour& b,AbaqusReal *const DDSDDE){
      using namespace tfel::material;
      typedef MechanicalBehaviourTraits<Behaviour> Traits;
      using handler = typename std::conditional<
	Traits::isConsistentTangentOperatorSymmetric,
	SymmetricConsistentTangentOperatorComputer,
	GeneralConsistentTangentOperatorComputer
	>::type;
      handler::exe(b,DDSDDE);
    }// end of exe
  private:
    struct ConsistentTangentOperatorComputer
    {
      template<typename Behaviour>
      static void exe(const Behaviour& bv,
		      AbaqusReal *const DDSDDE)
      {
	using tfel::material::ModellingHypothesisToSpaceDimension;
	const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
	using  TangentOperatorType     = typename AbaqusTangentOperatorType<AbaqusTraits<Behaviour>::btype,N>::type;
	using  TangentOperatorViewType = typename AbaqusTangentOperatorType<AbaqusTraits<Behaviour>::btype,N>::view_type;
	TangentOperatorViewType Dt{DDSDDE};
	Dt = static_cast<const TangentOperatorType&>(bv.getTangentOperator());
	AbaqusTangentOperator::normalize(Dt);
      } // end of exe	  
    };
    struct SymmetricConsistentTangentOperatorComputer
    {
      template<typename Behaviour>
      static void exe(const Behaviour& bv,
		      AbaqusReal *const DDSDDE)
      {
	ConsistentTangentOperatorComputer::exe(bv,DDSDDE);
      } // end of exe	  
    };
    struct GeneralConsistentTangentOperatorComputer
    {
      template<typename Behaviour>
      static void exe(const Behaviour& bv,AbaqusReal *const DDSDDE)
      {
	using tfel::material::ModellingHypothesisToSpaceDimension;
	const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
	using TangentOperatorViewType = typename AbaqusTangentOperatorType<AbaqusTraits<Behaviour>::btype,N>::view_type;
	ConsistentTangentOperatorComputer::exe(bv,DDSDDE);
	// les conventions fortran....
	TangentOperatorViewType Dt{DDSDDE};
	AbaqusTangentOperator::transpose(Dt);
      } // end of exe	  
    };
  }; // end of struct ExtractAndConvertTangentOperator
  /*!
   * \brief partial specialisation of the
   * ExtractAndConvertTangentOperator for the plane stress modelling
   * hypothesis
   */
  template<>
  struct ExtractAndConvertTangentOperator<tfel::material::ModellingHypothesis::PLANESTRESS>
  {
    template<typename Behaviour>
    static void exe(const Behaviour& b,AbaqusReal *const DDSDDE){
      using namespace tfel::material;
      typedef MechanicalBehaviourTraits<Behaviour> Traits;
      using handler = typename std::conditional<
	Traits::isConsistentTangentOperatorSymmetric,
	SymmetricConsistentTangentOperatorComputer,
	GeneralConsistentTangentOperatorComputer
	>::type;
      handler::exe(b,DDSDDE);
    }// end of exe
  private:
    struct SymmetricConsistentTangentOperatorComputer
    {
      template<typename Behaviour>
      static void exe(const Behaviour& bv,
		      AbaqusReal *const DDSDDE)
      {
	using tfel::math::constexpr_fct::sqrt;
#if defined __INTEL_COMPILER
	const AbaqusReal cste
	  = AbaqusReal(1)/sqrt(AbaqusReal(2));
#else
	constexpr const AbaqusReal cste
	  = AbaqusReal(1)/sqrt(AbaqusReal(2));
#endif
	constexpr const AbaqusReal one_half = AbaqusReal(1)/AbaqusReal(2);
	constexpr const unsigned short N = 2u;
	using  TangentOperatorType = typename AbaqusTangentOperatorType<AbaqusTraits<Behaviour>::btype,N>::type;
	auto Dt = static_cast<const TangentOperatorType&>(bv.getTangentOperator());
	DDSDDE[0] = Dt(0,0);
	DDSDDE[1] = Dt(1,0);
	DDSDDE[2] = Dt(3,0)*cste;
	DDSDDE[3] = Dt(0,1);
	DDSDDE[4] = Dt(1,1);
	DDSDDE[5] = Dt(3,1)*cste;
	DDSDDE[6] = Dt(0,3)*cste;
	DDSDDE[7] = Dt(1,3)*cste;
	DDSDDE[8] = Dt(3,3)*one_half;
      } // end of exe	  
    };
    struct GeneralConsistentTangentOperatorComputer
    {
      template<typename Behaviour>
      static void exe(const Behaviour& bv,AbaqusReal *const DDSDDE)
      {
	using tfel::math::constexpr_fct::sqrt;
	constexpr const AbaqusReal cste     = AbaqusReal(1)/sqrt(AbaqusReal(2));
	constexpr const AbaqusReal one_half = AbaqusReal(1)/AbaqusReal(2);
	constexpr const unsigned short N = 2u;
	using  TangentOperatorType = typename AbaqusTangentOperatorType<AbaqusTraits<Behaviour>::btype,N>::type;
	auto Dt = static_cast<const TangentOperatorType&>(bv.getTangentOperator());
	DDSDDE[0] = Dt(0,0);
	DDSDDE[1] = Dt(0,1);
	DDSDDE[2] = Dt(0,3)*cste;
	DDSDDE[3] = Dt(1,0);
	DDSDDE[4] = Dt(1,1);
	DDSDDE[5] = Dt(1,3)*cste;
	DDSDDE[6] = Dt(3,0)*cste;
	DDSDDE[7] = Dt(3,1)*cste;
	DDSDDE[8] = Dt(3,3)*one_half;
      } // end of exe	  
    };
  };
  
} // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSTANGENTOPERATOR_H_ */

