/*!
 * \file   MFrontLawVirtualInterface.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 jan 2007
 */

#ifndef _LIB_MFRONTLAWVIRTUALINTERFACE_HXX_
#define _LIB_MFRONTLAWVIRTUALINTERFACE_HXX_ 

#include<utility>
#include<string>
#include<map>
#include<vector>

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"VarHandler.hxx"
#include"StaticVarHandler.hxx"
#include"LawFunction.hxx"
#include"VariableBoundsDescription.hxx"

namespace mfront{
  
  struct MFrontLawVirtualInterface
  {

    virtual void 
    setVerboseMode() = 0;

    virtual void 
    setDebugMode() = 0;

    virtual void 
    setWarningMode() = 0;

    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) = 0;

    /*
     * \param const std::string&, name of the original file
     * \param const std::string&, name of the output library
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     * \param const std::string&, author
     * \param const std::string&, date
     * \param const std::string&, description
     * \param const std::string&, includes
     * \param const std::string&, inputs
     * \param const std::string&, output name
     * \param const VarContainer&, inputs
     * \param const std::vector<std::string>&, material laws
     * \param const std::map<std::string,std::string>&, glossary names
     * \param const std::map<std::string,std::string>&, entry names
     * \param const StaticVarContainer&, static variables
     * \param const std::vector<std::string>&, parameters
     * \param const std::map<std::string,double>&, parameters values
     * \param const LawFunction&, function definition
     * \param const std::vector<VariableBoundsDescription>&, bounds of the law
     * \param const std::vector<VariableBoundsDescription>&, physical bounds of the law
     * \param const bool, use template
     * \param const std::vector<std::string>&, namespaces
     */
    virtual void writeOutputFiles(const std::string&,
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
				  const std::vector<std::string>&) = 0;

    virtual void
    reset(void) = 0;

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const std::string&,
		      const std::string&,
		      const std::string&) = 0;

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const std::string&,
			  const std::string&,
			  const std::string&) = 0;

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const std::string&,
			const std::string&,
			const std::string&) = 0;

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const std::string&,
			 const std::string&,
			 const std::string&) = 0;

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const std::string&,
			     const std::string&,
			     const std::string&) = 0;

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
		       const std::vector<std::string>&) = 0;

    virtual ~MFrontLawVirtualInterface();

  }; // end of MFrontLawVirtualInterface

} // end of namespace mfront  

#endif /* _LIB_MFRONTLAWVIRTUALINTERFACE_HXX */
