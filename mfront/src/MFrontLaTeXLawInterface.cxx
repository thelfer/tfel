/*!
 * \file   MFrontLaTeXLawInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 mai 2008
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
#include"MFront/MFrontCLawInterface.hxx"
#include"MFront/MFrontLaTeXLawInterface.hxx"

namespace mfront
{
    
  std::string
  MFrontLaTeXLawInterface::getName(void)
  {
    return "LaTeX";
  }

  MFrontLaTeXLawInterface::MFrontLaTeXLawInterface()
    : verboseMode(false),
      debugMode(false),
      warningMode(false)
  {}

  void
  MFrontLaTeXLawInterface::reset(void)
  {} // end of MFrontLaTeXLawInterface::reset(void)
  
  void 
  MFrontLaTeXLawInterface::setVerboseMode(void)
  {
    this->verboseMode = true;
  }

  void 
  MFrontLaTeXLawInterface::setWarningMode(void)
  {
    this->warningMode = true;
  }

  void 
  MFrontLaTeXLawInterface::setDebugMode(void)
  {
    this->debugMode = true;
  }

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontLaTeXLawInterface::treatKeyword(const std::string&,
					 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    return make_pair(false,current);
  } // end of treatKeyword

  MFrontLaTeXLawInterface::~MFrontLaTeXLawInterface()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontLaTeXLawInterface::getGlobalDependencies(const std::string&,
						 const std::string&,
						 const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontLaTeXLawInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontLaTeXLawInterface::getGlobalIncludes(const std::string&,
					     const std::string&,
					     const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontLaTeXLawInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontLaTeXLawInterface::getGeneratedSources(const std::string&,
					       const std::string&,
					       const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontLaTeXLawInterface::getGeneratedSources

  std::vector<std::string>
  MFrontLaTeXLawInterface::getGeneratedIncludes(const std::string&,
						const std::string&,
						const std::string&)
  {
    using namespace std;
    return vector<string>();
  } // end of MFrontLaTeXLawInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontLaTeXLawInterface::getLibrariesDependencies(const std::string&,
						    const std::string&,
						    const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontLaTeXLawInterface::getLibrariesDependencies()

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontLaTeXLawInterface::getSpecificTargets(const std::string&,
					      const std::string&,
					      const std::string&,
					      const std::vector<std::string>&)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of MFrontLaTeXLawInterface::getSpecificTargets

  void
  MFrontLaTeXLawInterface::writeOutputFiles(const std::string&,
					    const std::string& library,
					    const std::string& material,
					    const std::string& className,
					    const std::string&,
					    const std::string&,
					    const std::string&,
					    const std::string&,
					    const std::string&,
					    const VarContainer& vars,
					    const std::vector<std::string>&,
					    const std::map<std::string,std::string>&,
					    const std::map<std::string,std::string>&,
					    const StaticVarContainer&,
					    const std::vector<std::string>&,
					    const std::map<std::string,double>&,
					    const LawFunction&,
					    const std::vector<VariableBoundsDescription>&,
					    const std::vector<VariableBoundsDescription>&,
					    const bool,
					    const std::vector<std::string>&)
  {
    using namespace std;
    string name;
    if(material.empty()){
      name = className;
    } else {
      name = material+"_"+className;
    }
  } // end of MFrontLaTeXLawInterface::writeSrcFile(void)

} // end of namespace mfront
