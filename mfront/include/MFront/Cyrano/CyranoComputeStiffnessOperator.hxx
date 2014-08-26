/*!
 * \file   CyranoComputeStiffnessOperator.hxx
 * \brief  This file declares the CyranoComputeStiffnessOperator class
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_MFRONT_CYRANOCOMPUTESTIFFNESSOPERATOR_H_
#define _LIB_MFRONT_CYRANOCOMPUTESTIFFNESSOPERATOR_H_ 

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/Cyrano/Cyrano.hxx"
#include"MFront/Cyrano/CyranoConfig.hxx"
#include"MFront/Cyrano/CyranoTraits.hxx"

namespace cyrano
{

  /*!
   * This structure is in charge of computing the Stiffness operator
   * from the material properties given by Cast3M.  The resulting
   * operator uses MFront representation of tensors and symmetric
   * tensors.
   */
  template<tfel::material::ModellingHypothesis::Hypothesis,
	   CyranoSymmetryType>
  struct CyranoComputeStiffnessOperator;

  template<>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
  CyranoComputeStiffnessOperator<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
     exe(const CyranoReal* const,
	 tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor&);
  }; // end of struct CyranoComputeStiffnessOperator

  template<>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
  CyranoComputeStiffnessOperator<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
     exe(const CyranoReal* const,
	 tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor&);
  }; // end of struct CyranoComputeStiffnessOperator

} // end of namespace cyrano

#endif /* _LIB_MFRONT_CYRANOCOMPUTESTIFFNESSOPERATOR_H */

