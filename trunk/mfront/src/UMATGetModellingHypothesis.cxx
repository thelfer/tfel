/*! 
 * \file  UMATGetModellingHypothesis.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 17 mar 2011
 */

#include<sstream>

#include"MFront/UMAT/UMATException.hxx"
#include"MFront/UMAT/UMATGetModellingHypothesis.hxx"

namespace umat
{

  tfel::material::ModellingHypothesis::Hypothesis
  getModellingHypothesis(const UMATInt ndi)
  {
    using namespace std;
    using namespace tfel::material;
    switch(ndi){
    case 2:
      return ModellingHypothesis::TRIDIMENSIONAL;
    case 0:
      return ModellingHypothesis::AXISYMMETRICAL;
    case -1:
      return ModellingHypothesis::PLANESTRAIN;
    case -2:
      return ModellingHypothesis::PLANESTRESS;
    case -3:
      return ModellingHypothesis::GENERALISEDPLANESTRAIN;
    case 14:
      return ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN;
// C                       =  3 a 11 : unidimensionnel plan (massif 1D)
// C                       = 12 a 14 : unidimensionnel axisymetrique (massif 1D)
// C                       = 15 : unidimensionnel spherique (massif 1D)
    }
    ostringstream msg;
    msg << "umat::getModellingHypothesis : "
	<< "invalid argument ('" << ndi << "')";
    throw(UMATException(msg.str()));
    return ModellingHypothesis::UNDEFINEDHYPOTHESIS;
  }

} // end of namespace umat
