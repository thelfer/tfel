/*! 
 * \file  src/Material/ModellingHypothesis.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 mai 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>

#include"TFEL/Material/ModellingHypothesis.hxx"

namespace tfel
{

  namespace material
  {
    
    const std::vector<ModellingHypothesis::Hypothesis>&
    ModellingHypothesis::getModellingHypotheses(void)
    {
      using namespace std;
      static constexpr Hypothesis h[7u] = {AXISYMMETRICALGENERALISEDPLANESTRAIN,
				       AXISYMMETRICALGENERALISEDPLANESTRESS,
				       AXISYMMETRICAL,
				       PLANESTRESS,
				       PLANESTRAIN,
				       GENERALISEDPLANESTRAIN,
				       TRIDIMENSIONAL};
      static vector<Hypothesis> hypotheses(h,h+7u);
      return hypotheses;
    } // end of ModellingHypothesis::getModellingHypotheses

    bool
    ModellingHypothesis::isModellingHypothesis(const std::string& h)
    {
      if((h=="AxisymmetricalGeneralisedPlaneStrain")||
	 (h=="AxisymmetricalGeneralisedPlaneStress")||
	 (h=="Axisymmetrical")||
	 (h=="PlaneStress")||
	 (h=="PlaneStrain")||
	 (h=="GeneralisedPlaneStrain")||
	 (h=="Tridimensional")){
	return true;
      }
      return false;
    } // end of ModellingHypothesis::isModellingHypothesis

    std::string
    ModellingHypothesis::toString(const Hypothesis h)
    {
      using namespace std;
      if(h==AXISYMMETRICALGENERALISEDPLANESTRAIN){
	return "AxisymmetricalGeneralisedPlaneStrain";
      } else if(h==AXISYMMETRICALGENERALISEDPLANESTRESS){
	return "AxisymmetricalGeneralisedPlaneStress";
      } else if (h==AXISYMMETRICAL){
	return "Axisymmetrical";
      } else if (h==PLANESTRESS){
	return "PlaneStress";
      } else if (h==PLANESTRAIN){
	return "PlaneStrain";
      } else if (h==GENERALISEDPLANESTRAIN){
	return "GeneralisedPlaneStrain";
      } else if (h==TRIDIMENSIONAL){
	return "Tridimensional";
      }
      string msg("ModellingHypothesis::toString : ");
      msg += "unsupported modelling hypothesis";
      throw(runtime_error(msg));
      return "";				       
    }

    std::string
    ModellingHypothesis::toUpperCaseString(const Hypothesis h)
    {
      using namespace std;
      if(h==AXISYMMETRICALGENERALISEDPLANESTRAIN){
	return "AXISYMMETRICALGENERALISEDPLANESTRAIN";
      } else if(h==AXISYMMETRICALGENERALISEDPLANESTRESS){
	return "AXISYMMETRICALGENERALISEDPLANESTRESS";
      } else if (h==AXISYMMETRICAL){
	return "AXISYMMETRICAL";
      } else if (h==PLANESTRESS){
	return "PLANESTRESS";
      } else if (h==PLANESTRAIN){
	return "PLANESTRAIN";
      } else if (h==GENERALISEDPLANESTRAIN){
	return "GENERALISEDPLANESTRAIN";
      } else if (h==TRIDIMENSIONAL){
	return "TRIDIMENSIONAL";
      }
      string msg("ModellingHypothesis::toUpperCaseString : ");
      msg += "unsupported modelling hypothesis";
      throw(runtime_error(msg));
      return "";				       
    }

    ModellingHypothesis::Hypothesis
    ModellingHypothesis::fromString(const std::string& h)
    {
      using namespace std;
      if(h=="AxisymmetricalGeneralisedPlaneStrain"){
	return AXISYMMETRICALGENERALISEDPLANESTRAIN;
      } else if(h=="AxisymmetricalGeneralisedPlaneStress"){
	return AXISYMMETRICALGENERALISEDPLANESTRESS;
      } else if(h=="Axisymmetrical"){
	return AXISYMMETRICAL;
      } else if(h=="PlaneStress"){
	return PLANESTRESS;
      } else if(h=="PlaneStrain"){
	return PLANESTRAIN;
      } else if(h=="GeneralisedPlaneStrain"){
	return GENERALISEDPLANESTRAIN;
      } else if(h=="Tridimensional"){
	return TRIDIMENSIONAL;
      } else {
	string msg("MFrontBehaviourParserCommon::getModellingHypothesisFromString : "
		   "invalid or unsupported hypothesis '"+h+"'. The following "
		   "hypotheses are supported:\n"
		   "- AxisymmetricalGeneralisedPlaneStrain\n"
		   "- Axisymmetrical\n"
		   "- PlaneStress\n"
		   "- PlaneStrain\n"
		   "- GeneralisedPlaneStrain\n"
		   "- Tridimensional");
	throw(runtime_error(msg));
      }
      return UNDEFINEDHYPOTHESIS;
    }

  } // end of namespace material

} // end of namespace tfel
