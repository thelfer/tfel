/*!
 * \file   mfront/include/MFront/MFrontCastemLawInterface.hxx
 * \brief  This file declares the MFrontCastemLawInterface class
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTCASTEMLAWINTERFACE_H_
#define _LIB_MFRONTCASTEMLAWINTERFACE_H_ 

#include<string>
#include<fstream>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/LawFunction.hxx"
#include"MFront/VariableBoundsDescription.hxx"
#include"MFront/MFrontLawVirtualInterface.hxx"

namespace mfront{

  struct TFEL_VISIBILITY_EXPORT MFrontCastemLawInterface
    : public MFrontLawVirtualInterface
  {
    static std::string 
    getName(void);
    
    MFrontCastemLawInterface();
    
    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const std::string&,
		      const std::string&,
		      const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const std::string&,
			  const std::string&,
			  const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const std::string&,
			const std::string&,
			const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const std::string&,
			 const std::string&,
			 const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const std::string&,
			     const std::string&,
			     const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     * \param const std::vector<std::string>&, library links
     */
    virtual std::map<std::string,
		     std::pair<std::vector<std::string>,
			       std::vector<std::string> > >
    getSpecificTargets(const std::string&,
		       const std::string&,
		       const std::string&,
		       const std::vector<std::string>&);

    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);
    
    /*
     * \param const std::string&, name of the original file
     * \param const std::string&, name of the output library
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     * \param const std::string&, author
     * \param const std::string&, date
     * \param const std::string&, description
     * \param const std::string&, includes
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
    virtual void
    writeOutputFiles(const std::string&,
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
    
    virtual void
    reset(void);

    ~MFrontCastemLawInterface();

  protected:
    
    /*!
     * \param[in] material
     * \param[in] className
     */
    virtual std::string
    getCastemFunctionName(const std::string&,
			  const std::string&);
    
    virtual std::string
    getHeaderFileName(const std::string&);

    virtual std::string
    getSourceFileName(const std::string&);

  private:

    /*
     * \param[in] className
     * \param[in] author
     * \param[in] date
     * \param[in] description
     * \param[in] params
     */
    virtual void
    writeHeaderFile(const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::vector<std::string>&);
    
    /*
     * \param const std::string&, name of the original file
     * \param const std::string&, className
     * \param const std::string&, author
     * \param const std::string&, date
     * \param const std::string&, includes
     * \param const std::string&, output name
     * \param const VariableDescriptionContainer&, inputs
     * \param const std::vector<std::string>&, material laws
     * \param const StaticVariableDescriptionContainer&, static variables
     * \param const std::map<std::string,std::string>&, glossary names,
     * \param const std::map<std::string,std::string>&, entry names,
     * \param const std::vector<std::string>&, parameters
     * \param const std::map<std::string,double>&, parameters values
     * \param const LawFunction&, function definition
     * \param const std::vector<VariableBoundsDescription>&, bounds of the law
     * \param const std::vector<VariableBoundsDescription>&, physical bounds of the law
     */
    virtual void
    writeSrcFile(const std::string&,
		 const std::string&,
		 const std::string&,
		 const std::string&,
		 const std::string&,
		 const std::string&,
		 const VariableDescriptionContainer&,
		 const std::vector<std::string>&,
		 const StaticVariableDescriptionContainer&,
		 const std::map<std::string,std::string>&,
		 const std::map<std::string,std::string>&,
		 const std::vector<std::string>&,
		 const std::map<std::string,double>&,
		 const LawFunction&,
		 const std::vector<VariableBoundsDescription>&,
		 const std::vector<VariableBoundsDescription>&);
    
    std::ofstream headerFile;

    std::ofstream srcFile;

    std::string headerFileName;

    std::string srcFileName;
    
  }; // end of MfrontCastemLawInterface

} // end of namespace mfront

#endif /* _LIB_MFRONTCASTEMLAWINTERFACE_H */
