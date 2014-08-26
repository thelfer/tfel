/*! 
 * \file  CyranoGetModellingHypothesis.cxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#include<sstream>

#include"MFront/Cyrano/CyranoException.hxx"
#include"MFront/Cyrano/CyranoGetModellingHypothesis.hxx"

namespace cyrano
{

  tfel::material::ModellingHypothesis::Hypothesis
  getModellingHypothesis(const CyranoInt ndi)
  {
    using namespace std;
    using namespace tfel::material;
    switch(ndi){
    case 14:
      return ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN;
// C                       =  3 a 11 : unidimensionnel plan (massif 1D)
// C                       = 12 a 14 : unidimensionnel axisymetrique (massif 1D)
// C                       = 15 : unidimensionnel spherique (massif 1D)
    default:
      ;
    }
    ostringstream msg;
    msg << "cyrano::getModellingHypothesis : "
	<< "invalid argument ('" << ndi << "')";
    throw(CyranoException(msg.str()));
    return ModellingHypothesis::UNDEFINEDHYPOTHESIS;
  }

} // end of namespace cyrano
