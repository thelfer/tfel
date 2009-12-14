/*!
 * \file   GraphCoordinates.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   07 jan 2009
 */

#include"Graphics/GraphCoordinates.hxx"

namespace tfel
{

  namespace graphics
  {

    GraphCoordinates::GraphCoordinates(const CoordinateType ct_,
				       const tfel::utilities::SmartPtr<tfel::math::Evaluator> c_)
      : coordinateType(ct_),
	c(c_)
    {} // end of GraphCoordinates::GraphCoordinates

  } // end of namespace graphics

} // end of namespace tfel
