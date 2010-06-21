/*!
 * \file   MFrontGnuplotLawInterface.cxx
 * \brief    
 * \author P. GOLDBRONN
 * \date   14 mai 2008
 */

#include<sstream>
#include<stdexcept>

#include"MFront/MFrontHeader.hxx"
#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontCppTestLawInterface.hxx"
#include"MFront/MFrontGnuplotLawInterface.hxx"

namespace mfront
{

  tfel::utilities::CxxTokenizer::TokensContainer::const_iterator
  MFrontGnuplotLawInterface::nextToken(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				       const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator endTokens,
				       const std::string &msg) {
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
  MFrontGnuplotLawInterface::getName(void)
  {
    return "gnuplot";
  }

  MFrontGnuplotLawInterface::MFrontGnuplotLawInterface()
    : verboseMode(false),
      debugMode(false),
      warningMode(false)
  {}

  MFrontGnuplotLawInterface::~MFrontGnuplotLawInterface()
  {}

  void
  MFrontGnuplotLawInterface::reset(void)
  {} // end of MFrontGnuplotLawInterface::reset
  
  void 
  MFrontGnuplotLawInterface::setVerboseMode(void)
  {
    this->verboseMode = true;
  }

  void 
  MFrontGnuplotLawInterface::setWarningMode(void)
  {
    this->warningMode = true;
  }

  void 
  MFrontGnuplotLawInterface::setDebugMode(void)
  {
    this->debugMode = true;
  }

  std::string
  MFrontGnuplotLawInterface::eraseQuote(const std::string& in)
  {
    using namespace std;
    string res(in);
    if ((res[0]=='"')&&(res[res.size()-1]=='"')) {
      res.erase(res.begin()) ;
      res.erase(res.end()-1) ;
    }
    return res;
  }

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontGnuplotLawInterface::treatKeyword(const std::string& key,
					  tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					  const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator endTokens)
  {
    using namespace std;
    if ( key == "@TestBounds" )
      return registerTestBounds(current,endTokens);
    if ( key == "@Graph" )
      return registerGraph(current,endTokens);
    else
      return make_pair(false,current);
  } // end of treatKeyword

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontGnuplotLawInterface::registerTestBounds(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
						const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator endTokens)
  {
    using namespace std;
    string msg("MFrontGnuplotLawInterface::registerTestBounds : ");
    VariableBoundsDescription boundsDescription;
    
    current=nextToken(--current,endTokens,msg);
    if(current->value=="{"){
      msg+="'{' is not allowed in graph bounds.\n";
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

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontGnuplotLawInterface::registerGraph(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					   const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator endTokens)
  {
    using namespace std;
    typedef multimap<string,dataFile>::value_type MVType;
    string msg("MFrontGnuplotLawInterface::registerGraph : ");
    current=nextToken(--current,endTokens,msg);
    if(current->value!="{"){
      msg+="expected '{'.\n";
      msg+="Error at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }
    current=nextToken(current,endTokens,msg);
    while((current->value!="}")&&
	  (current!=endTokens)){
      if(current->value=="{"){
	msg+="'{' is not allowed in graph.\n";
	msg+="Error at line : ";
	msg+=toString(current->line);
	throw(runtime_error(msg));
      }
      // on lit les mots clefs
      if (current->value == "Label") {
	//	current=readUntilEndOfInstruction(yLabel,++current,endTokens);
	// suppress " at begin and end of string if any
	
	
	current=nextToken(current,endTokens,msg);
	yLabel=eraseQuote(current->value);
	current=nextToken(current,endTokens,msg);
	if (current->value != ";") {
	  msg+="Label : you must give only a string.\n";
	  msg+="Error at line : ";
	  msg+=toString((--current)->line);
	  throw(runtime_error(msg));
	}
	current=nextToken(current,endTokens,msg);
      } else if (current->value == "ExperimentalData") {
	current=nextToken(current,endTokens,msg);
	string input = eraseQuote(current->value) ;
	current=nextToken(current,endTokens,msg);
	dataFile mydataFile;
	mydataFile.name = eraseQuote(current->value) ;
	current=nextToken(current,endTokens,msg);
	mydataFile.legend = eraseQuote(current->value) ;
	experimentalData.insert(MVType(input,mydataFile)) ;
	current=nextToken(current,endTokens,msg);
	if (current->value != ";") {
	  msg+="ExperimentalData : you provide more than three arguments.\n";
	  msg+="Error at line : ";
	  msg+=toString((--current)->line);
	  throw(runtime_error(msg));
	}
	current=nextToken(current,endTokens,msg);
	//      } else if (current->value == "xxx") {
      } else {
	msg+="Keyword ";
	msg+=current->value ;
	msg+=" is not allowed in graph.\n";
	msg+="Error at line : ";
	msg+=toString(current->line);
	throw(runtime_error(msg));
      }
      //current=nextToken(current,endTokens,msg);
    }
    if(current==endTokens){
      msg+="File ended before the end of graph.\n";
      msg+="Error at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }
    return make_pair(true,++current);
  } // end of registerGraph

  std::map<std::string,std::vector<std::string> >
  MFrontGnuplotLawInterface::getGlobalDependencies(const std::string&,
						   const std::string&,
						   const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    return libs;
  } // end of MFrontGnuplotLawInterface::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  MFrontGnuplotLawInterface::getGlobalIncludes(const std::string&,
					       const std::string&,
					       const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontGnuplotLawInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontGnuplotLawInterface::getGeneratedSources(const std::string&,
						 const std::string&,
						 const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontGnuplotLawInterface::getGeneratedSources

  std::vector<std::string>
  MFrontGnuplotLawInterface::getGeneratedIncludes(const std::string&,
						  const std::string&,
						  const std::string&)
  {
    using namespace std;
    vector<string> incs;
    return incs;
  } // end of MFrontGnuplotLawInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontGnuplotLawInterface::getLibrariesDependencies(const std::string&,
						      const std::string&,
						      const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontGnuplotLawInterface::getLibrariesDependencies()

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontGnuplotLawInterface::getSpecificTargets(const std::string&,
						const std::string& material,
						const std::string& className,
						const std::vector<std::string>&)
  {
    using namespace std;
    map<string,pair<vector<string>,vector<string> > > res;
    string name;
    if(material.empty()){
      name = className;
    } else {
      name = material+"_"+className;
    }
    string target = name+"GnuplotGraph";
    res[target].first.push_back(name+"CppTest");
    res[target].second.push_back("@./"+name+"CppTest");
    res[target].second.push_back(string("@")+string(GNUPLOT_PATH)+" "+name+".gp");
    res["graph"].first.push_back(target);
    res["check"].first.push_back(target);
    return res;
  } // end of MFrontGnuplotLawInterface::getSpecificTargets

  void
  MFrontGnuplotLawInterface::writeOutputFiles(const std::string& file,
					      const std::string& ,
					      const std::string& material,
					      const std::string& className,
					      const std::string& author,
					      const std::string& date,
					      const std::string& ,
					      const std::string& ,
					      const std::string& ,
					      const VarContainer& inputs,
					      const std::vector<std::string>&,
					      const std::map<std::string,std::string>&,
					      const std::map<std::string,std::string>&,
					      const StaticVarContainer& staticVars,
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
      name=className;
    } else {
      name=material+"_"+className;
    }
    this->srcFileName  = "src/" + name;
    this->srcFileName += ".gp";
    this->srcFile.open(this->srcFileName.c_str());
    if(!this->srcFile){
      string msg("MFrontMaterialLawParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.exceptions(ios::badbit|ios::failbit);
    this->writeSrcFile(file,name,author,date,
		       inputs,staticVars,params,paramValues,
		       function,bounds,physicalBounds,useTemplate,namespaces);
  } // end of MFrontGnuplotLawInterface::writeOutputFiles

  void
  MFrontGnuplotLawInterface::writeSrcFile(const std::string& file,
					  const std::string& name,
					  const std::string&,
					  const std::string&,
					  const VarContainer& inputs,
					  const StaticVarContainer&,
					  const std::vector<std::string>&,
					  const std::map<std::string,double>&,
					  const LawFunction&,
					  const std::vector<VariableBoundsDescription>& bounds,
					  const std::vector<VariableBoundsDescription>&,
					  const bool,
					  const std::vector<std::string>&)
  {
    using namespace std;
    vector<VariableBoundsDescription>::const_iterator p6;
    VarContainer::const_iterator p3;
    VarContainer::const_iterator p4;
    
    string base;

    if(file.find('/')!=string::npos){
      base = file.substr(0,file.rfind('/'))+'/';
    }

    // en attendant qu'on puisse passer testbounds
    if (testBounds.empty()) testBounds=bounds;
    
    this->srcFile<<"# generated by mfront from " << file << endl << endl;
    this->srcFile<< "set term postscript eps color enhanced" << endl
		 << "set encoding iso_8859_1" << endl
		 << "set grid" << endl << endl
		 << "set key under" << endl << endl;
    // On doit demander a l'utilisateur ce que l'on veut pour le placement de la legende !!!!
    if (!yLabel.empty())
      this->srcFile<< "set ylabel '" << yLabel << "'" << endl;
    
    // on boucle sur toutes les variables pour generer les plot
    string csvFileName = name ;
    csvFileName+="_test.csv";
    int column = 0 ;
    if(!inputs.empty()){
      if (inputs.size() == 1) {
	this->srcFile<< "set xlabel '"<< inputs.begin()->name <<"'"<<endl;
	this->srcFile<< "set output '"<< name << "_" << inputs.begin()->name << ".eps'" << endl;
	this->srcFile<< "plot '" << csvFileName << "' u 1:2 notitle" ;
	//adding experimental data if any
	for(multimap< string, dataFile >::const_iterator it=experimentalData.upper_bound(inputs[0].name);
	    it==experimentalData.lower_bound(inputs[0].name);++it)
	  this->srcFile<<"\\\n'../"<< base + it->second.name <<"' u 1:2 t '"<<it->second.legend<<"'" ;
	this->srcFile<<endl;
      } else {
	for(p3=inputs.begin();p3!=inputs.end();p3++){
	  this->srcFile<< "set xlabel '"<< p3->name <<"'"<<endl;
	  this->srcFile<< "set output '"<< name << "_" << p3->name << ".eps'" << endl ;
	  int columnIni=++column ;
	  this->srcFile<< "plot '" << csvFileName << "' u " <<columnIni<<":"<<++column<< " t '" ;
	  for(p4=inputs.begin();p4!=inputs.end();++p4){
	    if (p4 != p3) {
	      for(p6=testBounds.begin();p6!=testBounds.end();++p6){
		if (p6->varName == p4->name) {
		  this->srcFile<<p4->name<<"="<<p6->lowerBound ;
		  if (p3+1==inputs.end()) {
		    if (p4+2!=inputs.end())
		      this->srcFile<<",";
		  } else {
		    if (p4+1!=inputs.end())
		      this->srcFile<<",";
		  }
		}
	      }
	    } 
	  }
	  this->srcFile<< "'" ;
	  this->srcFile<<" w l lw 2 ";
	  this->srcFile<< ", '" << csvFileName << "' u " <<columnIni<<":"<<++column<< " t '" ;
	  for(p4=inputs.begin();p4!=inputs.end();++p4){
	    if (p4 != p3) {
	      // pb si vide !
	      for(p6=testBounds.begin();p6!=testBounds.end();++p6){
		if (p6->varName == p4->name) {
		  this->srcFile<<p4->name<<"="<<(p6->upperBound-p6->lowerBound)/2.;
		  if (p3+1==inputs.end()) {
		    if (p4+2!=inputs.end())
		      this->srcFile<<",";
		  } else {
		    if (p4+1!=inputs.end())
		      this->srcFile<<",";
		  }
		}
	      }
	    }	
	  }
	  this->srcFile<< "'" ;
	  this->srcFile<<" w l lw 2 ";
	  this->srcFile<< ", '" << csvFileName << "' u " <<columnIni<<":"<<++column<< " t '" ;
	  for(p4=inputs.begin();p4!=inputs.end();++p4){
	    if (p4 != p3) {
	      for(p6=testBounds.begin();p6!=testBounds.end();++p6){
		if (p6->varName == p4->name){
		  this->srcFile<<p4->name<<"="<<p6->upperBound;
		  if (p3+1==inputs.end()) {
		    if (p4+2!=inputs.end())
		      this->srcFile<<",";
		  } else {
		    if (p4+1!=inputs.end())
		      this->srcFile<<",";
		  }
		}
	      }
	    }
	  }
	  this->srcFile<< "'" ;
	  this->srcFile<<" w l lw 2 ";
	  //adding experimental data if any
	  for(multimap< string, dataFile >::const_iterator it=experimentalData.begin();it!=experimentalData.end();++it){
	    if (p3->name == (*it).first){
	      this->srcFile << "\\"<<endl<<",'../"<< base + (*it).second.name
			    << "' u 1:2 t '"<<(it->second).legend<<"' w p lw 2";
	    }
	  }
	  this->srcFile<<endl;
	}
      }
    }

  } // end of MFrontGnuplotLawInterface::writeSrcFile(void)

} // end of namespace mfront
