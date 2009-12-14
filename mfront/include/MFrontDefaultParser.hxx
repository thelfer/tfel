/*!
 * \file   MFrontDefaultParser.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
 */

#ifndef _LIB_MFRONTPARSER_HXX_
#define _LIB_MFRONTPARSER_HXX_ 

#include<string>
#include"MFrontVirtualParser.hxx"
#include"MFrontBehaviourParserBase.hxx"

namespace mfront{

  struct MFrontDefaultParser
    : public MFrontVirtualParser,
      public MFrontBehaviourParserBase<MFrontDefaultParser>
  {

    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

    MFrontDefaultParser();

    void setVerboseMode(void);
    
    void setDebugMode(void);

    void setWarningMode(void);

    void
    setInterfaces(const std::set<std::string>&);

    void treatFile(const std::string&);

    void writeOutputFiles();

    std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(void);

    std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(void);

    std::map<std::string,std::vector<std::string> >
    getGeneratedSources(void);

    std::vector<std::string>
    getGeneratedIncludes(void);

    std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(void);

    std::map<std::string,
	     std::pair<std::vector<std::string>,
		       std::vector<std::string> > >
    getSpecificTargets(void);

    ~MFrontDefaultParser();

  private:

    void generateOutputFiles();

  }; // end of struct MFrontDefaultParser

} // end of namespace mfront  

#endif /* _LIB_MFRONTPARSER_HXX */

