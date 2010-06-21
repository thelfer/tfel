/*!
 * \file   MFrontCLawInterfaceBase.hxx
 * \brief  This file declares the MFrontCLawInterfaceBase class
 * \author Helfer Thomas
 * \date   06 mai 2008
 */

#ifndef _LIB_MFRONTCLAWINTERFACEBASE_H_
#define _LIB_MFRONTCLAWINTERFACEBASE_H_ 

#include<string>
#include<fstream>

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/VarHandler.hxx"
#include"MFront/StaticVarHandler.hxx"
#include"MFront/LawFunction.hxx"
#include"MFront/VariableBoundsDescription.hxx"
#include"MFront/MFrontLawVirtualInterface.hxx"

namespace mfront{

  struct MFrontCLawInterfaceBase
    : public MFrontLawVirtualInterface
  {
    
    MFrontCLawInterfaceBase();
    
    void setVerboseMode(void);

    void setDebugMode(void);

    void setWarningMode(void);
    std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);

    /*!
     * \param const std::string&, name of the original file
     * \param const std::string&, name of the output library
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     * \param const std::string&, author
     * \param const std::string&, date
     * \param const std::string&, description
     * \param const std::string&, inputs
     * \param const std::string&, output name
     * \param const VarContainer&, inputs
     * \param const std::vector<std::string>&, material laws
     * \param const std::map<std::string,std::string>&, glossary names,
     * \param const std::map<std::string,std::string>&, entry names,
     * \param const StaticVarContainer&, static variables
     * \param const std::vector<std::string>&, parameters
     * \param const std::map<std::string,double>&, parameters values
     * \param const LawFunction&, function definition
     * \param const std::vector<VariableBoundsDescription>&, bounds of the law
     * \param const std::vector<VariableBoundsDescription>&, physical bounds of the law
     * \param const bool, use template
     * \param const std::vector<std::string>&, namespaces
     */
    void writeOutputFiles(const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const VarContainer&,
			  const std::vector<std::string>&,
			  const std::map<std::string,std::string>&,
			  const std::map<std::string,std::string>&,
			  const StaticVarContainer& staticVars,
			  const std::vector<std::string>&,
			  const std::map<std::string,double>&,
			  const LawFunction&,
			  const std::vector<VariableBoundsDescription>&,
			  const std::vector<VariableBoundsDescription>&,
			  const bool,
			  const std::vector<std::string>&);
    
    void
    reset(void);

    virtual ~MFrontCLawInterfaceBase();
    
  protected:

    static void
    replace(std::string&,
	    const std::string::value_type,
	    const std::string::value_type);

    std::string
    transformHeaderName(const std::string&,
			const std::string&);

    virtual void
    writeParameterList(std::ostream&,
		       const VarContainer&);

    virtual void
    writeInterfaceSpecificVariables(const VarContainer&);

    virtual void
    writeBeginHeaderNamespace(void) = 0;

    virtual void
    writeEndHeaderNamespace(void) = 0;

    virtual void
    writeBeginSrcNamespace(void) = 0;

    virtual void
    writeEndSrcNamespace(void) = 0;

    virtual void
    writeHeaderPreprocessorDirectives(const std::string&,
				      const std::string&) = 0;

    virtual void
    writeSrcPreprocessorDirectives(const std::string&,
				   const std::string&) = 0;

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string
    getFunctionDeclaration(const std::string&,
			   const std::string&) = 0;

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string
    getCheckBoundsFunctionDeclaration(const std::string&,
				      const std::string&) = 0;

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string
    getHeaderFileName(const std::string&,
		      const std::string&) = 0;

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string
    getSrcFileName(const std::string&,
		   const std::string&) = 0;

    /*
     * \param const std::string&, material
     * \param const std::string&, className
     * \param const std::string&, author
     * \param const std::string&, date
     * \param const std::string&, description
     * \param const VarContainer&, inputs
     * \param const std::vector<VariableBoundsDescription>&, bounds of the law
     * \param const std::vector<VariableBoundsDescription>&, physical bounds of the law
     */
    void writeHeaderFile(const std::string&,
			 const std::string&,
			 const std::string&,
			 const std::string&,
			 const std::string&,
			 const VarContainer&,
			 const std::vector<VariableBoundsDescription>&,
			 const std::vector<VariableBoundsDescription>&);

    /*
     * \param const std::string&, name of the original file
     * \param const std::string&, material
     * \param const std::string&, className
     * \param const std::string&, author
     * \param const std::string&, date
     * \param const std::string&, includes
     * \param const std::string&, output name
     * \param const VarContainer&, inputs
     * \param const std::vector<std::string>&, material laws
     * \param const StaticVarContainer&, static variables
     * \param const std::vector<std::string>&, parameters
     * \param const std::map<std::string,double>&, parameters values
     * \param const LawFunction&, function definition
     * \param const std::vector<VariableBoundsDescription>&, bounds of the law
     * \param const std::vector<VariableBoundsDescription>&, physical bounds of the law
     */
    void writeSrcFile(const std::string&,
		      const std::string&,
		      const std::string&,
		      const std::string&,
		      const std::string&,
		      const std::string&,
		      const std::string&,
		      const VarContainer&,
		      const std::vector<std::string>&,
		      const StaticVarContainer& staticVars,
		      const std::vector<std::string>&,
		      const std::map<std::string,double>&,
		      const LawFunction&,
		      const std::vector<VariableBoundsDescription>&,
		      const std::vector<VariableBoundsDescription>&);

    std::ofstream headerFile;

    std::ofstream srcFile;

    std::string headerFileName;

    std::string srcFileName;
    
    bool verboseMode;

    bool debugMode;

    bool warningMode;

  }; // end of MfrontCLawInterfaceBase

} // end of namespace mfront

#endif /* _LIB_MFRONTCLAWINTERFACEBASE_H */

