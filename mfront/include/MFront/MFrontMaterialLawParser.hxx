/*!
 * \file   mfront/include/MFront/MFrontMaterialLawParser.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 jui 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTMATERIALLAWPARSER_HXX_
#define _LIB_MFRONTMATERIALLAWPARSER_HXX_ 

#include<string>
#include<set>

#include"MFront/ParserBase.hxx"
#include"MFront/LawFunction.hxx"
#include"MFront/VariableBoundsDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/MFrontVirtualParser.hxx"

namespace mfront{

  /*!
   * Parser handling material properties
   */
  struct TFEL_VISIBILITY_EXPORT MFrontMaterialLawParser
    : public MFrontVirtualParser,
      public ParserBase,
      protected MaterialPropertyDescription
  {
    /*!
     * return the name of the parser
     */
    static std::string 
    getName(void);
    /*!
     * return a description of the parser
     */
    static std::string
    getDescription(void);
    /*!
     * default constructor
     */
    MFrontMaterialLawParser();

    virtual void
    getKeywordsList(std::vector<std::string>&) const;

    virtual void
    setInterfaces(const std::set<std::string>&);

    /*!
     * set the analysers to be used
     */
    virtual void
    setAnalysers(const std::set<std::string>&);

    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(void);

    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(void);

    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(void);

    virtual std::vector<std::string>
    getGeneratedIncludes(void);

    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(void);

    virtual std::map<std::string,
		     std::pair<std::vector<std::string>,
			       std::vector<std::string> > >
    getSpecificTargets(void);
    /*!
     * \brief analyse a file and generates output files
     * \param[in] f     : file name
     * \param[in] ecmds : additionnal commands inserted treated before
     * the input file commandes (those commands are given through the
     * --@?? option of the command line
     * \note the method barely calls the analyseFile method and the
     * generateOutputFiles
     */
    virtual void
    treatFile(const std::string&,
	      const std::vector<std::string>&);
    /*!
     * \brief analyse a file without generating any output
     * \param[in] f     : file name
     * \param[in] ecmds : additionnal commands inserted treated before
     */
    virtual void
    analyseFile(const std::string&,
		const std::vector<std::string>& = std::vector<std::string>());
    
    virtual const MaterialPropertyDescription&
    getMaterialPropertyDescription(void) const;

  protected:

    typedef void (MFrontMaterialLawParser::* MemberFuncPtr)(void);
    typedef std::map<std::string,MemberFuncPtr> CallBackContainer;
    /*!
     * \return the name of the generated class
     */
    virtual std::string getClassName(void) const;
    /*!
     * \brief add a material law
     * \param[in] m : added material law name
     */
    virtual void addMaterialLaw(const std::string&);
    /*!
     * \brief append the given code to the includes
     */
    virtual void appendToIncludes(const std::string&);
    /*!
     * \brief append the given code to the members
     */
    virtual void appendToMembers(const std::string&);
    /*!
     * \brief append the given code to the private code
     */
    virtual void appendToPrivateCode(const std::string&);
    /*!
     * \brief append the given code to the sources
     */
    virtual void appendToSources(const std::string&);
    /*!
     * \brief write the output files.
     * \note this shall be called after the analyseFile method.
     */
    virtual void
    writeOutputFiles(void);
    /*!
     * \brief add a static variable description
     * \param[in] v : variable description
     */
    virtual void
    addStaticVariableDescription(const StaticVariableDescription&);

    void
    addInterface(const std::string&);
    /*!
     * treat the material keyword
     */
    virtual void
    treatMaterial(void);
    /*!
     * treat the library keyword
     */
    virtual void
    treatLibrary(void);

    void
    treatNamespace(void);

    void
    treatUseTemplate(void);

    void
    treatOutput(void);

    void
    treatInput(void);

    void
    treatLaw(void);

    void
    treatFunction(void);

    /*!
     * treat the setGlossaryName and the setEntryName method for
     * inputs or output.
     */
    void
    treatMethod(void);

    void
    treatInterface(void);

    void
    treatParameter(void);

    void
    treatConstant(void);

    void
    treatBounds(void);

    void
    treatPhysicalBounds(void);

    void
    treatUnknownKeyword(void);

    void
    registerBounds(std::vector<VariableBoundsDescription>&);
    
    void
    registerNewCallBack(const std::string&,const MemberFuncPtr);
    /*!
     * registred interfaces for the material property being treated
     */
    std::set<std::string> interfaces;
    /*!
     * list of resgistred keywords
     */
    std::set<std::string> registredKeyWords;
    /*!
     * list of dependencies
     */
    std::map<std::string,std::vector<std::string> > sourcesLibrairiesDependencies;
    /*!
     * map of callbacks used during the file interpretation
     */
    CallBackContainer callBacks;

    std::vector<std::string> namespaces;
    bool useTemplate;
    /*!
     * a temporary object used to store the name of the variable on
     * which a method is called
     */
    std::string currentVar;

  }; // end of class MFrontMaterialLawParser

} // end of namespace mfront  
  
#endif /* _LIB_MFRONTMATERIALLAWPARSER_HXX */

