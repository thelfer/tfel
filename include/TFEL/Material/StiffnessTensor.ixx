/*! 
 * \file   StiffnessTensor.ixx
 * \brief
 * \author Helfer Thomas
 * \date   23 oct. 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATERIAL_STIFFNESSTENSOR_IXX_
#define _LIB_TFEL_MATERIAL_STIFFNESSTENSOR_IXX_ 

namespace tfel{

  namespace material{

    template<ModellingHypothesis::Hypothesis H>
    template<typename StressType>
    void
    computeAlteredStiffnessTensor<H>::exe(tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value,StressType>& Da,
					  const tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value,StressType>& D)
    {
      Da = D;
    }
    
    template<typename StressType>
    void
    computeAlteredStiffnessTensor<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS>::exe(tfel::math::st2tost2<1u,StressType>& Da,
												  const tfel::math::st2tost2<1u,StressType>& D)
    {
      Da(0,0)=D(0,0)-D(1,0)/D(1,1)*D(0,1);
      Da(0,2)=D(0,2)-D(1,2)/D(1,1)*D(0,1);
      Da(2,0)=D(2,0)-D(1,0)/D(1,1)*D(2,1);
      Da(2,2)=D(2,2)-D(1,2)/D(1,1)*D(2,1);
      Da(0,1)=StressType(0);
      Da(1,0)=StressType(0);
      Da(1,1)=StressType(0);
      Da(1,2)=StressType(0);
      Da(2,1)=StressType(0);
    }

    template<typename StressType>
    void
    computeAlteredStiffnessTensor<ModellingHypothesis::PLANESTRESS>::exe(tfel::math::st2tost2<2u,StressType>& Da,
									 const tfel::math::st2tost2<2u,StressType>& D)
    {
      Da(0,0)=D(0,0)-D(2,0)/D(2,2)*D(0,2);
      Da(0,1)=D(0,1)-D(2,1)/D(2,2)*D(0,2);
      Da(1,0)=D(1,0)-D(2,0)/D(2,2)*D(1,2);
      Da(1,1)=D(1,1)-D(2,1)/D(2,2)*D(1,2);
      Da(0,2)=StressType(0);
      Da(1,2)=StressType(0);
      Da(2,0)=StressType(0);
      Da(2,1)=StressType(0);
      Da(2,2)=StressType(0);
      Da(0,3)=StressType(0);
      Da(1,3)=StressType(0);
      Da(2,3)=StressType(0);
      Da(3,0)=StressType(0);
      Da(3,1)=StressType(0);
      Da(3,2)=StressType(0);
      Da(3,3)=D(3,3);
    } // end of computeAlteredStiffnessTensor

  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIAL_STIFFNESSTENSOR_IXX */
