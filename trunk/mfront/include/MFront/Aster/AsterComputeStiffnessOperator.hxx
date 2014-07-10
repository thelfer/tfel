/*!
 * \file   AsterComputeStiffnessOperator.hxx
 * \brief  This file declares the AsterComputeStiffnessOperator class
 * \author Helfer Thomas
 * \date   18 mar 2008
 */

#ifndef _LIB_MFRONT_ASTERCOMPUTESTIFFNESSOPERATOR_H_
#define _LIB_MFRONT_ASTERCOMPUTESTIFFNESSOPERATOR_H_ 

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterConfig.hxx"
#include"MFront/Aster/AsterTraits.hxx"

namespace aster
{

  template<unsigned short N,
	   AsterSymmetryType>
  struct AsterComputeStiffnessOperator;

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeStiffnessOperator<1u,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<1u,AsterReal,false>::StiffnessTensor&);
  }; // end of struct AsterComputeStiffnessOperator

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeStiffnessOperator<2u,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<2u,AsterReal,false>::StiffnessTensor&);
  }; // end of struct AsterComputeStiffnessOperator

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeStiffnessOperator<3u,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<3u,AsterReal,false>::StiffnessTensor&);
  }; // end of struct AsterComputeStiffnessOperator

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeStiffnessOperator<1u,ORTHOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<1u,AsterReal,false>::StiffnessTensor&);
  }; // end of struct AsterComputeStiffnessOperator

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeStiffnessOperator<2u,ORTHOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<2u,AsterReal,false>::StiffnessTensor&);
  }; // end of struct AsterComputeStiffnessOperator

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeStiffnessOperator<3u,ORTHOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<3u,AsterReal,false>::StiffnessTensor&);
  }; // end of struct AsterComputeStiffnessOperator

} // end of namespace aster

#endif /* _LIB_MFRONT_ASTERCOMPUTESTIFFNESSOPERATOR_H */

