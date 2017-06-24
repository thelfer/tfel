/*! 
 * \file  mfront/include/MFront/CalculiX/CalculiXTangentOperator.hxx
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

#ifndef LIB_MFRONT_CALCULIX_CALCULIXTANGENTOPERATOR_HXX
#define LIB_MFRONT_CALCULIX_CALCULIXTANGENTOPERATOR_HXX 

#include<type_traits>

#include"TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include"TFEL/Math/General/MathConstants.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Material/MechanicalBehaviourTraits.hxx"

#include"MFront/CalculiX/CalculiX.hxx"
#include"MFront/CalculiX/CalculiXTraits.hxx"
#include"MFront/CalculiX/CalculiXConfig.hxx"

namespace calculix
{

  template<CalculiXBehaviourType btype,
	   typename real,unsigned short N>
  struct CalculiXTangentOperatorType
  {
    using type      = tfel::math::st2tost2<N,real>;
    using view_type = tfel::math::ST2toST2View<N,real>;
  };
  
  /*!
   * \brief an helper structure introduced to normalise the tangent
   * operator to follow the umat interface
   */
  template<typename real>
  struct CalculiXTangentOperator
  {
    /*!
     * \brief normalize the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::ST2toST2View<3u,real>&);
    /*!
     * \brief transpose the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::ST2toST2View<3u,real>&);
  }; // end of struct CalculiXTangentOperator

  /*!
   * \brief this class extracts and normalise (convert to
   * calculix/fortran conventions) the tangent operator
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
	using  TangentOperatorType     = typename CalculiXTangentOperatorType<CalculiXTraits<Behaviour>::btype,real,N>::type;
	using  TangentOperatorViewType = typename CalculiXTangentOperatorType<CalculiXTraits<Behaviour>::btype,real,N>::view_type;
	TangentOperatorViewType Dt{DDSDDE};
	Dt = static_cast<const TangentOperatorType&>(bv.getTangentOperator());
	CalculiXTangentOperator<real>::normalize(Dt);
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
	using TangentOperatorViewType = typename CalculiXTangentOperatorType<CalculiXTraits<Behaviour>::btype,real,N>::view_type;
	ConsistentTangentOperatorComputer::exe(bv,DDSDDE);
	// les conventions fortran....
	TangentOperatorViewType Dt{DDSDDE};
	CalculiXTangentOperator<real>::transpose(Dt);
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
	using  TangentOperatorType
	  = typename CalculiXTangentOperatorType<CalculiXTraits<Behaviour>::btype,real,2u>::type;
	constexpr      const auto icste = tfel::math::Cste<real>::isqrt2;
	TFEL_CONSTEXPR const auto one_half = real(1)/real(2);
	auto Dt = static_cast<const TangentOperatorType&>(bv.getTangentOperator());
	DDSDDE[0] = Dt(0,0);
	DDSDDE[1] = Dt(1,0);
	DDSDDE[2] = Dt(3,0)*icste;
	DDSDDE[3] = Dt(0,1);
	DDSDDE[4] = Dt(1,1);
	DDSDDE[5] = Dt(3,1)*icste;
	DDSDDE[6] = Dt(0,3)*icste;
	DDSDDE[7] = Dt(1,3)*icste;
	DDSDDE[8] = Dt(3,3)*one_half;
      } // end of exe	  
    };
    struct GeneralConsistentTangentOperatorComputer
    {
      template<typename Behaviour,typename real>
      static void exe(const Behaviour& bv,real *const DDSDDE)
      {
	using  TangentOperatorType =
	  typename CalculiXTangentOperatorType<CalculiXTraits<Behaviour>::btype,real,2u>::type;
	constexpr const auto icste = tfel::math::Cste<real>::isqrt2;
	TFEL_CONSTEXPR const auto one_half = real(1)/real(2);
	auto Dt = static_cast<const TangentOperatorType&>(bv.getTangentOperator());
	DDSDDE[0] = Dt(0,0);
	DDSDDE[1] = Dt(0,1);
	DDSDDE[2] = Dt(0,3)*icste;
	DDSDDE[3] = Dt(1,0);
	DDSDDE[4] = Dt(1,1);
	DDSDDE[5] = Dt(1,3)*icste;
	DDSDDE[6] = Dt(3,0)*icste;
	DDSDDE[7] = Dt(3,1)*icste;
	DDSDDE[8] = Dt(3,3)*one_half;
      } // end of exe	  
    };
  };
  
} // end of namespace calculix

#include"MFront/CalculiX/CalculiXTangentOperator.ixx"

#endif /* LIB_MFRONT_CALCULIX_CALCULIXTANGENTOPERATOR_HXX */
