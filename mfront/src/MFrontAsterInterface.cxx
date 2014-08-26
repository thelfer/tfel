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
	msg+="failed to read criterium value.";
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
	msg+="failed to read string perturbation value.";
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

    if(!((mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL))){
      string msg("MFrontAsterInterface::endTreatement : "
		 "the aster interface only supports small and finite strain behaviours, and "
		 "cohesive zone model");
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

    out << "/*!" << endl;
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file declares the aster interface for the " 
	<< mb.getClassName() << " behaviour law" << endl;
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/" << endl << endl;

    const string header = this->getHeaderDefine(mb);
    out << "#ifndef "<< header << endl;
    out << "#define "<< header << endl << endl;

    out << "#include\"TFEL/Config/TFELConfig.hxx\"" << endl << endl;
    out << "#include\"MFront/Aster/Aster.hxx\"" << endl << endl;

    out << "#ifdef __cplusplus" << endl;
    out << "#include\"MFront/Aster/AsterTraits.hxx\"" << endl;
    if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/Aster/AsterOrthotropicBehaviour.hxx\"" << endl;
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"" << endl;
    out << "#endif /* __cplusplus */" << endl << endl;

    this->writeVisibilityDefines(out);

    out << "#define aster" 
	<< makeUpperCase(name)
	<< "_F77 \\" << endl
	<< "        F77_FUNC(aster"
	<< makeLowerCase(name) << ",Aster"
	<< makeUpperCase(name) << ")" << endl << endl;

    asterFctName = "aster"+makeUpperCase(name)+"_F77";

    out << "#ifdef __cplusplus" << endl << endl;

    out << "namespace aster{" << endl << endl;

    if(!mb.areAllMechanicalDataSpecialised(h)){
      this->writeAsterBehaviourTraits(out,mb,ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
      if(mb.hasSpecialisedMechanicalData(*ph)){
	this->writeAsterBehaviourTraits(out,mb,*ph);
      }
    }

    out << "} // end of namespace aster" << endl << endl;

    out << "#endif /* __cplusplus */" << endl << endl;

    out << "#ifdef __cplusplus" << endl;
    out << "extern \"C\"{" << endl;
    out << "#endif /* __cplusplus */" << endl << endl;

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
	<< ");" << endl << endl;

    out << "#ifdef __cplusplus" << endl;
    out << "}" << endl;
    out << "#endif /* __cplusplus */" << endl << endl;

    out << "#endif /* " << header << " */" << endl;

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

    string sfeh;
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      sfeh = "aster::AsterStandardSmallStrainStressFreeExpansionHandler";
    } else if (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      sfeh = "0";
    } else if (mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL){
      sfeh = "0";
    } else {
      string msg("MFrontAsterInterface::endTreatement : "
		 "the aster interface only supports small and finite strain behaviours");
      throw(runtime_error(msg));
    }

    out << "/*!" << endl;
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file implements the aster interface for the " 
	<< mb.getClassName() << " behaviour law" << endl;
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/" << endl << endl;

    this->getExtraSrcIncludes(out,mb);

    if(this->compareToNumericalTangentOperator){
      out << "#include<cmath>" << endl;
      out << "#include<vector>" << endl;
    }
    if((this->compareToNumericalTangentOperator)||
       (this->savesTangentOperator)){
      out << "#include<algorithm>" << endl;
    }
    if((this->compareToNumericalTangentOperator)&&
       (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      out << "#include\"TFEL/Math/tensor.hxx\""  << endl;
      out << "#include\"TFEL/Math/t2tost2.hxx\"" << endl;
      out << "#include\"TFEL/Math/t2tot2.hxx\""  << endl;
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"" << endl;
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "#include\"MFront/MFrontBehaviourProfiler.hxx\"\n" << endl;
    }
    out << "#include\"MFront/Aster/AsterStressFreeExpansionHandler.hxx\"\n" << endl;
    out << "#include\"MFront/Aster/AsterInterface.hxx\"\n" << endl;
    out << "#include\"MFront/Aster/aster" << name << ".hxx\"\n" << endl;

    if((this->compareToNumericalTangentOperator)&&
       (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      out << "namespace aster{" << endl << endl;
      out << "static void" << endl
	  << "getDeformationGradientIndexes(AsterInt& mi,AsterInt& mj,const AsterInt i,const AsterInt ntens){" << endl;
      out << "if(ntens==3){" << endl
	  << "switch(i){" << endl
	  << "case 0:" << endl
	  << "  mi=0;" << endl
	  << "  mj=0;" << endl
	  << "  break;" << endl
	  << "case 1:" << endl
	  << "  mi=1;" << endl
	  << "  mj=1;" << endl
	  << "  break;" << endl
	  << "default:" << endl
	  << "  mi=2;" << endl
	  << "  mj=2;" << endl
	  << "}" << endl
	  << "} else if(ntens==4){" << endl
	  << "switch(i){" << endl
	  << "case 0:" << endl
	  << "  mi=0;" << endl
	  << "  mj=0;" << endl
	  << "  break;" << endl
	  << "case 1:" << endl
	  << "  mi=1;" << endl
	  << "  mj=1;" << endl
	  << "  break;" << endl
	  << "case 2:" << endl
	  << "  mi=2;" << endl
	  << "  mj=2;" << endl
	  << "  break;" << endl
	  << "case 3:" << endl
	  << "  mi=0;" << endl
	  << "  mj=1;" << endl
	  << "  break;" << endl
	  << "default:" << endl
	  << "  mi=1;" << endl
	  << "  mj=0;" << endl
	  << "}" << endl
	  << "} else {" << endl
	  << "switch(i){" << endl
	  << "case 0:" << endl
	  << "  mi=0;" << endl
	  << "  mj=0;" << endl
	  << "  break;" << endl
	  << "case 1:" << endl
	  << "  mi=1;" << endl
	  << "  mj=1;" << endl
	  << "  break;" << endl
	  << "case 2:" << endl
	  << "  mi=2;" << endl
	  << "  mj=2;" << endl
	  << "  break;" << endl
	  << "case 3:" << endl
	  << "  mi=0;" << endl
	  << "  mj=1;" << endl
	  << "  break;" << endl
	  << "case 4:" << endl
	  << "  mi=1;" << endl
	  << "  mj=0;" << endl
	  << "  break;" << endl
	  << "case 5:" << endl
	  << "  mi=0;" << endl
	  << "  mj=2;" << endl
	  << "  break;" << endl
	  << "case 6:" << endl
	  << "  mi=2;" << endl
	  << "  mj=0;" << endl
	  << "  break;" << endl
	  << "case 7:" << endl
	  << "  mi=1;" << endl
	  << "  mj=2;" << endl
	  << "  break;" << endl
	  << "default:" << endl
	  << "  mi=2;" << endl
	  << "  mj=1;" << endl
	  << "}" << endl
	  << "}" << endl
	  << "}" << endl
	  << "} // end of namespace aster" << endl << endl;
    }
    
    out << "extern \"C\"{\n" << endl;
 
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

    out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL" << endl
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
	<< ")" << endl;
    out << "{" << endl;
    if(((this->compareToNumericalTangentOperator)||
	(this->savesTangentOperator))&&(!this->generateMTestFile)){
      out << "using namespace std;" << endl;
    }
    if((this->compareToNumericalTangentOperator)&&
       (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      out << "using namespace tfel::math;" << endl;
      out << "using std::vector;" << endl;
    }
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "using mfront::MFrontBehaviourProfiler;" << endl;
    }
    if(this->compareToNumericalTangentOperator){
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	out << "const aster::AsterInt ndv    = *NTENS;" << endl;
	out << "const aster::AsterInt nth    = *NTENS;" << endl;
      } else if (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "const aster::AsterReal cste = sqrt(aster::AsterReal(2));" << endl;
	out << "const aster::AsterInt ndv  = (*NTENS==3) ? 3 : ((*NTENS==4) ? 5 : 9);" << endl;
	out << "const aster::AsterInt nth  = *NTENS;" << endl;
      } else if (mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL){
	out << "const aster::AsterInt ndv = (*NTENS==4) ? 2 : 3;" << endl;
	out << "const aster::AsterInt nth = (*NTENS==4) ? 2 : 3;" << endl;
      } else {
	string msg("MFrontAsterInterface::endTreatement : "
		   "the aster interface only supports small and finite strain behaviours, "
		   "and cohesive zone models");
	throw(runtime_error(msg));
      }
    }
    if((this->compareToNumericalTangentOperator)||(this->savesTangentOperator)){
      if((mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL)){
	if(!(this->compareToNumericalTangentOperator)){
	  if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	    out << "const aster::AsterInt ndv    = *NTENS;" << endl;
	    out << "const aster::AsterInt nth    = *NTENS;" << endl;
	  } else {
	    out << "const aster::AsterInt ndv = (*NTENS==4) ? 2 : 3;" << endl;
	    out << "const aster::AsterInt nth = (*NTENS==4) ? 2 : 3;" << endl;
	  }
	}
	out << "const aster::AsterInt tgsize = ndv*nth;" << endl;
      } else if (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "const aster::AsterInt tgsize = 54;" << endl;
      } else {
	string msg("MFrontAsterInterface::endTreatement : "
		   "the aster interface only supports small and finite strain behaviours, "
		   "and cohesive zone models");
	throw(runtime_error(msg));
      }
    }
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "using tfel::material::" << mb.getClassName() << "Profiler;" << endl;
      out << "MFrontBehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler()," << endl
	  << "MFrontBehaviourProfiler::TOTALTIME);" << endl;
    }
    this->generateMTestFile1(out);
    if(this->compareToNumericalTangentOperator){
      out << "const bool computeTangentOperator = (*DDSOE>0.5);" << endl;
    }
    if(this->compareToNumericalTangentOperator){
      if (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "vector<aster::AsterReal> dv0(9u);" << endl;
      } else {
	out << "vector<aster::AsterReal> dv0(ndv);" << endl;
      }
      out << "vector<aster::AsterReal> sig0(nth);" << endl;
      out << "vector<aster::AsterReal> sv0(*NSTATV);" << endl;
      out << "copy(DSTRAN,DSTRAN+dv0.size(),dv0.begin());" << endl;
      out << "copy(STRESS,STRESS+sig0.size(),sig0.begin());" << endl;
      out << "copy(STATEV,STATEV+sv0.size(),sv0.begin());" << endl;
    }
    if(!this->savesTangentOperator){
      out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName() 
	  << ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,STATEV,NSTATV,STRESS,"
	  << sfeh << ")!=0){" << endl;
      this->generateMTestFile2(out,mb.getBehaviourType(),
       			       name,"",mb);
      out << "*PNEWDT = -1.;" << endl;
      out << "return;" << endl;
      out << "}" << endl;
    } else {
      out << "if(*(NSTATV)<tgsize){" << endl
	  << "string msg(\"aster" << makeLowerCase(name) 
	  << ": invalid number of state variables (can't save tangent operator)\");" << endl
	  << "throw(runtime_error(msg));" << endl
	  << "}" << endl;
      out << "aster::AsterInt nNSTATV = max(*(NSTATV)-tgsize,aster::AsterInt(1));" << endl;
      out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName() 
	  << ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,STATEV,&nNSTATV,STRESS,"
	  << sfeh << ")!=0){" << endl;
      this->generateMTestFile2(out,mb.getBehaviourType(),
			       name,"",mb);
      out << "*PNEWDT = -1.;" << endl;
      out << "return;" << endl;
      out << "}" << endl;
      out << "copy(DDSOE,DDSOE+tgsize,STATEV+*(NSTATV)-tgsize);" << endl;
    }
    if(this->compareToNumericalTangentOperator){
      out << "if(computeTangentOperator){" << endl;
      out << "// computing the tangent operator by pertubation" << endl;
      out << "aster::AsterInt i;" << endl;
      out << "aster::AsterInt j;" << endl;
      out << "vector<aster::AsterReal> nD(ndv*nth);" << endl;
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "vector<aster::AsterReal> tmp_nD(ndv*nth);" << endl;
      }
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "vector<aster::AsterReal> ddv(9u);" << endl;
      } else {
	out << "vector<aster::AsterReal> ddv(ndv);" << endl;
      }
      out << "vector<aster::AsterReal> sigf(nth);" << endl;
      out << "vector<aster::AsterReal> sigb(nth);" << endl;
      out << "vector<aster::AsterReal> sv(*NSTATV);" << endl;
      out << "vector<aster::AsterReal> D(tgsize);" << endl;
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "aster::AsterReal Jf;" << endl
	    << "aster::AsterReal Jb;" << endl;
      }
      out << "aster::AsterReal m;" << endl;
      out << "aster::AsterReal mDt;" << endl;
      out << "aster::AsterReal mnDt;" << endl;
      out << "for(i=0;i!=ndv;++i){" << endl;
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "aster::AsterInt mi;" << endl;
	out << "aster::AsterInt mj;" << endl;
	out << "aster::getDeformationGradientIndexes(mi,mj,i,*NTENS);" << endl;
      }
      out << "copy(dv0.begin(),dv0.end(),ddv.begin());" << endl;
      out << "copy(sig0.begin(),sig0.end(),sigf.begin());" << endl;
      out << "copy(sv0.begin(),sv0.end(),sv.begin());" << endl;
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "ddv[mj*3+mi] += " << this->strainPerturbationValue << ";" << endl;
      } else {
	out << "ddv[i] += " << this->strainPerturbationValue << ";" << endl;
      }
      out << "D[0] = 0.;" << endl;
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "Jf=ddv[0]*(ddv[4]*ddv[8]-ddv[5]*ddv[7])-ddv[3]*(ddv[1]*ddv[8]-ddv[2]*ddv[7])+(ddv[1]*ddv[5]-ddv[2]*ddv[4])*ddv[6];" << endl;
      }
      if(!this->savesTangentOperator){
	out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName() 
	    << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&ddv[0],TEMP,DTEMP,PROPS,NPROPS,"
	    << "PREDEF,DPRED,&sv[0],NSTATV,&sigf[0],"
	    << sfeh << ")!=0){" << endl;
      } else {
	out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName() 
	    << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&ddv[0],TEMP,DTEMP,PROPS,NPROPS,"
	    << "PREDEF,DPRED,&sv[0],&nNSTATV,&sigf[0],"
	    << sfeh << ")!=0){" << endl;
      }
      out << "return;" << endl;
      out << "}" << endl;
      out << "copy(dv0.begin(),dv0.end(),ddv.begin());" << endl;
      out << "copy(sig0.begin(),sig0.end(),sigb.begin());" << endl;
      out << "copy(sv0.begin(),sv0.end(),sv.begin());" << endl;
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "ddv[mj*3+mi] -= " << this->strainPerturbationValue << ";" << endl;
      } else {
	out << "ddv[i] -= " << this->strainPerturbationValue << ";" << endl;
      }
      out << "D[0] = 0.;" << endl;
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "Jb=ddv[0]*(ddv[4]*ddv[8]-ddv[5]*ddv[7])-ddv[3]*(ddv[1]*ddv[8]-ddv[2]*ddv[7])+(ddv[1]*ddv[5]-ddv[2]*ddv[4])*ddv[6];" << endl;
      }
      if(!this->savesTangentOperator){
	out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName() 
	    << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&ddv[0],TEMP,DTEMP,PROPS,NPROPS,"
	    << "PREDEF,DPRED,&sv[0],NSTATV,&sigb[0],"
	    << sfeh << ")!=0){" << endl;
      } else {
	out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName() 
	    << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&ddv[0],TEMP,DTEMP,PROPS,NPROPS,"
	    << "PREDEF,DPRED,&sv[0],&nNSTATV,&sigb[0],"
	    << sfeh << ")!=0){" << endl;
      }
      out << "return;" << endl;
      out << "}" << endl;
      out << "for(j=0;j!=nth;++j){" << endl;
      // filling the ith column
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	// conversion to Kirchhoff stress
	// conversion to Aster
	out << "if(j<3){" << endl
	    << "tmp_nD[j*ndv+i] = (Jf*sigf[j]-Jb*sigb[j])/(2.*" << this->strainPerturbationValue << ");" << endl
	    << "} else {" << endl
	    << "tmp_nD[j*ndv+i] = cste*(Jf*sigf[j]-Jb*sigb[j])/(2.*" << this->strainPerturbationValue << ");" << endl
	    << "}" << endl;
      } else {
	out << "nD[j*nth+i] = (sigf[j]-sigb[j])/(2.*" << this->strainPerturbationValue << ");" << endl;
      }
      out << "}" << endl;
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "if(*NTENS==3){" << endl
	    << "tensor<1u,aster::AsterReal> F0;" << endl
	    << "tensor<1u,aster::AsterReal>::buildFromFortranMatrix(F0,STRAN);" << endl
	    << "*(reinterpret_cast<t2tost2<1u,aster::AsterReal>*>(&nD[0])) = (*(reinterpret_cast<const t2tost2<1u,aster::AsterReal>*>(&tmp_nD[0])))*t2tot2<1u,aster::AsterReal>::tpld(F0);" << endl
	    << "} else if(*NTENS==4){" << endl
	    << "tensor<2u,aster::AsterReal> F0;" << endl
	    << "tensor<2u,aster::AsterReal>::buildFromFortranMatrix(F0,STRAN);" << endl
	    << "*(reinterpret_cast<t2tost2<2u,aster::AsterReal>*>(&nD[0])) = (*(reinterpret_cast<const t2tost2<2u,aster::AsterReal>*>(&tmp_nD[0])))*t2tot2<2u,aster::AsterReal>::tpld(F0);" << endl
	    << "} else {" << endl
	    << "tensor<3u,aster::AsterReal> F0;" << endl
	    << "tensor<3u,aster::AsterReal>::buildFromFortranMatrix(F0,STRAN);" << endl
	    << "*(reinterpret_cast<t2tost2<3u,aster::AsterReal>*>(&nD[0])) = (*(reinterpret_cast<const t2tost2<3u,aster::AsterReal>*>(&tmp_nD[0])))*t2tot2<3u,aster::AsterReal>::tpld(F0);" << endl
	    << "}";
      }
      out << "}" << endl;
      out << "// comparison" << endl;
      out << "m=0.;" << endl;
      out << "mDt=0.;" << endl;
      out << "mnDt=0.;" << endl;
      out << "for(i=0;i!=nth;++i){" << endl;
      out << "for(j=0;j!=ndv;++j){" << endl;
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "aster::AsterInt mi;" << endl;
	out << "aster::AsterInt mj;" << endl;
	out << "aster::getDeformationGradientIndexes(mi,mj,j,*NTENS);" << endl;
      }
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "mDt=max(mDt,*(DDSOE+i+6*(mi+3*mj)));" << endl;
      } else {
	// conventions fortran...
	out << "mDt=max(mDt,*(DDSOE+j*nth+i));" << endl;
      }
      out << "mnDt=max(mnDt,nD[i*ndv+j]);" << endl;
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "m=max(m,abs(nD[i*ndv+j]-*(DDSOE+i+6*(mi+3*mj))));" << endl;
      } else {
	// conventions fortran...
	out << "m=max(m,abs(nD[i*ndv+j]-*(DDSOE+j*nth+i)));" << endl;
      }
      out << "}" << endl;
      out << "}" << endl;
      out << "if(m>" << this->tangentOperatorComparisonCriterium << "){" << endl;
      out << "cout << \"||nDt-Dt|| = \" << m << \" (\" << 100.*m/(0.5*(mDt+mnDt)+1.e-50) << \"%)\"<< endl;" << endl;
      out << "cout << \"Dt :\" << endl;" << endl;
      out << "for(i=0;i!=nth;++i){" << endl;
      out << "for(j=0;j!=ndv;++j){" << endl;
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "aster::AsterInt mi;" << endl;
	out << "aster::AsterInt mj;" << endl;
	out << "aster::getDeformationGradientIndexes(mi,mj,j,*NTENS);" << endl;
	out << "cout << *(DDSOE+i+6*(mi+3*mj)) << \" \";" << endl;
      } else {
	out << "cout << *(DDSOE+i*ndv+j) << \" \";" << endl;
      }
      out << "}" << endl;
      out << "cout << endl;" << endl;
      out << "}" << endl;
      out << "cout << \"nDt :\" << endl;" << endl;
      out << "for(i=0;i!=nth;++i){" << endl;
      out << "for(j=0;j!=ndv;++j){" << endl;
      out << "cout << nD[i*ndv+j] << \" \";" << endl;
      out << "}" << endl;
      out << "cout << endl;" << endl;
      out << "}" << endl;
      out << "cout << endl;" << endl;
      out << "}" << endl;
      out << "}" << endl;
    }
    out << "}" << endl << endl;
    out << "} // end of extern \"C\"" << endl;
    out.close();
  } // end of MFrontAsterInterface::endTreatement

  void
  MFrontAsterInterface::writeMTestFileGeneratorSetModellingHypothesis(std::ostream& out) const
  {
    using namespace std;
    out << "ModellingHypothesis::Hypothesis h;" << endl;
    out << "if(*NTENS==3u){" << endl;
    out << "  h = ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN;" << endl;
    out << "} else if(*NTENS==4){" << endl;
    out << "  h = ModellingHypothesis::GENERALISEDPLANESTRAIN;" << endl;
    out << "} else if(*NTENS==6){" << endl;
    out << "  h = ModellingHypothesis::TRIDIMENSIONAL;" << endl;
    out << "} else {" << endl;
    out << "  return;" << endl;
    out << "}" << endl;
    out << "mg.setModellingHypothesis(h);" << endl;
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
    using namespace std;
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name,h);
    out << "_savesTangentOperator = ";
    if(this->savesTangentOperator){
      out << "1";
    } else {
      out << "0";
    }
    out << ";" << endl;
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
    out << ">" << endl;
    out << "struct AsterTraits<tfel::material::" << mb.getClassName() << "<H,Type,";
    if(mb.useQt()){
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >\n{" << endl;
    out << "//! behaviour type" << endl;
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "static const AsterBehaviourType btype = aster::SMALLSTRAINSTANDARDBEHAVIOUR;"  << endl;
    } else if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "static const AsterBehaviourType btype = aster::FINITESTRAINSTANDARDBEHAVIOUR;" << endl;
    } else if(mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL){
      out << "static const AsterBehaviourType btype = aster::COHESIVEZONEMODEL;" << endl;
    } else {
      string msg("MFrontAsterInterface::writeAsterBehaviourTraits : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    out << "//! space dimension" << endl;
    out << "static const unsigned short N           = tfel::material::ModellingHypothesisToSpaceDimension<H>::value;" << endl;
    out << "// tiny vector size" << endl;
    out << "static const unsigned short TVectorSize = N;" << endl;
    out << "// symmetric tensor size" << endl;
    out << "static const unsigned short StensorSize = tfel::math::StensorDimeToSize<N>::value;" << endl;
    out << "// tensor size" << endl;
    out << "static const unsigned short TensorSize  = tfel::math::TensorDimeToSize<N>::value;" << endl;
    out << "// size of the driving variable array (STRAN)" << endl;
    out << "static const unsigned short DrivingVariableSize = " << mvs.first <<  ";" << endl;
    out << "// size of the thermodynamic force variable array (STRESS)" << endl;
    out << "static const unsigned short ThermodynamicForceVariableSize = " << mvs.second <<  ";" << endl;
    if(this->errorReport){
      out << "static const AsterErrorReportPolicy errorReportPolicy = ASTER_WRITEONSTDOUT;" << endl;
    } else {
      out << "static const AsterErrorReportPolicy errorReportPolicy = ASTER_NOERRORREPORT;" << endl;
    }
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
      out << "static const bool requiresStiffnessTensor = true;" << endl;
    } else {
      out << "static const bool requiresStiffnessTensor = false;" << endl;
    }
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      out << "static const bool requiresThermalExpansionCoefficientTensor = true;" << endl;
    } else {
      out << "static const bool requiresThermalExpansionCoefficientTensor = false;" << endl;
    }
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      out << "static const AsterSymmetryType type = aster::ISOTROPIC;" << endl;
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "static const AsterSymmetryType type = aster::ORTHOTROPIC;" << endl;
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
    out << "static const unsigned short material_properties_nb = " << msize << ";" << endl;
    if(mb.getElasticSymmetryType()==mfront::ISOTROPIC){
      out << "static const AsterSymmetryType etype = aster::ISOTROPIC;" << endl;
      if(mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
	out << "static const unsigned short elasticPropertiesOffset = 2u;" << endl;
      } else {
	out << "static const unsigned short elasticPropertiesOffset = 0u;" << endl;
      }
      if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	out << "static const unsigned short thermalExpansionPropertiesOffset = 1u;" << endl; 
      } else {
	out << "static const unsigned short thermalExpansionPropertiesOffset = 0u;" << endl; 
      }
    } else if (mb.getElasticSymmetryType()==mfront::ORTHOTROPIC){
      out << "static const AsterSymmetryType etype = aster::ORTHOTROPIC;" << endl;
      if(mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
    	out << "static const unsigned short elasticPropertiesOffset "
    	    << "= AsterOrthotropicElasticPropertiesOffset<N>::value;" << endl;
      } else {
	out << "static const unsigned short elasticPropertiesOffset = 0u;" << endl;
      }
      if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	out << "static const unsigned short thermalExpansionPropertiesOffset = 3u;" << endl; 
      } else {
	out << "static const unsigned short thermalExpansionPropertiesOffset = 0u;" << endl; 
      }
    } else {
      string msg("MFrontAsterInterface::endTreatement : ");
      msg += "unsupported behaviour type.\n";
      msg += "The aster interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    out << "}; // end of class AsterTraits\n" << endl;
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
