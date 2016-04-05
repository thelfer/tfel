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

  template<AbaqusBehaviourType btype,
	   typename real,unsigned short N>
  struct AbaqusTangentOperatorType
  {
    using type      = tfel::math::st2tost2<N,real>;
    using view_type = tfel::math::ST2toST2View<N,real>;
  };
  
  /*!
   * \brief an helper structure introduced to normalise the tangent
   * operator to follow the umat interface
   */
  template<typename real>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT AbaqusTangentOperator
  {
    /*!
     * \brief normalize the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::ST2toST2View<1u,real>&);
    /*!
     * \brief normalize the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::ST2toST2View<2u,real>&);
    /*!
     * \brief normalize the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::ST2toST2View<3u,real>&);
    /*!
     * \brief transpose the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::ST2toST2View<1u,real>&);
    /*!
     * \brief transpose the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::ST2toST2View<2u,real>&);
    /*!
     * \brief transpose the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::ST2toST2View<3u,real>&);
  }; // end of struct AbaqusTangentOperator

  /*!
   * \brief this class extracts and normalise (convert to
   * abaqus/fortran conventions) the tangent operator
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H>
  struct ExtractAndConvertTangentOperator
  {
    template<typename Behaviour,typename real>
    static void exe(const Behaviour& b,real *const DDSDDE){
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
      template<typename Behaviour,typename real>
      static void exe(const Behaviour& bv,
		      real *const DDSDDE)
      {
	using tfel::material::ModellingHypothesisToSpaceDimension;
	const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
	using  TangentOperatorType     = typename AbaqusTangentOperatorType<AbaqusTraits<Behaviour>::btype,real,N>::type;
	using  TangentOperatorViewType = typename AbaqusTangentOperatorType<AbaqusTraits<Behaviour>::btype,real,N>::view_type;
	TangentOperatorViewType Dt{DDSDDE};
	Dt = static_cast<const TangentOperatorType&>(bv.getTangentOperator());
	AbaqusTangentOperator<real>::normalize(Dt);
      } // end of exe	  
    };
    struct SymmetricConsistentTangentOperatorComputer
    {
      template<typename Behaviour,typename real>
      static void exe(const Behaviour& bv,
		      real *const DDSDDE)
      {
	ConsistentTangentOperatorComputer::exe(bv,DDSDDE);
      } // end of exe	  
    };
    struct GeneralConsistentTangentOperatorComputer
    {
      template<typename Behaviour,typename real>
      static void exe(const Behaviour& bv,real *const DDSDDE)
      {
	using tfel::material::ModellingHypothesisToSpaceDimension;
	const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
	using TangentOperatorViewType = typename AbaqusTangentOperatorType<AbaqusTraits<Behaviour>::btype,real,N>::view_type;
	ConsistentTangentOperatorComputer::exe(bv,DDSDDE);
	// les conventions fortran....
	TangentOperatorViewType Dt{DDSDDE};
	AbaqusTangentOperator<real>::transpose(Dt);
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
    template<typename Behaviour,typename real>
    static void exe(const Behaviour& b,real *const DDSDDE){
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
      template<typename Behaviour,typename real>
      static void exe(const Behaviour& bv,
		      real *const DDSDDE)
      {
#if (not defined _MSC_VER) && (not defined __INTEL_COMPILER)
	using tfel::math::constexpr_fct::sqrt;
	constexpr const real one   = real(1);
	constexpr const real two   = real(2);
	constexpr const real sqrt2 = sqrt(two);
	constexpr const real cste = one/sqrt2;
#else
	const real cste = real(1)/std::sqrt(real(2));
#endif
	constexpr const real one_half = real(1)/real(2);
	constexpr const unsigned short N = 2u;
	using  TangentOperatorType = typename AbaqusTangentOperatorType<AbaqusTraits<Behaviour>::btype,real,N>::type;
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
      template<typename Behaviour,typename real>
      static void exe(const Behaviour& bv,real *const DDSDDE)
      {
#ifndef _MSC_VER
	using tfel::math::constexpr_fct::sqrt;
	constexpr real cste
	  = real(1)/tfel::math::constexpr_fct::sqrt(real(2));
#else
	const real cste = real(1)/std::sqrt(real(2));
#endif
	constexpr const real one_half = real(1)/real(2);
	constexpr const unsigned short N = 2u;
	using  TangentOperatorType = typename AbaqusTangentOperatorType<AbaqusTraits<Behaviour>::btype,real,N>::type;
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

#include"MFront/Abaqus/AbaqusTangentOperator.ixx"

#endif /* LIB_MFRONT_ABAQUS_ABAQUSTANGENTOPERATOR_H_ */
