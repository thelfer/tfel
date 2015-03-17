/*!
 * \file   mfront/src/GnuplotMaterialPropertyInterface.cxx
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

#include"MFront/MFrontHeader.hxx"
#include"MFront/DSLUtilities.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/GnuplotMaterialPropertyInterface.hxx"

namespace mfront
{

  tfel::utilities::CxxTokenizer::TokensContainer::const_iterator
  GnuplotMaterialPropertyInterface::nextToken(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				       const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator endTokens,
				       const std::string &msg) {
    using namespace std;
    string what(msg) ;
    if(++current==endTokens){
      what+="unexpected end of file.\n";
      what+="Error at line : ";
      what+=to_string((--current)->line);
      throw(std::runtime_error(what));
    }
    return current;
  }

  std::string
  GnuplotMaterialPropertyInterface::getName(void)
  {
    return "gnuplot";
  }

  GnuplotMaterialPropertyInterface::GnuplotMaterialPropertyInterface()
  {}

  GnuplotMaterialPropertyInterface::~GnuplotMaterialPropertyInterface()
  {}
  
  std::string
  GnuplotMaterialPropertyInterface::eraseQuote(const std::string& in)
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
  GnuplotMaterialPropertyInterface::treatKeyword(const std::string& key,
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
  GnuplotMaterialPropertyInterface::registerTestBounds(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
						const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator endTokens)
  {
    using namespace std;
    string msg("GnuplotMaterialPropertyInterface::registerTestBounds : ");
    VariableBoundsDescription boundsDescription;
    
    current=nextToken(--current,endTokens,msg);
    if(current->value=="{"){
      msg+="'{' is not allowed in graph bounds.\n";
      msg+="Error at line : ";
      msg+=to_string(current->line);
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
      msg+=to_string(current->line);
      throw(runtime_error(msg));
    }
    current=nextToken(current,endTokens,msg);
    if(current->value!="["){
      msg+="expected '[' (read ";
      msg+=current->value;
      msg+=")\nError at line : ";
      msg+=to_string(current->line);
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
      msg+=to_string(current->line);
      throw(runtime_error(msg));
    }
    current=nextToken(current,endTokens,msg);
    if(current->value!=":"){
      msg+="expected ':' (read ";
      msg+=current->value;
      msg+=")\nError at line : ";
      msg+=to_string(current->line);
      throw(runtime_error(msg));
    }
    current=nextToken(current,endTokens,msg);
    istringstream converterU(current->value);
    converterU >> boundsDescription.upperBound;
    if(!converterU&&(!converterU.eof())){
      msg+="could not read upper bound value\nRead : ";
      msg+=current->value;
      msg+="\nError at line : ";
      msg+=to_string(current->line);
      throw(runtime_error(msg));
    }
    if(boundsDescription.boundsType==VariableBoundsDescription::LowerAndUpper){
      if(boundsDescription.lowerBound>boundsDescription.upperBound){
	msg+="lower bound value is greater than upper bound value for variable ";
	msg+=boundsDescription.varName;
	msg+="\nError at line : ";
	msg+=to_string(current->line);
	throw(runtime_error(msg));
      }
    }
    current=nextToken(current,endTokens,msg);
    if(current->value!="]"){
      msg+="expected ']' (read ";
      msg+=current->value;
      msg+=")\nError at line : ";
      msg+=to_string(current->line);
      throw(runtime_error(msg));
    }      
    current=nextToken(current,endTokens,msg);
    if(current->value!=";"){
      msg+="expected ';' (read ";
      msg+=current->value;
      msg+=")\nError at line : ";
      msg+=to_string(current->line);
      throw(runtime_error(msg));
    }      
    testBounds.push_back(boundsDescription);
    return make_pair(true,++current);
  } // end of registerTestBounds

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  GnuplotMaterialPropertyInterface::registerGraph(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					   const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator endTokens)
  {
    using namespace std;
    string msg("GnuplotMaterialPropertyInterface::registerGraph : ");
    current=nextToken(--current,endTokens,msg);
    if(current->value!="{"){
      msg+="expected '{'.\n";
      msg+="Error at line : ";
      msg+=to_string(current->line);
      throw(runtime_error(msg));
    }
    current=nextToken(current,endTokens,msg);
    while((current->value!="}")&&
	  (current!=endTokens)){
      if(current->value=="{"){
	msg+="'{' is not allowed in graph.\n";
	msg+="Error at line : ";
	msg+=to_string(current->line);
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
	  msg+=to_string((--current)->line);
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
	experimentalData.insert({input,mydataFile}) ;
	current=nextToken(current,endTokens,msg);
	if (current->value != ";") {
	  msg+="ExperimentalData : you provide more than three arguments.\n";
	  msg+="Error at line : ";
	  msg+=to_string((--current)->line);
	  throw(runtime_error(msg));
	}
	current=nextToken(current,endTokens,msg);
	//      } else if (current->value == "xxx") {
      } else {
	msg+="Keyword ";
	msg+=current->value ;
	msg+=" is not allowed in graph.\n";
	msg+="Error at line : ";
	msg+=to_string(current->line);
	throw(runtime_error(msg));
      }
      //current=nextToken(current,endTokens,msg);
    }
    if(current==endTokens){
      msg+="File ended before the end of graph.\n";
      msg+="Error at line : ";
      msg+=to_string(current->line);
      throw(runtime_error(msg));
    }
    return make_pair(true,++current);
  } // end of registerGraph

  std::map<std::string,std::vector<std::string> >
  GnuplotMaterialPropertyInterface::getGlobalDependencies(const std::string&,
						   const std::string&,
						   const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    return libs;
  } // end of GnuplotMaterialPropertyInterface::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  GnuplotMaterialPropertyInterface::getGlobalIncludes(const std::string&,
					       const std::string&,
					       const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of GnuplotMaterialPropertyInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  GnuplotMaterialPropertyInterface::getGeneratedSources(const std::string&,
						 const std::string&,
						 const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of GnuplotMaterialPropertyInterface::getGeneratedSources

  std::vector<std::string>
  GnuplotMaterialPropertyInterface::getGeneratedIncludes(const std::string&,
						  const std::string&,
						  const std::string&)
  {
    using namespace std;
    vector<string> incs;
    return incs;
  } // end of GnuplotMaterialPropertyInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  GnuplotMaterialPropertyInterface::getLibrariesDependencies(const std::string&,
						      const std::string&,
						      const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of GnuplotMaterialPropertyInterface::getLibrariesDependencies()

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  GnuplotMaterialPropertyInterface::getSpecificTargets(const std::string&,
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
  } // end of GnuplotMaterialPropertyInterface::getSpecificTargets
  
  std::map<std::string,std::vector<std::string> >
  GnuplotMaterialPropertyInterface::getGeneratedEntryPoints(const std::string&,
							    const std::string&,
							    const std::string&)
  {
    return {};
  } // end of GnuplotMaterialPropertyInterface::getGeneratedEntryPoints

  void
  GnuplotMaterialPropertyInterface::writeOutputFiles(const MaterialPropertyDescription& mpd,
						     const FileDescription& fd)
  {
    using namespace std;
    const auto& file=fd.fileName;
    const auto& className=mpd.className;
    const auto& material=mpd.material;
    const auto& inputs=mpd.inputs;
    const auto& bounds=mpd.boundsDescriptions;
    const auto name = (material.empty()) ? className : material+"_"+className;
    this->srcFileName  = "src/" + name;
    this->srcFileName += ".gp";
    this->srcFile.open(this->srcFileName.c_str());
    if(!this->srcFile){
      string msg("MaterialPropertyDSL::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.exceptions(ios::badbit|ios::failbit);
    // writing src file
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
	for(multimap< string, dataFile >::const_iterator it=experimentalData.begin();it!=experimentalData.end();++it){
	  if (inputs[0].name == (*it).first){
	    this->srcFile<<",\\\n'../"<< base + it->second.name <<"' u 1:2 t '"<<it->second.legend<<"'" ;
	  }
	}
	this->srcFile<<endl;
      } else {
	for(auto p3=inputs.begin();p3!=inputs.end();p3++){
	  this->srcFile<< "set xlabel '"<< p3->name <<"'"<<endl;
	  this->srcFile<< "set output '"<< name << "_" << p3->name << ".eps'" << endl ;
	  int columnIni=++column ;
	  this->srcFile<< "plot '" << csvFileName << "' u " <<columnIni<<":"<<++column<< " t '" ;
	  for(auto p4=inputs.begin();p4!=inputs.end();++p4){
	    if (p4 != p3) {
	      for(auto p6=testBounds.begin();p6!=testBounds.end();++p6){
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
	  for(auto p4=inputs.begin();p4!=inputs.end();++p4){
	    if (p4 != p3) {
	      // pb si vide !
	      for(auto p6=testBounds.begin();p6!=testBounds.end();++p6){
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
	  for(auto p4=inputs.begin();p4!=inputs.end();++p4){
	    if (p4 != p3) {
	      for(auto p6=testBounds.begin();p6!=testBounds.end();++p6){
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
	  for(auto it=experimentalData.begin();it!=experimentalData.end();++it){
	    if (p3->name == (*it).first){
	      this->srcFile << "\\"<<endl<<",'../"<< base + (*it).second.name
			    << "' u 1:2 t '"<<(it->second).legend<<"' w p lw 2";
	    }
	  }
	  this->srcFile<<endl;
	}
      }
      this->srcFile.close();
    }

  } // end of GnuplotMaterialPropertyInterface::writeSrcFile(void)

} // end of namespace mfront
