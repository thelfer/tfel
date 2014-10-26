/*!
 * \file   mfront/src/CppTestMaterialPropertyInterface.cxx
 * \brief    
 * \author P. GOLDBRONN
 * \date   14 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/CppMaterialPropertyInterface.hxx"
#include"MFront/CppTestMaterialPropertyInterface.hxx"

namespace mfront
{

  tfel::utilities::CxxTokenizer::TokensContainer::const_iterator
  CppTestMaterialPropertyInterface::nextToken(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				       const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator endTokens,
				       const std::string &msg)
  {
    std::string what(msg) ;
    if(++current==endTokens){
      what+="unexpected end of file.\n";
      what+="Error at line : ";
      what+=toString((--current)->line);
      throw(std::runtime_error(what));
    }
    return current;
  }
  
  std::string
  CppTestMaterialPropertyInterface::getName(void)
  {
    return "cpptest";
  }

  CppTestMaterialPropertyInterface::CppTestMaterialPropertyInterface()
  {}

  CppTestMaterialPropertyInterface::~CppTestMaterialPropertyInterface()
  {}

  void
  CppTestMaterialPropertyInterface::reset(void)
  {} // end of CppTestMaterialPropertyInterface::reset(void)

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CppTestMaterialPropertyInterface::treatKeyword(const std::string& key,
					  tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					  const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator endTokens)
  {
    using namespace std;
    using namespace std;
    if ( key == "@TestBounds" )
      return registerTestBounds(current,endTokens);
    else
      return make_pair(false,current);
  } // end of treatKeyword

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CppTestMaterialPropertyInterface::registerTestBounds(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
						const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator endTokens)
  {
    using namespace std;
    string msg("CppTestMaterialPropertyInterface::registerTestBounds : ");
    VariableBoundsDescription boundsDescription;
    
    current=nextToken(--current,endTokens,msg);
    if(current->value=="{"){
      msg+="'{' is not allowed in test bounds.\n";
      msg+="Error at line : ";
      msg+=toString(current->line);
    }
    // howto test if current->value is a word ????
    boundsDescription.lineNumber = current->line;
    boundsDescription.varName = current->value;

    // on ne peut pas tester si la variable existe bien en input :-(
    // peut-on avoir ici autre chose que 1 ?
    boundsDescription.varNbr = 1 ;

    current=nextToken(current,endTokens,msg);
    if(current->value!="in"){
      msg+="expected 'in' (read ";
      msg+=current->value;
      msg+=")\nError at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }
    current=nextToken(current,endTokens,msg);
    if(current->value!="["){
      msg+="expected '[' (read ";
      msg+=current->value;
      msg+=")\nError at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }
    current=nextToken(current,endTokens,msg);
    istringstream converterL(current->value);
    converterL >> boundsDescription.lowerBound;
    boundsDescription.boundsType = VariableBoundsDescription::LowerAndUpper;
    if(!converterL&&(!converterL.eof())){
      msg+="could not read lower bound value\nRead : ";
      msg+=current->value;
      msg+="\nError at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }
    current=nextToken(current,endTokens,msg);
    if(current->value!=":"){
      msg+="expected ':' (read ";
      msg+=current->value;
      msg+=")\nError at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }
    current=nextToken(current,endTokens,msg);
    istringstream converterU(current->value);
    converterU >> boundsDescription.upperBound;
    if(!converterU&&(!converterU.eof())){
      msg+="could not read upper bound value\nRead : ";
      msg+=current->value;
      msg+="\nError at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }
    if(boundsDescription.boundsType==VariableBoundsDescription::LowerAndUpper){
      if(boundsDescription.lowerBound>boundsDescription.upperBound){
	msg+="lower bound value is greater than upper bound value for variable ";
	msg+=boundsDescription.varName;
	msg+="\nError at line : ";
	msg+=toString(current->line);
	throw(runtime_error(msg));
      }
    }
    current=nextToken(current,endTokens,msg);
    if(current->value!="]"){
      msg+="expected ']' (read ";
      msg+=current->value;
      msg+=")\nError at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }      
    current=nextToken(current,endTokens,msg);
    if(current->value!=";"){
      msg+="expected ';' (read ";
      msg+=current->value;
      msg+=")\nError at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }      
    testBounds.push_back(boundsDescription);
    return make_pair(true,++current);
  } // end of registerTestBounds

  std::map<std::string,std::vector<std::string> >
  CppTestMaterialPropertyInterface::getGlobalDependencies(const std::string&,
						   const std::string&,
						   const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    return libs;
  } // end of CppTestMaterialPropertyInterface::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  CppTestMaterialPropertyInterface::getGlobalIncludes(const std::string&,
					       const std::string&,
					       const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of CppTestMaterialPropertyInterface::getGlobalIncludes

  std::map<std::string,std::vector<std::string> >
  CppTestMaterialPropertyInterface::getGeneratedSources(const std::string&,
						 const std::string&,
						 const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of CppTestMaterialPropertyInterface::getGeneratedSources

  std::vector<std::string>
  CppTestMaterialPropertyInterface::getGeneratedIncludes(const std::string&,
						  const std::string&,
						  const std::string&)
  {
    using namespace std;
    vector<string> incs;
    return incs;
  } // end of CppTestMaterialPropertyInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  CppTestMaterialPropertyInterface::getLibrariesDependencies(const std::string&,
						      const std::string&,
						      const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of CppTestMaterialPropertyInterface::getLibrariesDependencies()

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  CppTestMaterialPropertyInterface::getSpecificTargets(const std::string& lib,
						const std::string& mat,
						const std::string& className,
						const std::vector<std::string>&)
  {
    using namespace std;
    map<string,pair<vector<string>,vector<string> > > res;
    string target;
    string cpplib = "Cpp"+getMaterialLawLibraryNameBase(lib,mat);
    string name;
    if(mat.empty()){
      name = className;
    } else {
      name = mat+"_"+className;
    }
    target = name+"CppTest";
#if defined _WIN32 || defined _WIN64
    res[target].first.push_back("lib"+cpplib+".dll");
#else
    res[target].first.push_back("lib"+cpplib+".so");
#endif
    res[target].first.push_back(name+"-CppTest.o");
    res[target].second.push_back("@$(CXX) $(LDFLAGS) -L. -l"+cpplib+" $^ -o $@");
    res["check"].first.push_back(target);
    return res;
  } // end of CppMaterialPropertyInterface::getSpecificTargets

  void
  CppTestMaterialPropertyInterface::writeOutputFiles(const std::string& file,
					      const std::string& ,
					      const std::string& material,
					      const std::string& className,
					      const std::string& author,
					      const std::string& date,
					      const std::string&,
					      const std::string&,
					      const std::string& output,
					      const VariableDescriptionContainer& inputs,
					      const std::vector<std::string>&,
					      const std::map<std::string,std::string>&,
					      const std::map<std::string,std::string>&,
					      const StaticVariableDescriptionContainer& staticVars,
					      const std::vector<std::string>& params,
					      const std::map<std::string,double>& paramValues,
					      const LawFunction& function,
					      const std::vector<VariableBoundsDescription>& bounds,
					      const std::vector<VariableBoundsDescription>& physicalBounds,
					      const bool useTemplate,
					      const std::vector<std::string>& namespaces)
  {
    using namespace std;
    string name;
    if(material.empty()){
      name = className;
    } else {
      name = material+"_"+className;
    }
    this->srcFileName  = "src/" + name;
    this->srcFileName += "-CppTest.cxx";
    this->srcFile.open(this->srcFileName.c_str());
    if(!this->srcFile){
      string msg("MaterialPropertyDSL::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.exceptions(ios::badbit|ios::failbit);
    this->writeSrcFile(file,name,author,date,output,
		       inputs,staticVars,params,paramValues,
		       function,bounds,physicalBounds,useTemplate,namespaces);
  } // end of CppTestMaterialPropertyInterface::writeOutputFiles

  void
  CppTestMaterialPropertyInterface::writeSrcFile(const std::string& file,
					  const std::string& name,
					  const std::string&,
					  const std::string&,
					  const std::string&,
					  const VariableDescriptionContainer& inputs,
					  const StaticVariableDescriptionContainer&,
					  const std::vector<std::string>&,
					  const std::map<std::string,double>&,
					  const LawFunction&,
					  const std::vector<VariableBoundsDescription>& bounds,
					  const std::vector<VariableBoundsDescription>&,
					  const bool useTemplate,
					  const std::vector<std::string>& namespaces)
  {
    using namespace std;
    vector<string>::const_iterator p;
    vector<VariableBoundsDescription>::const_iterator p6;
    VariableDescriptionContainer::const_iterator p3;
    VariableDescriptionContainer::const_iterator p4;
    // on initialise avec bounds
    std::vector<VariableBoundsDescription> tests(bounds);
    // nombre de point de calcul
    int numberOfLines = 100 ;
    this->srcFile<<"// generated by mfront from "<< file << endl << endl;
    this->srcFile<<"#include <vector>\n"
		 <<"#include <iostream>\n"
		 <<"#include <sstream>\n"
		 <<"#include <fstream>\n" << endl ;
    if(namespaces.empty()){
      this->srcFile << "#include\"" << name << "-cxx.hxx\"\n\n";
    } else {
      this->srcFile << "#include\"";
      for(p=namespaces.begin();p!=namespaces.end();++p){
	this->srcFile << *p << "/";
      }
      this->srcFile <<name;
      this->srcFile << "-cxx.hxx";
      this->srcFile << "\"\n\n";
    }

    this->srcFile<<"int\n main(const int argc, const char * const * const argv)\n{\n";
    this->srcFile<<"using namespace std;\n";
    if(namespaces.empty()){
      this->srcFile << "using namespace mfront;\n";
    } else {
      this->srcFile << "using namespace " << *namespaces.begin() ;
      for(p=namespaces.begin()+1;p!=namespaces.end();++p){
	this->srcFile << "::" << *p ;
      }
      this->srcFile << ";\n";
    }
    if(useTemplate){
      this->srcFile<<"  " << name <<"<double> my"<<name<<" ;"<<endl;
    } else {
      this->srcFile<<"  " << name <<" my"<<name<<" ;"<<endl;
    }
    this->srcFile<<"  int nb="<<numberOfLines<<" ;"<<endl<<endl;
    // on boucle sur toutes les variables pour recuperer les bornes
    if(testBounds.empty()){
      // we use bounds
      if(tests.empty())
	throw runtime_error("MaterialPropertyDSL::writeTestFile:\nNo bounds defined to draw graph !") ;
    }else tests=testBounds;
    
    for(p6=tests.begin();p6!=tests.end();++p6){
      if(p6->boundsType==VariableBoundsDescription::Lower){
	throw runtime_error("MaterialPropertyDSL::writeTestFile:\nNo upper bound defined to draw graph !") ;
      }
      if(p6->boundsType==VariableBoundsDescription::Upper){
	throw runtime_error("MaterialPropertyDSL::writeTestFile:\nNo lower bound defined to draw graph !") ;
      }
      this->srcFile<<"  const double " << p6->varName << "_min = " << p6->lowerBound << " ;" <<endl;
      this->srcFile<<"  const double " << p6->varName << "_max = " << p6->upperBound << " ;" <<endl;
      this->srcFile<<"  const double " << p6->varName << "_moy = (" 
		   << p6->varName << "_max - " << p6->varName << "_min)/2. ;" <<endl;
    }
    // fichier csv
    string csvFileName = name ;
    csvFileName+="_test" ;
    csvFileName+=".csv";
    this->srcFile<<"  ofstream file(\""<< csvFileName<<"\");" << endl;
    this->srcFile<<"  if (! file.is_open()) {" << endl
		 <<"    cerr << \"Could not open file "<<csvFileName<<"\" << endl;" << endl
		 <<"    return 1;" << endl
		 <<"  }" << endl ;
    // on boucle sur toutes les variables pour generer le fichier csv
    if(!inputs.empty()){
      for(p3=inputs.begin();p3!=inputs.end();++p3){
	this->srcFile<< "  double "<<p3->name<<"_interval = ("<<p3->name<<"_max - "<<p3->name<<"_min)/nb;"<<endl;
      }

      this->srcFile<< "  for(int i=0; i<nb; i++) {" <<endl ;
      for(p3=inputs.begin();p3!=inputs.end();++p3){
	this->srcFile<< "    file<<"<<p3->name<<"_min+i*"<<p3->name<<"_interval<<\" \""<<endl;
	// all min
	this->srcFile<< "        <<my"<<name<<".defaultCompute(";
	for(p4=inputs.begin();p4!=inputs.end();){
	  if (p3 != p4)
	    this->srcFile<< p4->name<<"_min" ;
	  if (p3 == p4)
	    this->srcFile<< p3->name<<"_min+i*"<<p3->name<<"_interval";
	  if((++p4)!=inputs.end())
	    this->srcFile << ",";
	}
	this->srcFile<< ") << \" \""<<endl;
	// all moy
	this->srcFile<< "        <<my"<<name<<".defaultCompute(";
	for(p4=inputs.begin();p4!=inputs.end();){
	  if (p3 != p4)
	    this->srcFile<< p4->name<<"_moy" ;
	  else
	    this->srcFile<< p3->name<<"_min+i*"<<p3->name<<"_interval";
	  if((++p4)!=inputs.end())
	    this->srcFile << ",";
	}
	this->srcFile<< ") << \" \""<<endl;
	// all max
	this->srcFile<< "        <<my"<<name<<".defaultCompute(";
	for(p4=inputs.begin();p4!=inputs.end();){
	  if (p3 != p4)
	    this->srcFile<< p4->name<<"_max" ;
	  else
	    this->srcFile<< p3->name<<"_min+i*"<<p3->name<<"_interval";
	  if((++p4)!=inputs.end())
	    this->srcFile << ",";
	}
	this->srcFile<< ") << \" \";"<<endl;	
      }
      this->srcFile<< "    file << endl ;"<<endl;
      this->srcFile<< "  }"<<endl;
    }

    this->srcFile<<"  return 0;" << endl
		 << "}" << endl ;
  } // end of CppTestMaterialPropertyInterface::writeSrcFile

} // end of namespace mfront
