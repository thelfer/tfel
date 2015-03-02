/*! 
 * \file   StiffnessTensor.hxx
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

#ifndef LIB_TFEL_MATERIAL_STIFFNESSTENSOR_H_
#define LIB_TFEL_MATERIAL_STIFFNESSTENSOR_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

namespace tfel{

  namespace material{
    
    /*!
     * \brief compute the altered stiffness tensor from the unaltered
     * one.
     */
    template<ModellingHypothesis::Hypothesis H>
    struct computeAlteredStiffnessTensor
    {
      /*!
       * \param[out] Da : altered stiffness
       * \param[in]  D  : unaltered stiffness
       */
      template<typename StressType>
      static TFEL_MATH_INLINE void
      exe(tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value,StressType>&,
	  const tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value,StressType>&);
    };
    /*!
     * \brief compute the altered stiffness tensor from the unaltered
     * one in axysymmetrical generalised plane stress.
     */
    template<>
    struct computeAlteredStiffnessTensor<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS>
    {
      /*!
       * \param[out] Da : altered stiffness
       * \param[in]  D  : unaltered stiffness
       */
      template<typename StressType>
      static TFEL_MATH_INLINE void
      exe(tfel::math::st2tost2<1u,StressType>&,
	  const tfel::math::st2tost2<1u,StressType>&);
    };
    /*!
     * \brief compute the altered stiffness tensor from the unaltered
     * one in plane stress.
     */
    template<>
    struct computeAlteredStiffnessTensor<ModellingHypothesis::PLANESTRESS>
    {
      /*!
       * \param[out] Da : altered stiffness
       * \param[in]  D  : unaltered stiffness
       */
      template<typename StressType>
      static TFEL_MATH_INLINE void
      exe(tfel::math::st2tost2<2u,StressType>&,
	  const tfel::math::st2tost2<2u,StressType>&);
    };

  } // end of namespace material

} // end of namespace tfel

#include"TFEL/Material/StiffnessTensor.ixx"

#endif /* LIB_TFEL_MATERIAL_STIFFNESSTENSOR_H_ */
