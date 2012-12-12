/*!
 * \file   MFrontBehaviourVirtualInterface.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 jan 2007
 */

#ifndef _LIB_MFRONTBEHAVIOURVIRTUALINTERFACE_HXX_
#define _LIB_MFRONTBEHAVIOURVIRTUALINTERFACE_HXX_ 

#include<utility>
#include<string>
#include<map>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/VarHandler.hxx"
#include"MFront/ComputedVarHandler.hxx"
#include"MFront/BehaviourCharacteristic.hxx"

namespace mfront{
  
  struct TFEL_VISIBILITY_EXPORT MFrontBehaviourVirtualInterface
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
    exportMechanicalData(std::ofstream&,
			 const std::string&,
			 const VarContainer&,
			 const VarContainer&,
			 const VarContainer&,
			 const VarContainer&,
			 const BehaviourCharacteristic) = 0;
    
    virtual void 
    writeBehaviourConstructor(std::ofstream&,
			      const std::string&,
			      const VarContainer&,
			      const VarContainer&,
			      const VarContainer&,
			      const VarContainer&,
			      const BehaviourCharacteristic,
			      const std::string&,
			      const std::string&) = 0;
    
    virtual void
    writeBehaviourDataConstructor(std::ofstream&,
				  const std::string&,
				  const VarContainer&,
				  const VarContainer&,
				  const VarContainer&,
				  const VarContainer&,
				  const BehaviourCharacteristic) = 0;
    
    virtual void 
    writeIntegrationDataConstructor(std::ofstream&,
				    const std::string&,
				    const VarContainer&,
				    const VarContainer&,
				    const VarContainer&,
				    const VarContainer&,
				    const BehaviourCharacteristic) = 0;

    virtual void
    endTreatement(const std::string&,
		  const std::string&,
		  const std::string&,
		  const std::string&,
		  const std::string&,
		  const std::string&,
		  const VarContainer&,
		  const VarContainer&,
		  const VarContainer&,
		  const VarContainer&,
		  const VarContainer&,
		  const std::map<std::string,std::string>&,
		  const std::map<std::string,std::string>&,
		  const BehaviourCharacteristic) = 0;

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

    virtual ~MFrontBehaviourVirtualInterface();

  }; // end of MFrontBehaviourVirtualInterface

} // end of namespace mfront  

#endif /* _LIB_MFRONTBEHAVIOURVIRTUALINTERFACE_HXX */
