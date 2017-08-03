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

  /*!
   * \brief this class extracts and normalise (convert to
   * calculix/fortran conventions) the tangent operator
   */
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
    struct SymmetricConsistentTangentOperatorComputer
    {
      template<typename Behaviour,typename real>
      static void exe(const Behaviour& bv,
		      real *const DDSDDE)
      {
	using TangentOperatorType = tfel::math::st2tost2<3u,real>;
	constexpr const auto icste = tfel::math::Cste<real>::isqrt2;
	const auto& Dt =
	  static_cast<const TangentOperatorType&>(bv.getTangentOperator());
	DDSDDE[0]  = Dt(0,0);
	DDSDDE[1]  = Dt(0,1);
	DDSDDE[2]  = Dt(1,1);
	DDSDDE[3]  = Dt(0,2);
	DDSDDE[4]  = Dt(1,2);
	DDSDDE[5]  = Dt(2,2);
	DDSDDE[6]  = Dt(0,3)*icste;
	DDSDDE[7]  = Dt(1,3)*icste;
	DDSDDE[8]  = Dt(2,3)*icste;
	DDSDDE[9]  = Dt(3,3);
	DDSDDE[10] = Dt(0,4)*icste;
	DDSDDE[11] = Dt(1,4)*icste;
	DDSDDE[12] = Dt(2,4)*icste;
	DDSDDE[13] = Dt(3,4);
	DDSDDE[14] = Dt(4,4);
	DDSDDE[15] = Dt(0,5)*icste;
	DDSDDE[16] = Dt(1,5)*icste;
	DDSDDE[17] = Dt(2,5)*icste;
	DDSDDE[18] = Dt(3,5);
	DDSDDE[19] = Dt(4,5);
	DDSDDE[20] = Dt(5,5);
      } // end of exe	  
    };
    struct GeneralConsistentTangentOperatorComputer
    {
      template<typename Behaviour,typename real>
      static void exe(const Behaviour& bv,real *const DDSDDE)
      {
	using TangentOperatorType = tfel::math::st2tost2<3u,real>;
	constexpr const auto hicste = tfel::math::Cste<real>::isqrt2/2;
	const auto& Dt =
	  static_cast<const TangentOperatorType&>(bv.getTangentOperator());
	DDSDDE[0]  = Dt(0,0);
	DDSDDE[1]  = (Dt(0,1)+Dt(1,0))/2;
	DDSDDE[2]  = Dt(1,1);
	DDSDDE[3]  = (Dt(0,2)+Dt(2,0))/2;
	DDSDDE[4]  = (Dt(1,2)+Dt(2,1))/2;
	DDSDDE[5]  = Dt(2,2);
	DDSDDE[6]  = (Dt(0,3)+Dt(3,0))*hicste;
	DDSDDE[7]  = (Dt(1,3)+Dt(3,1))*hicste;
	DDSDDE[8]  = (Dt(2,3)+Dt(3,2))*hicste;
	DDSDDE[9]  = Dt(3,3);
	DDSDDE[10] = (Dt(0,4)+Dt(4,0))*hicste;
	DDSDDE[11] = (Dt(1,4)+Dt(4,1))*hicste;
	DDSDDE[12] = (Dt(2,4)+Dt(4,2))*hicste;
	DDSDDE[13] = (Dt(3,4)+Dt(4,3))/2;
	DDSDDE[14] = Dt(4,4);
	DDSDDE[15] = (Dt(0,5)+Dt(5,0))*hicste;
	DDSDDE[16] = (Dt(1,5)+Dt(5,1))*hicste;
	DDSDDE[17] = (Dt(2,5)+Dt(5,2))*hicste;
	DDSDDE[18] = (Dt(3,5)+Dt(5,3))/2;
	DDSDDE[19] = (Dt(4,5)+Dt(5,4))/2;
	DDSDDE[20] = Dt(5,5);
      } // end of exe	  
    };
  }; // end of struct ExtractAndConvertTangentOperator
  
} // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIX_CALCULIXTANGENTOPERATOR_HXX */
