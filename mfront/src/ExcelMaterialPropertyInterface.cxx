/*!
 * \file   mfront/src/ExcelMaterialPropertyInterface.cxx
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

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/CMaterialPropertyInterface.hxx"
#include"MFront/ExcelMaterialPropertyInterface.hxx"

namespace mfront
{

  struct ExcelMaterialPropertyInterface::ExcelInterface
  {
    ExcelInterface() = default;
    ExcelInterface(ExcelInterface&&) = default;
    ExcelInterface(const ExcelInterface&) = default;
    ExcelInterface& operator=(ExcelInterface&&) = default;
    ExcelInterface& operator=(const ExcelInterface&) = default;
    ~ExcelInterface();
    std::vector<std::string> variables;
    std::string function;
    std::string library;
  }; // end of ExcelInterface

  ExcelMaterialPropertyInterface::ExcelInterface::~ExcelInterface()
  {} // end of ExcelMaterialPropertyInterface::ExcelMaterialPropertyInterface::~ExcelInterface

  void
  ExcelMaterialPropertyInterface::readExcelInterface(std::vector<ExcelMaterialPropertyInterface::ExcelInterface>& interfaces,
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
	string msg("ExcelMaterialPropertyInterface::readExcelInterface : ");
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
  ExcelMaterialPropertyInterface::getName(void)
  {
    return "excel";
  }

  ExcelMaterialPropertyInterface::ExcelMaterialPropertyInterface()
  {}

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  ExcelMaterialPropertyInterface::treatKeyword(const std::string&,
					 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    return make_pair(false,current);
  } // end of treatKeyword

  ExcelMaterialPropertyInterface::~ExcelMaterialPropertyInterface()
  {}

  void
  ExcelMaterialPropertyInterface::getTargetsDescription(TargetsDescription&,
							const MaterialPropertyDescription&)
  {} // end of ExcelMaterialPropertyInterface::getTargetsDescription

  void
  ExcelMaterialPropertyInterface::writeOutputFiles(const MaterialPropertyDescription& mpd,
						   const FileDescription&)
  {
    using namespace std;
    using namespace tfel::system;
    const auto& library=mpd.library;
    const auto& material=mpd.material;
    const auto& className=mpd.className;
    const auto& vars=mpd.inputs;
    // writing excel interface
    auto& lock = MFrontLock::getMFrontLock();
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
	string msg("ExcelMaterialPropertyInterface::writeOutputFiles : '"+fname+"' is not a regular file");
	throw(runtime_error(msg));
      }
      fstream iwrapper(fname.c_str());
      if(!iwrapper){
	string msg("ExcelMaterialPropertyInterface::writeOutputFiles : ");
	msg += "unable to open '"+fname+"'";
	throw(runtime_error(msg));
      }
      while(!iwrapper.eof()){
	string line;
	if(!iwrapper.good()){
	  string msg("ExcelMaterialPropertyInterface::writeOutputFiles : error while reading file ");
	  msg += fname;
	  throw(runtime_error(msg));
	}
	getline(iwrapper,line);
	ExcelMaterialPropertyInterface::readExcelInterface(interfaces,line);
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
	string msg("ExcelMaterialPropertyInterface::writeOutputFiles : ");
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
  } // end of ExcelMaterialPropertyInterface::writeSrcFile(void)

} // end of namespace mfront
