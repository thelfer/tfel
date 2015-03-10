/*!
 * \file   mfront/include/MFront/AbstractMaterialPropertyInterface.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTLAWVIRTUALINTERFACE_HXX_
#define LIB_MFRONTLAWVIRTUALINTERFACE_HXX_ 

#include<utility>
#include<string>
#include<map>
#include<vector>

#include"MFront/MFrontConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/LawFunction.hxx"
#include"MFront/StaticVariableDescription.hxx"
#include"MFront/VariableBoundsDescription.hxx"

namespace mfront{
  
  struct MFRONT_VISIBILITY_EXPORT AbstractMaterialPropertyInterface
  {

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
     * \param const VariableDescriptionContainer&, inputs
     * \param const std::vector<std::string>&, material laws
     * \param const std::map<std::string,std::string>&, glossary names
     * \param const std::map<std::string,std::string>&, entry names
     * \param const StaticVariableDescriptionContainer&, static variables
     * \param[in] parameters
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
				  const std::vector<std::string>&) = 0;

    virtual void
    reset(void) = 0;
    /*!
     * \param[in] library   : libary name
     * \param[in] material  : material name
     * \param[in] className : className
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const std::string&,
		      const std::string&,
		      const std::string&) = 0;
    /*!
     * \return a map associating to each library a list of entry
     * points (function or classes)
     * \param[in] library   : libary name
     * \param[in] material  : material name
     * \param[in] className : className
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedEntryPoints(const std::string&,
			    const std::string&,
			    const std::string&) = 0;
    /*!
     * \param[in] library   : libary name
     * \param[in] material  : material name
     * \param[in] className : className
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const std::string&,
			  const std::string&,
			  const std::string&) = 0;

    /*!
     * \param[in] library   : libary name
     * \param[in] material  : material name
     * \param[in] className : className
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const std::string&,
			const std::string&,
			const std::string&) = 0;

    /*!
     * \param[in] library   : libary name
     * \param[in] material  : material name
     * \param[in] className : className
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const std::string&,
			 const std::string&,
			 const std::string&) = 0;

    /*!
     * \param[in] library   : libary name
     * \param[in] material  : material name
     * \param[in] className : className
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const std::string&,
			     const std::string&,
			     const std::string&) = 0;

    /*!
     * \param[in] library   : libary name
     * \param[in] material  : material name
     * \param[in] className : className
     * \param const std::vector<std::string>&, library links
     */
    virtual std::map<std::string,
		     std::pair<std::vector<std::string>,
			       std::vector<std::string> > >
    getSpecificTargets(const std::string&,
		       const std::string&,
		       const std::string&,
		       const std::vector<std::string>&) = 0;

    virtual ~AbstractMaterialPropertyInterface();

  }; // end of AbstractMaterialPropertyInterface

} // end of namespace mfront  

#endif /* LIB_MFRONTLAWVIRTUALINTERFACE_HXX_ */
