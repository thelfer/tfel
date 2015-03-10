/*!
 * \file   mfront/include/MFront/AbstractModelInterface.hxx
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

#ifndef LIB_MFRONTMODELVIRTUALINTERFACE_HXX_
#define LIB_MFRONTMODELVIRTUALINTERFACE_HXX_ 

#include<map>
#include<set>
#include<string>
#include<vector>

#include"MFront/MFrontConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/ModelData.hxx"
#include"MFront/FileDescription.hxx"

namespace mfront{
  
  struct MFRONT_VISIBILITY_EXPORT AbstractModelInterface
  {

    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) = 0;

    virtual void
    reset(void) = 0;

    virtual
    void declareReservedNames(std::set<std::string>&) = 0;

    /*!
     * \param pdata : generic data
     * \param data  : model data
     */
    virtual
    void writeOutputFiles(const FileDescription&,
			  const ModelData&) = 0;

    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const ModelData&) = 0;

    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const ModelData&) = 0;

    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const ModelData&) = 0;

    /*!
     * \param pdata : generic data
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const ModelData&) = 0;

    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const ModelData&) = 0;
    /*!
     * \return a map associating to each library a list of entry
     * points (function or classes)
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedEntryPoints(const ModelData&) const = 0;
    //! desctructor
    virtual ~AbstractModelInterface();

  }; // end of AbstractModelInterface

} // end of namespace mfront  

#endif /* LIB_MFRONTMODELVIRTUALINTERFACE_HXX_ */
