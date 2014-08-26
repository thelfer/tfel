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
      res.push_back(FiniteStrainBehaviourTangentOperatorBase::DS_DEGL);   
      return res;
    }

    std::vector<std::pair<FiniteStrainBehaviourTangentOperatorBase::Flag,
			  FiniteStrainBehaviourTangentOperatorBase::Flag> >
    getAvailableFiniteStrainBehaviourTangentOperatorConvertions(void)
    {
      using namespace std;
      return vector<pair<FiniteStrainBehaviourTangentOperatorBase::Flag,
			 FiniteStrainBehaviourTangentOperatorBase::Flag> >();
    } // end of getAvailableFiniteStrainBehaviourTangentOperatorConvertions

    std::string
    getFiniteStrainBehaviourTangentOperatorFlagType(const FiniteStrainBehaviourTangentOperatorBase::Flag f)
    {
      using namespace std;
      switch(f){
      case FiniteStrainBehaviourTangentOperatorBase::DSIG_DF:
      case FiniteStrainBehaviourTangentOperatorBase::DSIG_DDF:
      case FiniteStrainBehaviourTangentOperatorBase::DTAU_DF:
      case FiniteStrainBehaviourTangentOperatorBase::DTAU_DDF:
      case FiniteStrainBehaviourTangentOperatorBase::DS_DF:
      case FiniteStrainBehaviourTangentOperatorBase::DS_DDF:
	return "t2tost2";
      case FiniteStrainBehaviourTangentOperatorBase::DSIG_DD:
      case FiniteStrainBehaviourTangentOperatorBase::DSIG_DDE:
      case FiniteStrainBehaviourTangentOperatorBase::DS_DC:
      case FiniteStrainBehaviourTangentOperatorBase::DS_DEGL:
	return "st2tost2";
      }
      string msg("getFiniteStrainBehaviourTangentOperatorFlagType : "
		 "unsupported FiniteStrainBehaviourTangentOperatorBase flag ");
      throw(runtime_error(msg));
      return "";
    }
    
    std::string
    convertFiniteStrainBehaviourTangentOperatorFlagToString(const FiniteStrainBehaviourTangentOperatorBase::Flag f)
    {
      using namespace std;
      switch(f){
      case FiniteStrainBehaviourTangentOperatorBase::DSIG_DF:
	return "DSIG_DF";
      case FiniteStrainBehaviourTangentOperatorBase::DSIG_DDF:
	return "DSIG_DDF";
      case FiniteStrainBehaviourTangentOperatorBase::DSIG_DD:
	return "DSIG_DD";
      case FiniteStrainBehaviourTangentOperatorBase::DSIG_DDE:
	return "DSIG_DDE";
      case FiniteStrainBehaviourTangentOperatorBase::DTAU_DF:
	return "DTAU_DF";
      case FiniteStrainBehaviourTangentOperatorBase::DTAU_DDF:
	return "DTAU_DDF";
      case FiniteStrainBehaviourTangentOperatorBase::DS_DF:
	return "DS_DF";
      case FiniteStrainBehaviourTangentOperatorBase::DS_DDF:
	return "DS_DDF";
      case FiniteStrainBehaviourTangentOperatorBase::DS_DC:
	return "DS_DC";
      case FiniteStrainBehaviourTangentOperatorBase::DS_DEGL:
	return "DS_DEGL";
      }
      string msg("convertFiniteStrainBehaviourTangentOperatorFlagToString : "
		 "unsupported FiniteStrainBehaviourTangentOperatorBase flag ");
      throw(runtime_error(msg));
      return "";
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
      case FiniteStrainBehaviourTangentOperatorBase::DS_DEGL:
	return "derivative of the second Piola-Kirchoff stress "
	  "with respect to the Green-Lagrange strain tensor";
      }
      string msg("getFiniteStrainBehaviourTangentOperatorDescription : "
		 "unsupported FiniteStrainBehaviourTangentOperatorBase flag ");
      throw(runtime_error(msg));
      return "";
    }

  } // end of namespace material
  
} // end of namespace tfel
