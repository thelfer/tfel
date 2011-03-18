/*! 
 * \file  ModellingHypothesis.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 16 mar 2011
 */

#ifndef _LIB_TFEL_MATERIAL_MODELLINGHYPOTHESIS_H_
#define _LIB_TFEL_MATERIAL_MODELLINGHYPOTHESIS_H_ 

namespace tfel
{

  namespace material
  {
    
    struct ModellingHypothesis
    {
      
      enum Hypothesis{
	AXISYMETRICALGENERALISEDPLANESTRAIN,
	AXISYMETRICAL,
	PLANESTRESS,
	PLANESTRAIN,
	GENERALISEDPLANESTRAIN,
	TRIDIMENSIONAL,
	UNDEFINEDHYPOTHESIS
      }; // end of enum ModellingHypothesis

    }; // end of struct ModellingHypothesis

  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIAL_MODELLINGHYPOTHESIS_H */

