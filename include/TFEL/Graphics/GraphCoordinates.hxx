/*!
 * \file   GraphCoordinates.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 jan 2009
 */

#ifndef _LIB_TFEL_GRAPHCOORDINATES_HXX_
#define _LIB_TFEL_GRAPHCOORDINATES_HXX_ 

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Math/Evaluator.hxx"

namespace tfel
{

  namespace graphics
  {

    struct GraphCoordinates
    {
      enum CoordinateType{
	FIRST,
	SECOND,
	GRAPH,
	SCREEN,
	CHARACTER
      }; // end of enum CoordinateType
      GraphCoordinates(CoordinateType,
		       const tfel::utilities::SmartPtr<tfel::math::Evaluator>);
      CoordinateType coordinateType;
      tfel::utilities::SmartPtr<tfel::math::Evaluator> c;
    }; // end of GraphCoordinates

  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TFEL_GRAPHCOORDINATES_HXX_ */
