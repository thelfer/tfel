/*! 
 * \file  FiniteStrainBehaviourTangentOperator.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 juin 2014
 */

#include"TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

namespace tfel{

  namespace material{
    
    std::vector<FiniteStrainBehaviourTangentOperatorBase::Flag>
    getFiniteStrainBehaviourTangentOperatorFlags(void)
    {
      using namespace std;
      vector<FiniteStrainBehaviourTangentOperatorBase::Flag> res;
      res.push_back(FiniteStrainBehaviourTangentOperatorBase::DSIG_DF); 
      res.push_back(FiniteStrainBehaviourTangentOperatorBase::DSIG_DDF);
      res.push_back(FiniteStrainBehaviourTangentOperatorBase::DSIG_DD); 
      res.push_back(FiniteStrainBehaviourTangentOperatorBase::DSIG_DDE);
      res.push_back(FiniteStrainBehaviourTangentOperatorBase::DTAU_DF); 
      res.push_back(FiniteStrainBehaviourTangentOperatorBase::DTAU_DDF);
      res.push_back(FiniteStrainBehaviourTangentOperatorBase::DS_DF);   
      res.push_back(FiniteStrainBehaviourTangentOperatorBase::DS_DDF);  
      res.push_back(FiniteStrainBehaviourTangentOperatorBase::DS_DC);   
      return res;
    }

    std::string
    getFiniteStrainBehaviourTangentOperatorDescription(const FiniteStrainBehaviourTangentOperatorBase::Flag t)
    {
      using namespace std;
      switch(t){
      case FiniteStrainBehaviourTangentOperatorBase::DSIG_DF:
	return "derivative of the Cauchy stress with respect to "
	  "the deformation gradient.";
      case FiniteStrainBehaviourTangentOperatorBase::DSIG_DDF:
	return "derivative of the Cauchy stress with respect to "
	  "the deformation gradient increment";
      case FiniteStrainBehaviourTangentOperatorBase::DSIG_DD:
	return "derivative of the Cauchy stress with respect to "
	  "the deformation rate";
      case FiniteStrainBehaviourTangentOperatorBase::DSIG_DDE:
	return "derivative of the Cauchy stress with respect to "
	  "the small strain deformation";
      case FiniteStrainBehaviourTangentOperatorBase::DTAU_DF:
	return "derivative of the Kirchhoff stress with respect to "
	  "the deformation gradient increment ";
      case FiniteStrainBehaviourTangentOperatorBase::DTAU_DDF:
	return "derivative of the Kirchhoff stress with respect to "
	  "the deformation gradient increment";
      case FiniteStrainBehaviourTangentOperatorBase::DS_DF:
	return "derivative of the second Piola-Kirchoff stress "
	  "with respect to the deformation gradient";
      case FiniteStrainBehaviourTangentOperatorBase::DS_DDF:
	return "derivative of the second Piola-Kirchoff stress "
	  "with respect to the deformation gradient increment";
      case FiniteStrainBehaviourTangentOperatorBase::DS_DC:
	return "derivative of the second Piola-Kirchoff stress "
	  "with respect to the right Cauchy-Green tensor";
      }
      string msg(" getFiniteStrainBehaviourTangentOperatorDescription : "
		 "unsupported FiniteStrainBehaviourTangentOperatorBase flag ");
      throw(runtime_error(msg));
      return "";
    }


  } // end of namespace material
  
} // end of namespace tfel
