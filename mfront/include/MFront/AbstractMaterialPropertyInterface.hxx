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

#include<map>
#include<string>
#include<utility>
#include<vector>

#include"MFront/MFrontConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

namespace mfront{
  
  // forward declartion
  struct MaterialPropertyDescription;
  // forward declartion
  struct FileDescription;

  struct MFRONT_VISIBILITY_EXPORT AbstractMaterialPropertyInterface
  {

    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) = 0;
    /*!
     * \brief generate the output files
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    virtual void writeOutputFiles(const MaterialPropertyDescription&,
				  const FileDescription&) = 0;
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
