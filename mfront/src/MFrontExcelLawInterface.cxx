/*!
 * \file   mfront/src/MFrontExcelLawInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<algorithm>
#include<fstream>
#include<sstream>
#include<stdexcept>
#include<iterator>
#include<string>
#include<set>

#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#include"TFEL/System/System.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/MFrontCLawInterface.hxx"
#include"MFront/MFrontExcelLawInterface.hxx"

namespace mfront
{

  struct MFrontExcelLawInterface::ExcelInterface
  {
    std::string function;
    std::string library;
    std::vector<std::string> variables;
  }; // end of ExcelInterface

  void
  MFrontExcelLawInterface::readExcelInterface(std::vector<MFrontExcelLawInterface::ExcelInterface>& interfaces,
					      const std::string& line)
  {
    using namespace std;
    if(!line.empty()){
      istringstream tokenizer(line);
      vector<string> tokens;
      ExcelInterface i;
      vector<string>::const_iterator p;
      copy(istream_iterator<string>(tokenizer),
	   istream_iterator<string>(),back_inserter(tokens));
      if(tokens.size()<2){
	string msg("MFrontExcelLawInterface::readExcelInterface : ");
	msg += "invalid line '"+line+"'";
	throw(runtime_error(msg));
      }
      i.function  = tokens[0];
      i.library   = tokens[1];
      for(p=tokens.begin()+2;p!=tokens.end();++p){
	i.variables.push_back(*p);
      }
      interfaces.push_back(i);
    }
  } // end of readExcelInterface
    
  std::string
  MFrontExcelLawInterface::getName(void)
  {
    return "excel";
  }

  MFrontExcelLawInterface::MFrontExcelLawInterface()
  {}

  void
  MFrontExcelLawInterface::reset(void)
  {} // end of MFrontExcelLawInterface::reset(void)

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontExcelLawInterface::treatKeyword(const std::string&,
					 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    return make_pair(false,current);
  } // end of treatKeyword

  MFrontExcelLawInterface::~MFrontExcelLawInterface()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontExcelLawInterface::getGlobalDependencies(const std::string&,
						 const std::string&,
						 const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontExcelLawInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontExcelLawInterface::getGlobalIncludes(const std::string&,
					     const std::string&,
					     const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontExcelLawInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontExcelLawInterface::getGeneratedSources(const std::string&,
					       const std::string&,
					       const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontExcelLawInterface::getGeneratedSources

  std::vector<std::string>
  MFrontExcelLawInterface::getGeneratedIncludes(const std::string&,
						const std::string&,
						const std::string&)
  {
    using namespace std;
    return vector<string>();
  } // end of MFrontExcelLawInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontExcelLawInterface::getLibrariesDependencies(const std::string&,
						    const std::string&,
						    const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontExcelLawInterface::getLibrariesDependencies()

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontExcelLawInterface::getSpecificTargets(const std::string&,
					      const std::string&,
					      const std::string&,
					      const std::vector<std::string>&)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of MFrontExcelLawInterface::getSpecificTargets

  void
  MFrontExcelLawInterface::writeOutputFiles(const std::string&,
					    const std::string& library,
					    const std::string& material,
					    const std::string& className,
					    const std::string&,
					    const std::string&,
					    const std::string&,
					    const std::string&,
					    const std::string&,
					    const VariableDescriptionContainer& vars,
					    const std::vector<std::string>&,
					    const std::map<std::string,std::string>&,
					    const std::map<std::string,std::string>&,
					    const StaticVariableDescriptionContainer&,
					    const std::vector<std::string>&,
					    const std::map<std::string,double>&,
					    const LawFunction&,
					    const std::vector<VariableBoundsDescription>&,
					    const std::vector<VariableBoundsDescription>&,
					    const bool,
					    const std::vector<std::string>&)
  {
    using namespace std;
    using namespace tfel::system;
    // writing excel interface
    MFrontLock& lock = MFrontLock::getMFrontLock();
    lock.lock();
    vector<ExcelInterface> interfaces;
    map<string,vector<ExcelInterface> > interfaces2;
    vector<ExcelInterface>::iterator p;
    vector<ExcelInterface>::iterator p2;
    VariableDescriptionContainer::const_iterator p3;
    map<string,vector<ExcelInterface> >::const_iterator p4;
    vector<ExcelInterface>::const_iterator p5;
    vector<string>::const_iterator p6;
    string fname("src/excel.lst");
    string name;
    string lib;
    if(material.empty()){
      name = className;
    } else {
      name = material+"_"+className;
    }
    struct stat buffer; // for call to stat
    if(access(fname.c_str(),F_OK)==0){
      stat(fname.c_str(),&buffer);
      if(!S_ISREG(buffer.st_mode)){
	string msg("MFrontExcelLawInterface::writeOutputFiles : '"+fname+"' is not a regular file");
	throw(runtime_error(msg));
      }
      fstream iwrapper(fname.c_str());
      if(!iwrapper){
	string msg("MFrontExcelLawInterface::writeOutputFiles : ");
	msg += "unable to open '"+fname+"'";
	throw(runtime_error(msg));
      }
      while(!iwrapper.eof()){
	string line;
	if(!iwrapper.good()){
	  string msg("MFrontExcelLawInterface::writeOutputFiles : error while reading file ");
	  msg += fname;
	  throw(runtime_error(msg));
	}
	getline(iwrapper,line);
	MFrontExcelLawInterface::readExcelInterface(interfaces,line);
      }
      iwrapper.close();
    }
    lib = "lib"+getMaterialLawLibraryNameBase(library,material);
    p2 = interfaces.end();
    for(p=interfaces.begin();p!=interfaces.end();++p){
      if((p->function==name)&&
	 (p->library==lib)){
	p2 = p;
      }
    }
    if(p2==interfaces.end()){
      interfaces.resize(interfaces.size()+1);
      p2 = interfaces.end();
      --p2;
    }
    p2->function = name;
    p2->library  = lib;
    p2->variables.clear();
    for(p3=vars.begin();p3!=vars.end();++p3){
      p2->variables.push_back(p3->name);
    }
    ofstream wrapper(fname.c_str());
    for(p=interfaces.begin();p!=interfaces.end();++p){
      wrapper << p->function << " " << p->library << " ";
      copy(p->variables.begin(),p->variables.end(),
	   ostream_iterator<string>(wrapper," "));
      interfaces2[p->library].push_back(*p);
      wrapper << endl;
    }
    for(p4=interfaces2.begin();p4!=interfaces2.end();++p4){
      string libName = p4->first;
      ofstream file(("src/"+libName+".bas").c_str());
      if(!file){
	string msg("MFrontExcelLawInterface::writeOutputFiles : ");
	msg += "";
	throw(runtime_error(msg));
      }
      for(p5=p4->second.begin();p5!=p4->second.end();++p5){
	file << "Declare Function " << p5->function 
	     << " Lib \"" << libName << ".dll\" (";
	for(p6=p5->variables.begin();
	    p6!=p5->variables.end();){
	  file << "ByVal " << *p6 << " As Double";
	  if(++p6!=p5->variables.end()){
	    file << ",";
	  }
	}
	file << ") As Double" << endl;
      }
    }
    wrapper.close();
    lock.unlock();
  } // end of MFrontExcelLawInterface::writeSrcFile(void)

} // end of namespace mfront
