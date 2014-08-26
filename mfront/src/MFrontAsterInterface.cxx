/*!
 * \file   MFrontAsterInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#include<sstream>
#include<cstdlib>
#include<stdexcept>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/MFrontFileDescription.hxx"
#include"MFront/MFrontAsterInterface.hxx"

namespace mfront{

  std::string
  MFrontAsterInterface::getName(void)
  {
    return "aster";
  }

  std::string
  MFrontAsterInterface::getLibraryName(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    string lib;
    if(mb.getLibrary().empty()){
      if(!mb.getMaterialName().empty()){
	lib = "libAster"+mb.getMaterialName();
      } else {
	lib = "libAsterBehaviour";
      }
    } else {
      lib = "libAster"+mb.getLibrary();
    }
    return lib;
  } // end of MFrontAsterInterface::getLibraryName

  std::string
  MFrontAsterInterface::getInterfaceName(void) const
  {
    return "Aster";
  } // end of MFrontAsterInterface::getInterfaceName

  std::string
  MFrontAsterInterface::getFunctionName(const std::string& name) const
  {
    return "aster"+makeLowerCase(name);
  } // end of MFrontAsterInterface::getLibraryName

  MFrontAsterInterface::MFrontAsterInterface()
  {
    this->reset();
  }

  void
  MFrontAsterInterface::reset(void)
  {
    MFrontUMATInterfaceBase::reset();
    this->compareToNumericalTangentOperator  = false;
    this->strainPerturbationValue            = 1.e-6;
    this->tangentOperatorComparisonCriterium = 1.e7;
    this->savesTangentOperator               = false;
    this->errorReport                        = true;
  }

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontAsterInterface::treatKeyword(const std::string& key,
				     tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				     const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end)
  {
    using namespace std;
    if (key=="@AsterGenerateMTestFileOnFailure"){
      this->generateMTestFile = this->readBooleanValue(key,current,end);
      return make_pair(true,current);      
    } else if(key=="@AsterCompareToNumericalTangentOperator"){
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterCompareToNumericalTangentOperator) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      if(current->value=="true"){
	this->compareToNumericalTangentOperator = true;
      } else if(current->value=="false"){
	this->compareToNumericalTangentOperator = false;
      } else {
	string msg("AsterInterface::treatKeyword (@AsterCompareToNumericalTangentOperator) :");
	msg += "expected 'true' or 'false'";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterCompareToNumericalTangentOperator) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }     
      if(current->value!=";"){
	string msg("AsterInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if (key=="@AsterTangentOperatorComparisonCriterium"){
      if(!this->compareToNumericalTangentOperator){
	string msg("AsterInterface::treatKeyword (@AsterTangentOperatorComparisonCriterium) : "
		   "comparison to tangent operator is not enabled at this stage.\n"
		   "Use the @AsterCompareToNumericalTangentOperator directive before "
		   "@AsterTangentOperatorComparisonCriterium");
	throw(runtime_error(msg));
      }
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterTangentOperatorComparisonCriterium) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      istringstream flux(current->value);
      flux >> this->tangentOperatorComparisonCriterium;
      if(flux.fail()){
	string msg("AsterInterface::treatKeyword (@AsterTangentOperatorComparisonCriterium) : ");
	msg+="failed to read criterium value.\n";
	throw(runtime_error(msg));
      }
      ++(current);    
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterTangentOperatorComparisonCriterium) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }  
      if(current->value!=";"){
	string msg("AsterInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if (key=="@AsterStrainPerturbationValue"){
      if(!this->compareToNumericalTangentOperator){
	string msg("AsterInterface::treatKeyword (@AsterStrainPerturbationValue) : ");
	msg += "time stepping is not enabled at this stage.\n";
	msg += "Use the @AsterUseTimeSubStepping directive before ";
	msg += "@AsterStrainPerturbationValue";
	throw(runtime_error(msg));
      }
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterStrainPerturbationValue) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      istringstream flux(current->value);
      flux >> this->strainPerturbationValue;
      if(flux.fail()){
	string msg("AsterInterface::treatKeyword (@AsterStrainPerturbationValue) : ");
	msg+="failed to read string perturbation value.\n";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterStrainPerturbationValue) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }      
      if(current->value!=";"){
	string msg("AsterInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if(key=="@AsterSaveTangentOperator"){
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterSaveTangentOperator) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      if(current->value=="true"){
	this->savesTangentOperator = true;
      } else if(current->value=="false"){
	this->savesTangentOperator = false;
      } else {
	string msg("AsterInterface::treatKeyword (@AsterSaveTangentOperator) :");
	msg += "expected 'true' or 'false'";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterSaveTangentOperator) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }      
      if(current->value!=";"){
	string msg("AsterInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if(key=="@AsterErrorReport"){
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterErrorReport) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      if(current->value=="true"){
	this->errorReport = true;
      } else if(current->value=="false"){
	this->errorReport = false;
      } else {
	string msg("AsterInterface::treatKeyword (@AsterErrorReport) :");
	msg += "expected 'true' or 'false'";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterErrorReport) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }      
      if(current->value!=";"){
	string msg("AsterInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    }
    return make_pair(false,current);
  } // end of treatKeyword

  std::set<tfel::material::ModellingHypothesis::Hypothesis>
  MFrontAsterInterface::getModellingHypothesesToBeTreated(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    using tfel::material::ModellingHypothesis;
    typedef ModellingHypothesis::Hypothesis Hypothesis;
    // treatment 
    set<Hypothesis> h;
    // modelling hypotheses handled by the behaviour
    const set<Hypothesis>& bh = mb.getModellingHypotheses();
    if(bh.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)!=bh.end()){
      h.insert(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN);
    }
    if(bh.find(ModellingHypothesis::GENERALISEDPLANESTRAIN)!=bh.end()){
      h.insert(ModellingHypothesis::GENERALISEDPLANESTRAIN);
    }
    if(bh.find(ModellingHypothesis::TRIDIMENSIONAL)!=bh.end()){
      h.insert(ModellingHypothesis::TRIDIMENSIONAL);
    }
    if(h.empty()){
      string msg("MFrontAsterInterfaceModellingHypothesesToBeTreated : "
		 "no hypotheses selected. This means that the given beahviour "
		 "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
		 "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
		 "make sense to use the Aster interface");
      throw(runtime_error(msg));
    }
    return h;
  } // end of MFrontAsterInterface::getModellingHypothesesToBeTreated

  void
  MFrontAsterInterface::endTreatement(const MechanicalBehaviourDescription& mb,
				      const MFrontFileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;
    string name;
    string asterFctName;
    string tmp;

    if(mb.getBehaviourType()!=MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      string msg("MFrontAsterInterface::endTreatement : "
		 "the aster interface only supports small strain behaviours");
      throw(runtime_error(msg));
    }

    // get the modelling hypotheses to be treated
    const set<Hypothesis>& h = this->getModellingHypothesesToBeTreated(mb);

    if(!mb.getLibrary().empty()){
      name += mb.getLibrary();
    }
    name += mb.getClassName();
    
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Aster");

    // header
    string fileName("aster");
    fileName += name;
    fileName += ".hxx";
    ofstream out(("include/MFront/Aster/"+fileName).c_str());
    if(!out){
      string msg("MFrontAsterInterface::endTreatement : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }

    out << "/*!\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file declares the aster interface for the " 
	<< mb.getClassName() << " behaviour law\n";
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/\n\n";

    const string header = this->getHeaderDefine(mb);
    out << "#ifndef "<< header << "\n";
    out << "#define "<< header << "\n\n";

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    out << "#include\"MFront/Aster/Aster.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/Aster/AsterTraits.hxx\"\n";
    if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/Aster/AsterOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#define aster" 
	<< makeUpperCase(name)
	<< "_F77 \\\n"
	<< "        F77_FUNC(aster"
	<< makeLowerCase(name) << ",Aster"
	<< makeUpperCase(name) << ")\n\n";

    asterFctName = "aster"+makeUpperCase(name)+"_F77";

    out << "#ifdef __cplusplus\n\n";

    out << "namespace aster{\n\n";

    if(!mb.areAllMechanicalDataSpecialised(h)){
      this->writeAsterBehaviourTraits(out,mb,ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
      if(mb.hasSpecialisedMechanicalData(*ph)){
	this->writeAsterBehaviourTraits(out,mb,*ph);
      }
    }

    out << "} // end of namespace aster\n\n";

    out << "#endif /* __cplusplus */\n\n";

    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeSetParametersFunctionsDeclarations(out,name,mb);

    out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL\naster"
	<< makeLowerCase(name) << "("
	<< "aster::AsterReal *const,"       /*< tenseur des contraintes */
	<< "aster::AsterReal *const,"       /*< variables internes */
	<< "aster::AsterReal *const,"       /*< matrice jacobienne du modèle */
	<< "aster::AsterReal *const,"       /*< énergie de déformation élastique */
	<< "aster::AsterReal *const,"       /*< dissipation plastique */
	<< "aster::AsterReal *const,"       /*< dissipation visqueuse */
	<< "aster::AsterReal *const,"       /*< puissance calorifique volumique degagee */
	<< "aster::AsterReal *const,"       /*< derivée des contraintes par rapport à la température */
	<< "aster::AsterReal *const,"       /*< derivées de RPL par rapport aux déformations */
	<< "aster::AsterReal *const,"       /*< derivée de RPL par rapport à la témperature */
	<< "const aster::AsterReal *const," /*< tenseur des déformations totales au début du pas */
	<< "const aster::AsterReal *const," /*< tenseur des incréments de déformation totale */
	<< "const aster::AsterReal *const," /*< temps courant TIME[0] = 0," TIME[1] = t0 */
	<< "const aster::AsterReal *const," /*< incrément de temps */
	<< "const aster::AsterReal *const," /*< température au début du pas */
	<< "const aster::AsterReal *const," /*< incrément de température */
	<< "const aster::AsterReal *const," /*< variables externes au début du pas */
	<< "const aster::AsterReal *const," /*< incréments des variables externes */
	<< "const char     *const," /*< identifiant de la loi de comportement   */
	<< "const aster::AsterInt  *const," /*< entier définissant le type de calcul  */
	<< "const aster::AsterInt  *const," /*< nombre de composantes extradiagonales */
	<< "const aster::AsterInt  *const," /*< nombre de composantes du tenseur des contraintes */
	<< "const aster::AsterInt  *const," /*< nombre de variables internes */
	<< "const aster::AsterReal *const," /*< propriétés du matériaux */
	<< "const aster::AsterInt  *const," /*< nombre de propriétés matériaux */
	<< "const aster::AsterReal *const," /*< coordonnées du point d'intégration */
	<< "const aster::AsterReal *const," /*< matrice de passage du repère local  
					      de l'élement fini massif au repère géneral
					      du maillage */
	<< "aster::AsterReal *const," /*< rapport entre le nouveau pas de temps 
					suggeré et le pas de temps donné en entrée */
	<< "const aster::AsterReal *const," /*< longueur caractéristique de l'élément */
	<< "const aster::AsterReal *const," /*< tenseur gradient de déplacement au début du pas */
	<< "const aster::AsterReal *const," /*< tenseur gradient de déplacement au fin du pas */
	<< "const aster::AsterInt  *const," /*< numéro de l'élément courant */
	<< "const aster::AsterInt  *const," /*< numéro du point d'intégration courant */
	<< "const aster::AsterInt  *const," /*< numero de couche pour des coques
					      composites ou des solides multi-couches  */
	<< "const aster::AsterInt  *const," /*< numero de section dans la couche courante */
	<< "const aster::AsterInt  *const," /*< entrée non utilisée dans Cast3M  */
	<< "aster::AsterInt  *const" /*< sortie d'erreur */
	<< ");\n\n";

    out << "#ifdef __cplusplus\n";
    out << "}\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "#endif /* " << header << " */\n";

    out.close();

    fileName  = "aster";
    fileName += name;
    fileName += ".cxx";

    out.open(("src/"+fileName).c_str());
    if(!out){
      string msg("MFrontAsterInterface::endTreatement : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }

    out << "/*!\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file implements the aster interface for the " 
	<< mb.getClassName() << " behaviour law\n";
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/\n\n";

    this->getExtraSrcIncludes(out,mb);

    if(this->compareToNumericalTangentOperator){
      out << "#include<cmath>\n";
      out << "#include<vector>\n";
    }
    if((this->compareToNumericalTangentOperator)||
       (this->savesTangentOperator)){
      out << "#include<algorithm>\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "#include\"MFront/MFrontBehaviourProfiler.hxx\"\n\n";
    }
    out << "#include\"MFront/Aster/AsterStressFreeExpansionHandler.hxx\"\n\n";
    out << "#include\"MFront/Aster/AsterInterface.hxx\"\n\n";
    out << "#include\"MFront/Aster/aster" << name << ".hxx\"\n\n";

    out << "extern \"C\"{\n\n";
 
    this->generateUMATxxGeneralSymbols(out,name,mb,fd);
    if(!mb.areAllMechanicalDataSpecialised(h)){
      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      this->generateUMATxxSymbols(out,name,uh,mb,fd);
    }
    for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
      if(mb.hasSpecialisedMechanicalData(*ph)){
	this->generateUMATxxSymbols(out,name,*ph,mb,fd);
      }
    }
    
    this->writeSetParametersFunctionsImplementations(out,name,mb);

    out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL\n"
	<< this->getFunctionName(name) << "("
	<< "aster::AsterReal *const STRESS,"       /*< tenseur des contraintes */
	<< "aster::AsterReal *const STATEV,"       /*< variables internes */
	<< "aster::AsterReal *const DDSOE,"       /*< matrice jacobienne du modèle */
	<< "aster::AsterReal *const,"       /*< énergie de déformation élastique */
	<< "aster::AsterReal *const,"       /*< dissipation plastique */
	<< "aster::AsterReal *const,"       /*< dissipation visqueuse */
	<< "aster::AsterReal *const,"       /*< puissance calorifique volumique degagee */
	<< "aster::AsterReal *const,"       /*< derivée des contraintes par rapport à la température */
	<< "aster::AsterReal *const,"       /*< derivées de RPL par rapport aux déformations */
	<< "aster::AsterReal *const,"       /*< derivée de RPL par rapport à la témperature */
	<< "const aster::AsterReal *const STRAN," /*< tenseur des déformations totales au début du pas */
	<< "const aster::AsterReal *const DSTRAN," /*< tenseur des incréments de déformation totale */
	<< "const aster::AsterReal *const," /*< temps courant TIME[0] = 0," TIME[1] = t0 */
	<< "const aster::AsterReal *const DTIME," /*< incrément de temps */
	<< "const aster::AsterReal *const TEMP," /*< température au début du pas */
	<< "const aster::AsterReal *const DTEMP," /*< incrément de température */
	<< "const aster::AsterReal *const PREDEF," /*< variables externes au début du pas */
	<< "const aster::AsterReal *const DPRED," /*< incréments des variables externes */
	<< "const char     *const," /*< identifiant de la loi de comportement   */
	<< "const aster::AsterInt  *const," /*< entier définissant le type de calcul  */
	<< "const aster::AsterInt  *const," /*< nombre de composantes extradiagonales */
	<< "const aster::AsterInt  *const NTENS," /*< nombre de composantes du tenseur des contraintes */
	<< "const aster::AsterInt  *const NSTATV," /*< nombre de variables internes */
	<< "const aster::AsterReal *const PROPS," /*< propriétés du matériaux */
	<< "const aster::AsterInt  *const NPROPS," /*< nombre de propriétés matériaux */
	<< "const aster::AsterReal *const," /*< coordonnées du point d'intégration */
	<< "const aster::AsterReal *const DROT," /*< matrice de passage du repère local  
						   de l'élement fini massif au repère géneral
						   du maillage */
	<< "aster::AsterReal *const PNEWDT," /*< rapport entre le nouveau pas de temps 
					       suggeré et le pas de temps donné en entrée */
	<< "const aster::AsterReal *const," /*< longueur caractéristique de l'élément */
	<< "const aster::AsterReal *const," /*< tenseur gradient de déplacement au début du pas */
	<< "const aster::AsterReal *const," /*< tenseur gradient de déplacement au fin du pas */
	<< "const aster::AsterInt  *const," /*< numéro de l'élément courant */
	<< "const aster::AsterInt  *const," /*< numéro du point d'intégration courant */
	<< "const aster::AsterInt  *const," /*< numero de couche pour des coques
					      composites ou des solides multi-couches  */
	<< "const aster::AsterInt  *const," /*< numero de section dans la couche courante */
	<< "const aster::AsterInt  *const," /*< entrée non utilisée dans Cast3M  */
	<< "aster::AsterInt  *const" /*< sortie d'erreur */
	<< ")\n";
    out << "{\n";
    if(((getDebugMode())||(this->compareToNumericalTangentOperator)||
	(this->savesTangentOperator))&&(!this->generateMTestFile)){
      out << "using namespace std;\n";
    }
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "using mfront::MFrontBehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "MFrontBehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "MFrontBehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    if((getDebugMode())||(this->compareToNumericalTangentOperator)){
      out << "const bool computeTangentOperator = (*DDSOE>0.5);\n";
    }
    if(this->compareToNumericalTangentOperator){
      out << "vector<aster::AsterReal> deto0(*NTENS);\n";
      out << "vector<aster::AsterReal> sig0(*NTENS);\n";
      out << "vector<aster::AsterReal> sv0(*NSTATV);\n";
      out << "copy(DSTRAN,DSTRAN+*(NTENS),deto0.begin());\n";
      out << "copy(STRESS,STRESS+*(NTENS),sig0.begin());\n";
      out << "copy(STATEV,STATEV+*(NSTATV),sv0.begin());\n";
    }
    if(!this->savesTangentOperator){
      out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName() 
	  << ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,STATEV,NSTATV,STRESS,\n"
	  << "aster::AsterStandardSmallStrainStressFreeExpansionHandler)!=0){\n";
      this->generateMTestFile2(out,MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
       			       name,"",mb);
      out << "*PNEWDT = -1.;\n";
      out << "return;\n";
      out << "}\n";
    } else {
      out << "if(*(NSTATV)<(*NTENS)*(*NTENS)){\n"
	  << "string msg(\"aster" << makeLowerCase(name) 
	  << ": invalid number of state variables (can't save tangent operator)\");\n"
	  << "throw(runtime_error(msg));\n"
	  << "}\n";
      out << "aster::AsterInt nNSTATV = max(*(NSTATV)-(*NTENS)*(*NTENS),aster::AsterInt(1));\n";
      out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName() 
	  << ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,STATEV,&nNSTATV,STRESS,\n"
	  << "aster::AsterStandardSmallStrainStressFreeExpansionHandler)!=0){\n";
      this->generateMTestFile2(out,MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
			       name,"",mb);
      out << "*PNEWDT = -1.;\n";
      out << "return;\n";
      out << "}\n";
      out << "copy(DDSOE,DDSOE+(*NTENS)*(*NTENS),STATEV+*(NSTATV)-(*NTENS)*(*NTENS));\n";
    }
    if(getDebugMode()){
      out << "if(computeTangentOperator){\n";
      out << "aster::AsterInt i;\n";
      out << "aster::AsterInt j;\n";
      out << "cout << \"Dt :\" << endl;\n";
      out << "for(i=0;i!=*NTENS;++i){\n";
      out << "for(j=0;j!=*NTENS;++j){\n";
      out << "cout << *(DDSOE+j*(*NTENS)+i) << \" \";\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
    }
    if(this->compareToNumericalTangentOperator){
      out << "if(computeTangentOperator){\n";
      out << "// computing the tangent operator by pertubation\n";
      out << "aster::AsterInt i;\n";
      out << "aster::AsterInt j;\n";
      out << "vector<aster::AsterReal> nD((*NTENS)*(*NTENS));\n";
      out << "vector<aster::AsterReal> deto(*NTENS);\n";
      out << "vector<aster::AsterReal> sigf(*NTENS);\n";
      out << "vector<aster::AsterReal> sigb(*NTENS);\n";
      out << "vector<aster::AsterReal> sv(*NSTATV);\n";
      out << "vector<aster::AsterReal> D((*NTENS)*(*NTENS));\n";
      out << "aster::AsterReal m;\n";
      out << "aster::AsterReal mDt;\n";
      out << "aster::AsterReal mnDt;\n";
      out << "for(i=0;i!=*NTENS;++i){\n";
      out << "copy(deto0.begin(),deto0.end(),deto.begin());\n";
      out << "copy(sig0.begin(),sig0.end(),sigf.begin());\n";
      out << "copy(sv0.begin(),sv0.end(),sv.begin());\n";
      out << "deto[i] += " << this->strainPerturbationValue << ";\n";
      out << "D[0] = 0.;\n";
      if(!this->savesTangentOperator){
	out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName() 
	    << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,NPROPS,"
	    << "PREDEF,DPRED,&sv[0],NSTATV,&sigf[0],\n"
	    << "aster::AsterStandardSmallStrainStressFreeExpansionHandler)!=0){\n";
      } else {
	out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName() 
	    << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,NPROPS,"
	    << "PREDEF,DPRED,&sv[0],&nNSTATV,&sigf[0],\n"
	    << "aster::AsterStandardSmallStrainStressFreeExpansionHandler)!=0){\n";
      }
      out << "return;\n";
      out << "}\n";
      out << "copy(deto0.begin(),deto0.end(),deto.begin());\n";
      out << "copy(sig0.begin(),sig0.end(),sigb.begin());\n";
      out << "copy(sv0.begin(),sv0.end(),sv.begin());\n";
      out << "deto[i] -= " << this->strainPerturbationValue << ";\n";
      out << "D[0] = 0.;\n";
      if(!this->savesTangentOperator){
	out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName() 
	    << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,NPROPS,"
	    << "PREDEF,DPRED,&sv[0],NSTATV,&sigb[0],\n"
	    << "aster::AsterStandardSmallStrainStressFreeExpansionHandler)!=0){\n";
      } else {
	out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName() 
	    << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,NPROPS,"
	    << "PREDEF,DPRED,&sv[0],&nNSTATV,&sigb[0],\n"
	    << "aster::AsterStandardSmallStrainStressFreeExpansionHandler)!=0){\n";
      }
      out << "return;\n";
      out << "}\n";
      out << "for(j=0;j!=*NTENS;++j){\n";
      out << "nD[j*(*NTENS)+i] = (sigf[j]-sigb[j])/(2.*" << this->strainPerturbationValue << ");\n";
      out << "}\n";
      out << "}\n";
      out << "// comparison\n";
      out << "m=0.;\n";
      out << "mDt=0.;\n";
      out << "mnDt=0.;\n";
      out << "for(i=0;i!=(*NTENS)*(*NTENS);++i){\n";
      out << "mDt=max(mDt,*(DDSOE+i));\n";
      out << "mnDt=max(mnDt,nD[i]);\n";
      out << "m=max(m,abs(nD[i]-*(DDSOE+i)));\n";
      out << "}\n";
      out << "if(m>" << this->tangentOperatorComparisonCriterium << "){\n";
      out << "cout << \"||nDt-Dt|| = \" << m << \" (\" << 100.*m/(0.5*(mDt+mnDt)) << \"%)\"<< endl;\n";
      out << "cout << \"Dt :\" << endl;\n";
      out << "for(i=0;i!=*NTENS;++i){\n";
      out << "for(j=0;j!=*NTENS;++j){\n";
      out << "cout << *(DDSOE+j*(*NTENS)+i) << \" \";\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
      out << "cout << \"nDt :\" << endl;\n";
      out << "for(i=0;i!=*NTENS;++i){\n";
      out << "for(j=0;j!=*NTENS;++j){\n";
      out << "cout << nD[j*(*NTENS)+i] << \" \";\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
      out << "}\n";
    }
    out << "}\n\n";
    out << "} // end of extern \"C\"\n";
    out.close();
  } // end of MFrontAsterInterface::endTreatement

  void
  MFrontAsterInterface::writeMTestFileGeneratorSetModellingHypothesis(std::ostream& out) const
  {
    out << "ModellingHypothesis::Hypothesis h;\n";
    out << "if(*NTENS==3u){\n";
    out << "  h = ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN;\n";
    out << "} else if(*NTENS==4){\n";
    out << "  h = ModellingHypothesis::GENERALISEDPLANESTRAIN;\n";
    out << "} else if(*NTENS==6){\n";
    out << "  h = ModellingHypothesis::TRIDIMENSIONAL;\n";
    out << "} else {\n";
    out << "  return;\n";
    out << "}\n";
    out << "mg.setModellingHypothesis(h);\n";
  } // end of MFrontAsterInterface::writeMTestFileGeneratorSetModellingHypothesis
  
  void 
  MFrontAsterInterface::writeInterfaceSpecificIncludes(std::ofstream& out,
						       const MechanicalBehaviourDescription&) const
  {
    using namespace std;
    out << "#include\"MFront/Aster/Aster.hxx\"" << endl << endl;
  } // end of MFrontAsterInterface::writeInterfaceSpecificIncludes

  MFrontAsterInterface::~MFrontAsterInterface()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontAsterInterface::getGlobalIncludes(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    map<string,vector<string> > incs;
    string lib = MFrontAsterInterface::getLibraryName(mb);
    incs[lib].push_back("`tfel-config --includes`");
#if ASTER_ARCH == 64
    incs[lib].push_back("-DASTER_ARCH=64");
#elif ASTER_ARCH == 32
    incs[lib].push_back("-DASTER_ARCH=32");
#else
#error "MFrontAsterInterface::getGlobalIncludes : unsuported architecture"
#endif
    return incs;
  } // end of MFrontAsterInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontAsterInterface::getGeneratedSources(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    map<string,vector<string> > src;
    string lib = MFrontAsterInterface::getLibraryName(mb);
    if(!mb.getLibrary().empty()){
      src[lib].push_back("aster"+mb.getLibrary()+mb.getClassName()+".cxx");
    } else {
      src[lib].push_back("aster"+mb.getClassName()+".cxx");
    }
    return src;
  } // end of MFrontAsterInterface::getGeneratedSources

  std::vector<std::string>
  MFrontAsterInterface::getGeneratedIncludes(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    vector<string> incs;
    if(!mb.getLibrary().empty()){
      incs.push_back("MFront/Aster/aster"+mb.getLibrary()+mb.getClassName()+".hxx");
    } else {
      incs.push_back("MFront/Aster/aster"+mb.getClassName()+".hxx");
    }
    return incs;
  } // end of MFrontAsterInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontAsterInterface::getLibrariesDependencies(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    map<string,vector<string> > deps;
    string lib = MFrontAsterInterface::getLibraryName(mb);
    deps[lib].push_back("-lAsterInterface");
#ifdef HAVE_CXX11
      deps[lib].push_back("`tfel-config --libs --material --mfront-profiling`");
#else 
      deps[lib].push_back("`tfel-config --libs --material`");
#endif
    return deps;
  } // end of MFrontAsterInterface::getLibrariesDependencies()

  std::pair<std::vector<MFrontUMATInterfaceBase::UMATMaterialProperty>,
	    SupportedTypes::TypeSize>
  MFrontAsterInterface::buildMaterialPropertiesList(const MechanicalBehaviourDescription& mb,
						     const Hypothesis h) const
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      const set<Hypothesis> ah = this->getModellingHypothesesToBeTreated(mb);
      set<Hypothesis> uh;
      for(set<Hypothesis>::const_iterator ph=ah.begin();ph!=ah.end();++ph){
	if(!mb.hasSpecialisedMechanicalData(*ph)){
	  uh.insert(*ph);
	}
      }
      if(uh.empty()){
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error : the mechanical behaviour says that not "
	  "all handled mechanical data are specialised, but we found none.";
	throw(runtime_error(msg));
      }
      // material properties for all the selected hypothesis
      vector<pair<vector<UMATMaterialProperty>,
		  SupportedTypes::TypeSize> > mpositions;
      for(set<Hypothesis>::const_iterator ph=uh.begin();ph!=uh.end();++ph){
	mpositions.push_back(this->buildMaterialPropertiesList(mb,*ph));
      }
      set<Hypothesis>::const_iterator ph=uh.begin();
      vector<pair<vector<UMATMaterialProperty>,
		  SupportedTypes::TypeSize> >::const_iterator pum = mpositions.begin();
      const pair<vector<UMATMaterialProperty>,
		 SupportedTypes::TypeSize>& mfirst = *pum;
      ++ph;
      ++pum;
      for(;ph!=uh.end();++ph,++pum){
	const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
	const VariableDescriptionContainer& mps = d.getMaterialProperties();
	for(VariableDescriptionContainer::const_iterator pm=mps.begin();pm!=mps.end();++pm){
	  const UMATMaterialProperty& mp1 = findUMATMaterialProperty(mfirst.first,
								     mb.getGlossaryName(h,pm->name));
	  const UMATMaterialProperty& mp2 = findUMATMaterialProperty(pum->first,
								     mb.getGlossaryName(h,pm->name));
	  SupportedTypes::TypeSize o1 = mp1.offset;
	  o1+=pum->second;
	  SupportedTypes::TypeSize o2 = mp2.offset;
	  o2+=mfirst.second;
	  if(o1!=o2){
	    string msg("MFrontUMATInterface::buildMaterialPropertiesList : "
		       "incompatible offset for material property '"+pm->name+
		       "' (aka '"+mp1.name+"'). This is one pitfall of the umat interface. "
		       "To by-pass this limitation, you may want to explicitely "
		       "specialise some modelling hypotheses");
	    throw(runtime_error(msg));
	  }
	}
      }
      return mfirst;
    }
    pair<vector<UMATMaterialProperty>,
	 SupportedTypes::TypeSize> res;
    vector<UMATMaterialProperty>& mprops = res.first;
    if((h!=ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)&&
       (h!=ModellingHypothesis::GENERALISEDPLANESTRAIN)&&
       (h!=ModellingHypothesis::TRIDIMENSIONAL)){
      string msg("MFrontAsterInterface::buildMaterialPropertiesList : "
		 "unsupported modelling hypothesis");
      if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg += " (default)";
      } else {
	msg += " '"+ModellingHypothesis::toString(h)+"'";
      }
      throw(runtime_error(msg));
    }
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus","youn",false);
	this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio","nu",false);
      } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus1","yg1",false);
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus2","yg2",false);
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus3","yg3",false);
	this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio12","nu12",false);
	this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio23","nu23",false);
	this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio13","nu13",false);
	if (h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus12","g12",false);
	} else if (h==ModellingHypothesis::TRIDIMENSIONAL){
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus12","g12",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus23","g23",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus13","g13",false);
	} else if(h!=ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	  string msg("MFrontAsterInterface::buildMaterialPropertiesList : "
		     "unsupported modelling hypothesis");
	  throw(runtime_error(msg));
	}
      } else {
	string msg("MFrontAsterInterface::buildMaterialPropertiesList : "
		   "unsupported behaviour symmetry type");
	throw(runtime_error(msg));
      }
    }
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion","alph",false);
      } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion1","alp1",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion2","alp2",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion3","alp3",false);
      } else {
	string msg("MFrontAsterInterface::buildMaterialPropertiesList : "
		   "unsupported behaviour symmetry type");
	throw(runtime_error(msg));
      }
    }
    if(!mprops.empty()){
      const UMATMaterialProperty& m = mprops.back();
      res.second  = m.offset;
      res.second += this->getTypeSize(m.type,m.arraySize);
    }
    this->completeMaterialPropertiesList(mprops,mb,h);
    return res;
  } // end of MFrontAsterInterface::buildMaterialPropertiesList
    
  void
  MFrontAsterInterface::writeUMATxxAdditionalSymbols(std::ostream& out,
						     const std::string& name,
						     const Hypothesis h,
						     const MechanicalBehaviourDescription&,
						     const MFrontFileDescription&) const
  {
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name,h);
    out << "_savesTangentOperator = ";
    if(this->savesTangentOperator){
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";
  } // end of MFrontAsterInterface::writeUMATxxAdditionalSymbols

  void
  MFrontAsterInterface::writeAsterBehaviourTraits(std::ostream& out,
						  const MechanicalBehaviourDescription& mb,
						  const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::material;
    const pair<SupportedTypes::TypeSize,
	       SupportedTypes::TypeSize> mvs = mb.getMainVariablesSize();
    pair<vector<UMATMaterialProperty>,
	 SupportedTypes::TypeSize> mprops = this->buildMaterialPropertiesList(mb,h);
     if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type";
      if(mb.useQt()){
	out << ",bool use_qt";
      }
    } else {
      out << "template<tfel::material::ModellingHypothesis::"
	  << ModellingHypothesis::toUpperCaseString(h) << ",typename Type";
      if(mb.useQt()){
	out << ",bool use_qt";
      }
    }
    out << ">\n";
    out << "struct AsterTraits<tfel::material::" << mb.getClassName() << "<H,Type,";
    if(mb.useQt()){
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >\n{\n";
    out << "// space dimension\n";
    out << "static const unsigned short N           = tfel::material::ModellingHypothesisToSpaceDimension<H>::value;\n";
    out << "// tiny vector size\n";
    out << "static const unsigned short TVectorSize = N;\n";
    out << "// symmetric tensor size\n";
    out << "static const unsigned short StensorSize = tfel::math::StensorDimeToSize<N>::value;\n";
    out << "// tensor size\n";
    out << "static const unsigned short TensorSize  = tfel::math::TensorDimeToSize<N>::value;\n";
    out << "// size of the driving variable array (STRAN)\n";
    out << "static const unsigned short DrivingVariableSize = " << mvs.first <<  ";\n";
    out << "// size of the thermodynamic force variable array (STRESS)\n";
    out << "static const unsigned short ThermodynamicForceVariableSize = " << mvs.second <<  ";\n";
    if(this->errorReport){
      out << "static const AsterErrorReportPolicy errorReportPolicy = ASTER_WRITEONSTDOUT;\n";
    } else {
      out << "static const AsterErrorReportPolicy errorReportPolicy = ASTER_NOERRORREPORT;\n";
    }
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
      out << "static const bool requiresStiffnessTensor = true;\n";
    } else {
      out << "static const bool requiresStiffnessTensor = false;\n";
    }
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      out << "static const bool requiresThermalExpansionCoefficientTensor = true;\n";
    } else {
      out << "static const bool requiresThermalExpansionCoefficientTensor = false;\n";
    }
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      out << "static const AsterSymmetryType type = aster::ISOTROPIC;\n";
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "static const AsterSymmetryType type = aster::ORTHOTROPIC;\n";
    } else {
      string msg("MFrontAsterInterface::endTreatement : ");
      msg += "unsupported behaviour type.\n";
      msg += "The aster interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    // computing material properties size
    SupportedTypes::TypeSize msize;
    if(!mprops.first.empty()){
      const UMATMaterialProperty& m = mprops.first.back();
      msize  = m.offset;
      msize += this->getTypeSize(m.type,m.arraySize);
      msize -= mprops.second;
    }
    out << "static const unsigned short material_properties_nb = " << msize << ";\n";
    if(mb.getElasticSymmetryType()==mfront::ISOTROPIC){
      out << "static const AsterSymmetryType etype = aster::ISOTROPIC;\n";
      if(mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
	out << "static const unsigned short elasticPropertiesOffset = 2u;\n";
      } else {
	out << "static const unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	out << "static const unsigned short thermalExpansionPropertiesOffset = 1u;\n"; 
      } else {
	out << "static const unsigned short thermalExpansionPropertiesOffset = 0u;\n"; 
      }
    } else if (mb.getElasticSymmetryType()==mfront::ORTHOTROPIC){
      out << "static const AsterSymmetryType etype = aster::ORTHOTROPIC;\n";
      if(mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
    	out << "static const unsigned short elasticPropertiesOffset "
    	    << "= AsterOrthotropicElasticPropertiesOffset<N>::value;\n";
      } else {
	out << "static const unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	out << "static const unsigned short thermalExpansionPropertiesOffset = 3u;\n"; 
      } else {
	out << "static const unsigned short thermalExpansionPropertiesOffset = 0u;\n"; 
      }
    } else {
      string msg("MFrontAsterInterface::endTreatement : ");
      msg += "unsupported behaviour type.\n";
      msg += "The aster interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    out << "}; // end of class AsterTraits\n\n";
  }

  std::string
  MFrontAsterInterface::getModellingHypothesisTest(const Hypothesis h) const
  {
    using namespace std;
    if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      return "*NTENS==3";
    } else if(h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      return "*NTENS==4";
    } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
      return "*NTENS==6";
    }
    string msg("MFrontAsterInterface::getModellingHypothesisTest : "
	       "unsupported modelling hypothesis");
    throw(runtime_error(msg));
    return "";
  } // end of MFrontUMATInterface::gatherModellingHypothesesAndTests

} // end of namespace mfront
