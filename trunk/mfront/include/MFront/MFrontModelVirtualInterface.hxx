/*!
 * \file   MFrontModelVirtualInterface.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 jan 2007
 */

#ifndef _LIB_MFRONTMODELVIRTUALINTERFACE_HXX_
#define _LIB_MFRONTMODELVIRTUALINTERFACE_HXX_ 

#include<map>
#include<set>
#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/MFrontModelData.hxx"
#include"MFront/MFrontGenericData.hxx"

namespace mfront{
  
  struct TFEL_VISIBILITY_EXPORT MFrontModelVirtualInterface
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

    virtual void
    reset(void) = 0;

    virtual
    void declareReservedNames(std::set<std::string>&) = 0;

    /*!
     * \param pdata : generic data
     * \param data  : model data
     */
    virtual
    void writeOutputFiles(const MFrontGenericData&,
			  const MFrontModelData&) = 0;

    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const MFrontGenericData&) = 0;

    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const MFrontGenericData&) = 0;

    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const MFrontGenericData&) = 0;

    /*!
     * \param pdata : generic data
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const MFrontGenericData&) = 0;

    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const MFrontGenericData&) = 0;

    virtual ~MFrontModelVirtualInterface();

  }; // end of MFrontModelVirtualInterface

} // end of namespace mfront  

#endif /* _LIB_MFRONTMODELVIRTUALINTERFACE_HXX */
