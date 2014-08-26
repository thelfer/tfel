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
#include"MFront/MFrontAsterInterface.hxx"

namespace mfront{

  std::string
  MFrontAsterInterface::getName(void)
  {
    return "aster";
  }

  std::string
  MFrontAsterInterface::getLibraryName(const std::string& library,
				       const std::string& material) const
  {
    using namespace std;
    string lib;
    if(library.empty()){
      if(!material.empty()){
	lib = "libAster"+material;
      } else {
	lib = "libAsterBehaviour";
      }
    } else {
      lib = "libAster"+library;
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

  void
  MFrontAsterInterface::endTreatement(const std::string& file,
				      const std::string& library,
				      const std::string& material,
				      const std::string& className,
				      const std::string& authorName,
				      const std::string& date,
				      const std::map<std::string,std::string>& glossaryNames,
				      const std::map<std::string,std::string>& entryNames,
				      const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;
    const VarContainer& coefsHolder              = mb.getMaterialProperties();
    string name;
    string asterFctName;
    string tmp;
    VarContainer::const_iterator p;

    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Aster");

    if(!library.empty()){
      name += library;
    }
    name += className;

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

    // specific treatment for isotropic behaviour
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      if(mb.getElasticSymmetryType()!=mfront::ISOTROPIC){
	string msg("MFrontAsterInterface::endTreatement : ");
	msg += "an isotropic behaviour must have an isotropic elastic behaviour";
	throw(runtime_error(msg));
      }
      if((mb.requiresStiffnessOperator())||
	 (mb.requiresThermalExpansionTensor())){
	unsigned short min_nprops = 2u;
	if(mb.requiresThermalExpansionTensor()){
	  min_nprops = 4u;
	}
	if(coefsHolder.size()<min_nprops){
	  string msg("MFrontAsterInterface::endTreatement : "
		     "you requested that either (or both) the "
		     "stiffness tensor or the thermal expansion "
		     "tensor to be computed for you. Thus,"
		     "the aster interface requires the ");
	  msg += "following material propertys to be defined (in the right order) ";
	  msg += "- the young modulus     (use @MaterialProperty stress           young)\n";
	  msg += "- the poisson ratio     (use @MaterialProperty real             nu)\n";
	  if(mb.requiresThermalExpansionTensor()){
	    msg += "- the density           (use @MaterialProperty density rho)";
	    msg += "- the thermal expansion (use @MaterialProperty thermalexpansion alpha)\n";
	  }
	  throw(runtime_error(msg));
	}
	if(coefsHolder[0].name!="young"){
	  string msg("MFrontASTERInterface::endTreatement : the aster interface requires the ");
	  msg += "first material property to be the young modulus (use @MaterialProperty stress young)";
	  throw(runtime_error(msg));
	}
	if(coefsHolder[1].name!="nu"){
	  string msg("MFrontASTERInterface::endTreatement : the aster interface requires the ");
	  msg += "second material property to be the poisson ratio (use @MaterialProperty real nu)";
	  throw(runtime_error(msg));
	}
	if(mb.requiresThermalExpansionTensor()){
	  if(coefsHolder[2].name!="rho"){
	    string msg("MFrontASTERInterface::endTreatement : the aster interface requires the " );
	    msg += "third material property to be the density (use @MaterialProperty density rho)";
	    throw(runtime_error(msg));
	  }
	  if(coefsHolder[3].name!="alpha"){
	    string msg("MFrontASTERInterface::endTreatement : the aster interface requires the" );
	    msg += "fourth material property to be the thermal expansion (use @MaterialProperty thermalexpansion alpha)";
	    throw(runtime_error(msg));
	  }
	}
      }
    }

    out << "/*!\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file declares the aster interface for the " 
	<< className << " behaviour law\n";
    out << "* \\author "  << authorName << endl;
    out << "* \\date   "  << date       << endl;
    out << "*/\n\n";

    const string header = this->getHeaderDefine(library,
						material,
						className);
    out << "#ifndef "<< header << "\n";
    out << "#define "<< header << "\n\n";

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    out << "#include\"MFront/Aster/Aster.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/Aster/AsterTraits.hxx\"\n";
    if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/Aster/AsterOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << className << ".hxx\"\n";
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

    if(mb.useQt()){
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type,bool use_qt>\n";
    } else {
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type>\n";
    } 
    out << "struct AsterTraits<tfel::material::" << className << "<H,Type,";
    if(mb.useQt()){
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >{\n";
    if(this->errorReport){
      out << "static const AsterErrorReportPolicy errorReportPolicy = ASTER_WRITEONSTDOUT;\n";
    } else {
      out << "static const AsterErrorReportPolicy errorReportPolicy = ASTER_NOERRORREPORT;\n";
    }
    if(mb.requiresStiffnessOperator()){
      out << "static const bool requiresStiffnessOperator = true;\n";
    } else {
      out << "static const bool requiresStiffnessOperator = false;\n";
    }
    if(mb.requiresThermalExpansionTensor()){
      out << "static const bool requiresThermalExpansionTensor = true;\n";
    } else {
      out << "static const bool requiresThermalExpansionTensor = false;\n";
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
    bool hasElasticMaterialPropertiesOffset;
    bool hasThermalExpansionMaterialPropertiesOffset;
    bool hasMassDensityOffsetForThermalExpansion;
    this->getMaterialPropertiesOffset(hasElasticMaterialPropertiesOffset,
				      hasThermalExpansionMaterialPropertiesOffset,
				      hasMassDensityOffsetForThermalExpansion,mb);
    if(mb.getElasticSymmetryType()==mfront::ISOTROPIC){
      out << "static const AsterSymmetryType etype = aster::ISOTROPIC;\n";
      if(hasElasticMaterialPropertiesOffset){
	out << "static const unsigned short elasticPropertiesOffset = 2u;\n";
      } else {
	out << "static const unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if(hasMassDensityOffsetForThermalExpansion){
	out << "static const unsigned short massDensityOffsetForThermalExpansion = 1u;\n";
      } else {
	out << "static const unsigned short massDensityOffsetForThermalExpansion = 0u;\n";
      }
      if(hasThermalExpansionMaterialPropertiesOffset){
	out << "static const unsigned short thermalExpansionPropertiesOffset = 1u;\n"; 
      } else {
	out << "static const unsigned short thermalExpansionPropertiesOffset = 0u;\n"; 
      }
    } else if (mb.getElasticSymmetryType()==mfront::ORTHOTROPIC){
      out << "static const AsterSymmetryType etype = aster::ORTHOTROPIC;\n";
      out << "static const unsigned short N = tfel::material::ModellingHypothesisToSpaceDimension<H>::value;\n";
      if(hasElasticMaterialPropertiesOffset){
	out << "static const unsigned short elasticPropertiesOffset "
	    << "= AsterOrthotropicElasticPropertiesOffset<N>::value;\n";
      } else {
	out << "static const unsigned short elasticPropertiesOffset = 0u;\n";
      }
      out << "static const unsigned short massDensityOffsetForThermalExpansion = 0u;\n";
      if(hasThermalExpansionMaterialPropertiesOffset){
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
	<< className << " behaviour law\n";
    out << "* \\author "  << authorName << endl;
    out << "* \\date   "  << date       << endl;
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
    out << "#include\"TFEL/Material/" << className << ".hxx\"\n";
    out << "#include\"MFront/Aster/AsterInterface.hxx\"\n\n";
    out << "#include\"MFront/Aster/aster" << name << ".hxx\"\n\n";

    out << "extern \"C\"{\n\n";
 
    this->generateUMATxxSymbols(out,name,file,mb,glossaryNames,entryNames);
    
    this->writeSetParametersFunctionsImplementations(out,name,className,mb);
    
    out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL\naster"
	<< makeLowerCase(name) << "("
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
    if(((this->debugMode)||(this->compareToNumericalTangentOperator)||
	(this->savesTangentOperator))&&(!this->generateMTestFile)){
      out << "using namespace std;\n";
    }
    this->generateMTestFile1(out);
    if((this->debugMode)||(this->compareToNumericalTangentOperator)){
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
      out << "if(aster::AsterInterface<tfel::material::" << className 
	  << ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,STATEV,NSTATV,STRESS)!=0){\n";
      this->generateMTestFile2(out,MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
			       library,material,name,"",mb,glossaryNames,entryNames);
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
      out << "if(aster::AsterInterface<tfel::material::" << className 
	  << ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,STATEV,&nNSTATV,STRESS)!=0){\n";
      this->generateMTestFile2(out,MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
			       library,material,name,"",mb,glossaryNames,entryNames);
      out << "*PNEWDT = -1.;\n";
      out << "return;\n";
      out << "}\n";
      out << "copy(DDSOE,DDSOE+(*NTENS)*(*NTENS),STATEV+*(NSTATV)-(*NTENS)*(*NTENS));\n";
    }
    if(this->debugMode){
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
	out << "if(aster::AsterInterface<tfel::material::" << className 
	    << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,NPROPS,"
	    << "PREDEF,DPRED,&sv[0],NSTATV,&sigf[0])!=0){\n";
      } else {
	out << "if(aster::AsterInterface<tfel::material::" << className 
	    << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,NPROPS,"
	    << "PREDEF,DPRED,&sv[0],&nNSTATV,&sigf[0])!=0){\n";
      }
      out << "return;\n";
      out << "}\n";
      out << "copy(deto0.begin(),deto0.end(),deto.begin());\n";
      out << "copy(sig0.begin(),sig0.end(),sigb.begin());\n";
      out << "copy(sv0.begin(),sv0.end(),sv.begin());\n";
      out << "deto[i] -= " << this->strainPerturbationValue << ";\n";
      out << "D[0] = 0.;\n";
      if(!this->savesTangentOperator){
	out << "if(aster::AsterInterface<tfel::material::" << className 
	    << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,NPROPS,"
	    << "PREDEF,DPRED,&sv[0],NSTATV,&sigb[0])!=0){\n";
      } else {
	out << "if(aster::AsterInterface<tfel::material::" << className 
	    << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,NPROPS,"
	    << "PREDEF,DPRED,&sv[0],&nNSTATV,&sigb[0])!=0){\n";
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
  
  MFrontAsterInterface::~MFrontAsterInterface()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontAsterInterface::getGlobalIncludes(const std::string& library,
					  const std::string& material,
					  const std::string&)
  {
    using namespace std;
    map<string,vector<string> > incs;
    string lib = MFrontAsterInterface::getLibraryName(library,material);
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
  MFrontAsterInterface::getGeneratedSources(const std::string& library,
					    const std::string& material,
					    const std::string& className)
  {
    using namespace std;
    map<string,vector<string> > src;
    string lib = MFrontAsterInterface::getLibraryName(library,material);
    if(!library.empty()){
      src[lib].push_back("aster"+library+className+".cxx");
    } else {
      src[lib].push_back("aster"+className+".cxx");
    }
    return src;
  } // end of MFrontAsterInterface::getGeneratedSources

  std::vector<std::string>
  MFrontAsterInterface::getGeneratedIncludes(const std::string& library,
					     const std::string&,
					     const std::string& className)
  {
    using namespace std;
    vector<string> incs;
    if(!library.empty()){
      incs.push_back("MFront/Aster/aster"+library+className+".hxx");
    } else {
      incs.push_back("MFront/Aster/aster"+className+".hxx");
    }
    return incs;
  } // end of MFrontAsterInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontAsterInterface::getLibrariesDependencies(const std::string& library,
						 const std::string& material,
						 const std::string&)
  {
    using namespace std;
    map<string,vector<string> > deps;
    string lib = MFrontAsterInterface::getLibraryName(library,material);
    deps[lib].push_back("-lAsterInterface");
    deps[lib].push_back("`tfel-config --libs --material`");
    return deps;
  } // end of MFrontAsterInterface::getLibrariesDependencies()

  void
  MFrontAsterInterface::getMaterialPropertiesOffset(bool& hasElasticMaterialPropertiesOffset,
						    bool& hasThermalExpansionMaterialPropertiesOffset,
						    bool& massDensityOffsetForThermalExpansion,
						    const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    const VarContainer& coefsHolder = mb.getMaterialProperties();
    VarContainer::const_iterator p;
    hasElasticMaterialPropertiesOffset          = false;
    hasThermalExpansionMaterialPropertiesOffset = false;
    massDensityOffsetForThermalExpansion        = false;
    // this indicates that the material properties required to compute
    // the stiffness tensor are part of the material properties
    bool founde = false;
    // this indicates that the material properties required to compute
    // the thermal expansion tensor are part of the material properties
    bool foundt = false;
    if(mb.getElasticSymmetryType()==mfront::ISOTROPIC){
      if(mb.requiresStiffnessOperator()){
	for(p=coefsHolder.begin();(p!=coefsHolder.end())&&(!founde);++p){
	  if((p->name=="young")||
	     (p->name=="nu")){
	    founde = true;
	  }
	}
	if(founde){
	  /*
	   * Check Aster requirements
	   */
	  if(coefsHolder.size()<2){
	    string msg("MFrontAsterInterface::getMaterialPropertiesOffset : the aster interface requires the ");
	    msg += "following two material propertys to be defined (in the right order) when the ";
	    msg += "stiffness tensor is required";
	    msg += "(currently only ";
	    msg += toString(static_cast<unsigned short>(coefsHolder.size()));
	    msg += " defined):\n";
	    msg += "- the young modulus     (use @MaterialProperty stress young)\n";
	    msg += "- the poisson ratio     (use @MaterialProperty real   nu)\n";
	    throw(runtime_error(msg));
	  }
	  if(coefsHolder[0].name!="young"){
	    string msg("MFrontAsterInterface::getMaterialPropertiesOffset : the aster interface requires the ");
	    msg += "first material property to be the young modulus (use @MaterialProperty stress young)";
	    throw(runtime_error(msg));
	  }
	  if(coefsHolder[1].name!="nu"){
	    string msg("MFrontAsterInterface::getMaterialPropertiesOffset : the aster interface requires the ");
	    msg += "second material property to be the poisson ratio (use @MaterialProperty real nu)";
	    throw(runtime_error(msg));
	  }
	} else {
	  hasElasticMaterialPropertiesOffset = true;
	}
      }
      if(mb.requiresThermalExpansionTensor()){
	for(p=coefsHolder.begin();(p!=coefsHolder.end())&&(!foundt);++p){
	  if(p->name=="alpha"){
	    foundt = true;
	  }
	}
	if(foundt){
	  if(mb.requiresStiffnessOperator()){
	    if(coefsHolder.size()<3){
	      string msg("MFrontAsterInterface::getMaterialPropertiesOffset : the aster interface requires the ");
	      msg += "following three or four material propertys to be defined (in the right order) when the ";
	      msg += "stiffness and the thermal expansion tensors are required";
	      msg += "(currently only ";
	      msg += toString(static_cast<unsigned short>(coefsHolder.size()));
	      msg += " defined):\n";
	      msg += "- the young modulus     (use @MaterialProperty stress young)\n";
	      msg += "- the poisson ratio     (use @MaterialProperty real   nu)\n";
	      msg += "[- the mass density     (use @MaterialProperty density rho)]\n";
	      msg += "- the thermal expansion (use @MaterialProperty thermalexpansion alpha)\n";
	      msg += "(the mass density is optionnal and shall be used only if the law ";
	      msg += "has to be compatible with Cast3M.)";
	      throw(runtime_error(msg));
	    }
	    // compatibility with Cast3M
	    if((coefsHolder[2].name!="alpha")&&
	       (coefsHolder[2].name!="rho")){
	      string msg("MFrontAsterInterface::getMaterialPropertiesOffset : the aster interface requires the ");
	      msg += "third material property to be the thermal expansion or the density";
	      throw(runtime_error(msg));
	    }
	    if(coefsHolder[2].name=="rho"){
	      if(coefsHolder.size()<4){
		string msg("MFrontAsterInterface::getMaterialPropertiesOffset : the aster interface requires the ");
		msg += "following three or four material propertys to be defined (in the right order) when the ";
		msg += "stiffness and the thermal expansion tensors are required";
		msg += "(currently only ";
		msg += toString(static_cast<unsigned short>(coefsHolder.size()));
		msg += " defined):\n";
		msg += "- the young modulus     (use @MaterialProperty stress young)\n";
		msg += "- the poisson ratio     (use @MaterialProperty real   nu)\n";
		msg += "[- the mass density     (use @MaterialProperty density rho)]\n";
		msg += "- the thermal expansion (use @MaterialProperty thermalexpansion alpha)\n";
		msg += "(the mass density is optionnal and shall be used only if the law ";
		msg += "has to be compatible with Cast3M.)";
		throw(runtime_error(msg));
	      }
	      if(coefsHolder[3].name!="alpha"){
		string msg("MFrontAsterInterface::getMaterialPropertiesOffset : the aster interface requires the ");
		msg += "third material property to be the thermal expansion or the density";
		throw(runtime_error(msg));
	      }
	      massDensityOffsetForThermalExpansion = true;
	    }
	  } else {
	    if(coefsHolder.empty()){
	      string msg("MFrontAsterInterface::getMaterialPropertiesOffset : the aster interface requires the ");
	      msg += "thermal expansion to be defined as the first material property";
	      throw(runtime_error(msg));
	    }
	    if(coefsHolder[0].name!="alpha"){
	      string msg("MFrontAsterInterface::getMaterialPropertiesOffset : the aster interface requires the ");
	      msg += "thermal expansion to be defined as the first material property";
	      throw(runtime_error(msg));
	    }
	  }
	} else {
	  hasThermalExpansionMaterialPropertiesOffset = true;
	}
      }
    } else if (mb.getElasticSymmetryType()==mfront::ORTHOTROPIC){
      if(mb.requiresStiffnessOperator()){
	hasElasticMaterialPropertiesOffset = true;
      }
      if(mb.requiresThermalExpansionTensor()){
	hasThermalExpansionMaterialPropertiesOffset = true;
      }
    } else {
      string msg("MFrontAsterInterface::getMaterialPropertiesOffset : ");
      msg += "unsupported behaviour type.\n";
      msg += "The aster interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
  } // end of MFrontAsterInterface::getMaterialPropertiesOffset

  bool
  MFrontAsterInterface::hasMaterialPropertiesOffset(const MechanicalBehaviourDescription& mb) const
  {
    bool hasElasticMaterialPropertiesOffset;
    bool hasThermalExpansionMaterialPropertiesOffset;
    bool hasMassDensityOffsetForThermalExpansion;
    this->getMaterialPropertiesOffset(hasElasticMaterialPropertiesOffset,
				      hasThermalExpansionMaterialPropertiesOffset,
				      hasMassDensityOffsetForThermalExpansion,mb);
    return hasElasticMaterialPropertiesOffset||hasThermalExpansionMaterialPropertiesOffset||hasMassDensityOffsetForThermalExpansion;
  } // end of MFrontAsterInterface::hasMaterialPropertiesOffset

  void
  MFrontAsterInterface::writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation(std::ostream& out,
											  const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    if(!this->hasMaterialPropertiesOffset(mb)){
      return;
    }
    bool hasElasticMaterialPropertiesOffset;
    bool hasThermalExpansionMaterialPropertiesOffset;
    bool hasMassDensityOffsetForThermalExpansion;
    this->getMaterialPropertiesOffset(hasElasticMaterialPropertiesOffset,
    				      hasThermalExpansionMaterialPropertiesOffset,
    				      hasMassDensityOffsetForThermalExpansion,mb);
    if(mb.getElasticSymmetryType()==mfront::ISOTROPIC){
      if(hasElasticMaterialPropertiesOffset){
	out << "mg.addMaterialProperty(\"YoungModulus\",*PROPS);\n"
	    << "mg.addMaterialProperty(\"PoissonRatio\",*(PROPS+1));\n";
      }
      if(hasThermalExpansionMaterialPropertiesOffset){
	if(hasElasticMaterialPropertiesOffset){
	  if(hasMassDensityOffsetForThermalExpansion){
	    out << "mg.addMaterialProperty(\"MassDensity\",*(PROPS+2));\n"
		<< "mg.addMaterialProperty(\"ThermalExpansion\",*(PROPS+3));\n";
	    out << "mg_mpoffset = 4u;\n";
	  } else {
	    out << "mg.addMaterialProperty(\"ThermalExpansion\",*(PROPS+2));\n";
	    out << "mg_mpoffset = 3u;\n";
	  }
	} else {
	  if(hasMassDensityOffsetForThermalExpansion){
	    out << "mg.addMaterialProperty(\"MassDensity\",*PROPS));\n"
		<< "mg.addMaterialProperty(\"ThermalExpansion\",*(PROPS+1));\n";
	    out << "mg_mpoffset = 2u;\n";
	  } else {
	    out << "mg.addMaterialProperty(\"ThermalExpansion\",*PROPS);\n";
	    out << "mg_mpoffset = 1u;\n";
	  }
	}
      } else {
	out << "mg_mpoffset = 0u;\n";
      }
    } else if(mb.getElasticSymmetryType()==mfront::ORTHOTROPIC){
      out << "if(*NTENS==3u){\n";
      if(hasElasticMaterialPropertiesOffset){
	out << "mg.addMaterialProperty(\"YoungModulus1\",*PROPS);\n"
	    << "mg.addMaterialProperty(\"YoungModulus2\",*(PROPS+1));\n"
	    << "mg.addMaterialProperty(\"YoungModulus3\",*(PROPS+2));\n"
	    << "mg.addMaterialProperty(\"PoissonRatio12\",*(PROPS+3));\n"
	    << "mg.addMaterialProperty(\"PoissonRatio13\",*(PROPS+4));\n"
	    << "mg.addMaterialProperty(\"PoissonRatio23\",*(PROPS+5));\n";
      }
      if(hasThermalExpansionMaterialPropertiesOffset){
	if(hasElasticMaterialPropertiesOffset){
	  out << "mg.addMaterialProperty(\"ThermalExpansion1\",*(PROPS+6));\n"
	      << "mg.addMaterialProperty(\"ThermalExpansion2\",*(PROPS+7));\n"
	      << "mg.addMaterialProperty(\"ThermalExpansion3\",*(PROPS+8));\n";
	  out << "mg_mpoffset = 9u;\n";
	} else {
	  out << "mg.addMaterialProperty(\"ThermalExpansion1\",*PROPS);\n"
	      << "mg.addMaterialProperty(\"ThermalExpansion2\",*(PROPS+1));\n"
	      << "mg.addMaterialProperty(\"ThermalExpansion3\",*(PROPS+2));\n";
	  out << "mg_mpoffset = 3u;\n";
	}
      } else {
	if(hasElasticMaterialPropertiesOffset){
	  out << "mg_mpoffset = 6u;\n";
	} else {
	  out << "mg_mpoffset = 0u;\n";
	}
      }
      out << "} else if(*NTENS==4){\n";
      if(hasElasticMaterialPropertiesOffset){
	out  << "mg.addMaterialProperty(\"YoungModulus1\",*PROPS);\n"
	     << "mg.addMaterialProperty(\"YoungModulus2\",*(PROPS+1));\n"
	     << "mg.addMaterialProperty(\"YoungModulus3\",*(PROPS+2));\n"
	     << "mg.addMaterialProperty(\"PoissonRatio12\",*(PROPS+3));\n"
	     << "mg.addMaterialProperty(\"PoissonRatio23\",*(PROPS+4));\n"
	     << "mg.addMaterialProperty(\"PoissonRatio13\",*(PROPS+5));\n"
	     << "mg.addMaterialProperty(\"ShearModulus12\",*(PROPS+6));\n";
      }
      if(hasThermalExpansionMaterialPropertiesOffset){
	if(hasElasticMaterialPropertiesOffset){
	  out << "mg.addMaterialProperty(\"ThermalExpansion1\",*(PROPS+7));\n"
	      << "mg.addMaterialProperty(\"ThermalExpansion2\",*(PROPS+8));\n"
	      << "mg.addMaterialProperty(\"ThermalExpansion3\",*(PROPS+9));\n";
	  out << "mg_mpoffset = 10u;\n";
	} else {
	  out << "mg.addMaterialProperty(\"ThermalExpansion1\",*PROPS);\n"
	      << "mg.addMaterialProperty(\"ThermalExpansion2\",*(PROPS+1));\n"
	      << "mg.addMaterialProperty(\"ThermalExpansion3\",*(PROPS+2));\n";
	  out << "mg_mpoffset = 3u;\n";
	}
      } else {
	if(hasElasticMaterialPropertiesOffset){
	  out << "mg_mpoffset = 7u;\n";
	} else {
	  out << "mg_mpoffset = 0u;\n";
	}
      }
      out << "} else if(*NTENS==6){\n";
      if(hasElasticMaterialPropertiesOffset){
	out  << "mg.addMaterialProperty(\"YoungModulus1\",*PROPS);\n"
	     << "mg.addMaterialProperty(\"YoungModulus2\",*(PROPS+1));\n"
	     << "mg.addMaterialProperty(\"YoungModulus3\",*(PROPS+2));\n"
	     << "mg.addMaterialProperty(\"PoissonRatio12\",*(PROPS+3));\n"
	     << "mg.addMaterialProperty(\"PoissonRatio23\",*(PROPS+4));\n"
	     << "mg.addMaterialProperty(\"PoissonRatio13\",*(PROPS+5));\n"
	     << "mg.addMaterialProperty(\"ShearModulus12\",*(PROPS+6));\n"
	     << "mg.addMaterialProperty(\"ShearModulus23\",*(PROPS+7));\n"
	     << "mg.addMaterialProperty(\"ShearModulus13\",*(PROPS+8));\n";
      }
      if(hasThermalExpansionMaterialPropertiesOffset){
	if(hasElasticMaterialPropertiesOffset){
	  out << "mg.addMaterialProperty(\"ThermalExpansion1\",*(PROPS+9));\n"
	      << "mg.addMaterialProperty(\"ThermalExpansion2\",*(PROPS+10));\n"
	      << "mg.addMaterialProperty(\"ThermalExpansion3\",*(PROPS+11));\n";
	  out << "mg_mpoffset = 12u;\n";
	} else {
	  out << "mg.addMaterialProperty(\"ThermalExpansion1\",*PROPS);\n"
	      << "mg.addMaterialProperty(\"ThermalExpansion2\",*(PROPS+1));\n"
	      << "mg.addMaterialProperty(\"ThermalExpansion3\",*(PROPS+2));\n";
	  out << "mg_mpoffset = 3u;\n";
	}
      } else {
	if(hasElasticMaterialPropertiesOffset){
	  out << "mg_mpoffset = 9u;\n";
	} else {
	  out << "mg_mpoffset = 0u;\n";
	}
      }
      out << "}\n";
    } else {
      string msg("MFrontAsterInterface::"
		 "writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation : ");
      msg += "unsupported elastic behaviour type.\n";
      msg += "The aster interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
  } // end of MFrontAsterInterface::writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation
    
  void
  MFrontAsterInterface::writeUMATxxMaterialPropertiesSymbols(std::ostream& out,
							     const std::string& name,
							     const MechanicalBehaviourDescription& mb,
							     const std::map<std::string,std::string>& glossaryNames,
							     const std::map<std::string,std::string>& entryNames) const
  {
    using namespace std;
    const VarContainer& coefsHolder = mb.getMaterialProperties();
    const unsigned short  cs        = this->getNumberOfVariables(mb.getMaterialProperties());
    bool hasElasticMaterialPropertiesOffset;
    bool hasThermalExpansionMaterialPropertiesOffset;
    bool hasMassDensityOffsetForThermalExpansion;
    this->getMaterialPropertiesOffset(hasElasticMaterialPropertiesOffset,
				      hasThermalExpansionMaterialPropertiesOffset,
				      hasMassDensityOffsetForThermalExpansion,mb);
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name);
    out << "_nMaterialProperties = " << cs << ";\n";
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name);
    out << "_eoffset = ";
    if(hasElasticMaterialPropertiesOffset){
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name);
    out << "_teoffset = ";
    if(hasThermalExpansionMaterialPropertiesOffset){
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";
    this->writeGlossaryNames(out,this->getGlossaryNames(coefsHolder,
							glossaryNames,
							entryNames),
			     name,"MaterialProperties");
  } // end of MFrontUMATInterface::writeUMATxxMaterialPropertiesSymbol

  void
  MFrontAsterInterface::writeUMATxxAdditionalSymbols(std::ostream& out,
						     const std::string& name,
						     const std::string&,
						     const MechanicalBehaviourDescription&,
						     const std::map<std::string,std::string>&,
						     const std::map<std::string,std::string>&) const
  {
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name);
    out << "_savesTangentOperator = ";
    if(this->savesTangentOperator){
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";
  } // end of MFrontAsterInterface::writeUMATxxAdditionalSymbols

} // end of namespace mfront
