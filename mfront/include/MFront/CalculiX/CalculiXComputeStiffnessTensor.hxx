/*!
 * \file   mfront/include/MFront/CalculiX/CalculiXComputeStiffnessTensor.hxx
 * \brief  This file declares the CalculiXComputeStiffnessTensor class
 * \author Helfer Thomas
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_CALCULIXCOMPUTESTIFFNESSTENSOR_HXX
#define LIB_MFRONT_CALCULIXCOMPUTESTIFFNESSTENSOR_HXX 

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/CalculiX/CalculiX.hxx"
#include"MFront/CalculiX/CalculiXConfig.hxx"
#include"MFront/CalculiX/CalculiXTraits.hxx"

namespace calculix
{

  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_CALCULIX_VISIBILITY_EXPORT void
  CalculiXComputeIsotropicStiffnessTensor3D(tfel::config::Types<3u,CalculiXReal,false>::StiffnessTensor&,
					    const CalculiXReal* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_CALCULIX_VISIBILITY_EXPORT void
  CalculiXComputeOrthotropicStiffnessTensor3D(tfel::config::Types<3u,CalculiXReal,false>::StiffnessTensor&,
					      const CalculiXReal* const);
  
  /*!
   * This structure is in charge of computing the Stiffness operator
   * from the material properties given by Cast3M.  The resulting
   * operator uses MFront representation of tensors and symmetric
   * tensors.
   */
  template<CalculiXBehaviourType,
	   tfel::material::ModellingHypothesis::Hypothesis,
	   CalculiXSymmetryType,bool>
  struct CalculiXComputeStiffnessTensor;

  template<bool b>
  struct TFEL_VISIBILITY_LOCAL
  CalculiXComputeStiffnessTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
			     ISOTROPIC,b>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template<typename real>
    static void
    exe(typename tfel::config::Types<3u,real,false>::StiffnessTensor& C,
	const real* const props)
    {
      CalculiXComputeIsotropicStiffnessTensor3D(C,props);
    }
  }; // end of struct CalculiXComputeStiffnessTensor

  template<bool b>
  struct TFEL_VISIBILITY_LOCAL
  CalculiXComputeStiffnessTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
				 tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
				 ORTHOTROPIC,b>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template<typename real>
    static void
    exe(typename tfel::config::Types<3u,real,false>::StiffnessTensor& C,
	const real* const props)
    {
      CalculiXComputeOrthotropicStiffnessTensor3D(C,props);      
    }
  }; // end of struct CalculiXComputeStiffnessTensor

  /*!
   * Partial specialisation for finite strain behaviours
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   CalculiXSymmetryType stype,bool b>
  struct TFEL_VISIBILITY_LOCAL
  CalculiXComputeStiffnessTensor<calculix::FINITESTRAINSTANDARDBEHAVIOUR,H,stype,b>
  {
    typedef tfel::material::ModellingHypothesisToSpaceDimension<H> ModellingHypothesisToSpaceDimension;
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template<typename real>
    static void
    exe(typename tfel::config::Types<ModellingHypothesisToSpaceDimension::value,real,false>::StiffnessTensor& D,
	const real* const props)
    {
      CalculiXComputeStiffnessTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,H,stype,b>::exe(D,props);
    }
  }; // end of struct CalculiXComputeStiffnessTensor

} // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIXCOMPUTESTIFFNESSTENSOR_HXX */

