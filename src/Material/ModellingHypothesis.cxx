/*! 
 * \file  ModellingHypothesis.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 mai 2013
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
      static const Hypothesis h[7u] = {AXISYMMETRICALGENERALISEDPLANESTRAIN,
				       AXISYMMETRICALGENERALISEDPLANESTRESS,
				       AXISYMMETRICAL,
				       PLANESTRESS,
				       PLANESTRAIN,
				       GENERALISEDPLANESTRAIN,
				       TRIDIMENSIONAL};
      static vector<Hypothesis> hypotheses(h,h+7u);
      return hypotheses;
    } // end of ModellingHypothesis::getModellingHypotheses

    std::string
    ModellingHypothesis::HypothesisToString(const Hypothesis h)
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
      string msg("ModellingHypothesis::HypothesisToString : ");
      msg += "unsupported modelling hypothesis";
      throw(runtime_error(msg));
      return "";				       
    }

  } // end of namespace material

} // end of namespace tfel
