/*!
 * \file   mfront/include/MFront/CMaterialPropertyInterfaceBase.hxx
 * \brief  This file declares the CMaterialPropertyInterfaceBase class
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTCLAWINTERFACEBASE_H_
#define LIB_MFRONTCLAWINTERFACEBASE_H_ 

#include<string>
#include<fstream>

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/LawFunction.hxx"
#include"MFront/VariableBoundsDescription.hxx"
#include"MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront{

  struct CMaterialPropertyInterfaceBase
    : public AbstractMaterialPropertyInterface
  {
    
    CMaterialPropertyInterfaceBase();
    
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
     * \param const VariableDescriptionContainer&, inputs
     * \param const std::vector<std::string>&, material laws
     * \param const std::map<std::string,std::string>&, glossary names,
     * \param const std::map<std::string,std::string>&, entry names,
     * \param const StaticVariableDescriptionContainer&, static variables
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
			  const VariableDescriptionContainer&,
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
			  const std::vector<std::string>&);
    
    void
    reset(void);

    virtual ~CMaterialPropertyInterfaceBase();
    
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
		       const VariableDescriptionContainer&);

    virtual void
    writeInterfaceSpecificVariables(const VariableDescriptionContainer&);

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

    virtual bool
    requiresCheckBoundsFunction(void) const = 0;

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
     * \param const VariableDescriptionContainer&, inputs
     * \param const std::vector<VariableBoundsDescription>&, bounds of the law
     * \param const std::vector<VariableBoundsDescription>&, physical bounds of the law
     */
    void writeHeaderFile(const std::string&,
			 const std::string&,
			 const std::string&,
			 const std::string&,
			 const std::string&,
			 const VariableDescriptionContainer&,
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
     * \param const VariableDescriptionContainer&, inputs
     * \param const std::vector<std::string>&, material laws
     * \param const StaticVariableDescriptionContainer&, static variables
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
		      const VariableDescriptionContainer&,
		      const std::vector<std::string>&,
		      const StaticVariableDescriptionContainer&,
		      const std::vector<std::string>&,
		      const std::map<std::string,double>&,
		      const LawFunction&,
		      const std::vector<VariableBoundsDescription>&,
		      const std::vector<VariableBoundsDescription>&);

    std::ofstream headerFile;

    std::ofstream srcFile;

    std::string headerFileName;

    std::string srcFileName;
    
  }; // end of MfrontCMaterialPropertyInterfaceBase

} // end of namespace mfront

#endif /* LIB_MFRONTCLAWINTERFACEBASE_H_ */

