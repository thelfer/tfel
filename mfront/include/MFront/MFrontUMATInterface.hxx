/*!
 * \file   MFrontUMATInterface.hxx
 * \brief  This file declares the MFrontUMATInterface class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#ifndef _LIB_MFRONTUMATINTERFACE_H_
#define _LIB_MFRONTUMATINTERFACE_H_ 

#include<string>
#include<fstream>

#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"MFront/SupportedTypes.hxx"
#include"MFront/VarHandler.hxx"
#include"MFront/ComputedVarHandler.hxx"
#include"MFront/BehaviourCharacteristic.hxx"
#include"MFront/MFrontBehaviourVirtualInterface.hxx"

namespace mfront{

  struct MFrontUMATInterface
    : public SupportedTypes,
      public MFrontBehaviourVirtualInterface
  {
    static std::string 
    getName(void);
    
    MFrontUMATInterface();
    
    void setVerboseMode(void);

    void setDebugMode(void);

    void setWarningMode(void);

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

    std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);
    
    void exportMechanicalData(std::ofstream&,
			      const std::string&,
			      const VarContainer&,
			      const VarContainer&,
			      const VarContainer&,
			      const BehaviourCharacteristic);
    
    void writeBehaviourConstructor(std::ofstream&,
				   const std::string&,
				   const VarContainer&,
				   const VarContainer&,
				   const BehaviourCharacteristic,
				   const std::string&,
				   const std::string&);
    
    void writeBehaviourDataConstructor(std::ofstream&,
				       const std::string&,
				       const VarContainer&,
				       const VarContainer&,
				       const VarContainer&,
				       const BehaviourCharacteristic);
    
    void writeIntegrationDataConstructor(std::ofstream&,
					 const std::string&,
					 const VarContainer&,
					 const VarContainer&,
					 const VarContainer&,
					 const BehaviourCharacteristic);

    void
    endTreatement(const std::string&,
		  const std::string&,
		  const std::string&,
		  const std::string&,
		  const std::string&,
		  const VarContainer&,
		  const VarContainer&,
		  const VarContainer&,
		  const std::map<std::string,std::string>&,
		  const std::map<std::string,std::string>&,
		  const BehaviourCharacteristic);

    void
    reset(void);

    ~MFrontUMATInterface();
    
  private:

    static std::string
    getLibraryName(const std::string&,
		   const std::string&);

    static std::string
    makeUpperCase(const std::string& s);

    static std::string
    makeLowerCase(const std::string& s);

    static std::string
    treatScalar(const std::string& s);

    static std::string
    treatStensor(const std::string& s,const unsigned short);

    static std::string
    treatCoefScalar(const std::string& s);

    static std::string
    treatCoefStensor(const std::string& s,const unsigned short);

    static std::string
    treatStateVarScalar(const std::string& s);

    static std::string
    treatStateVarStensor(const std::string& s,const unsigned short);

    static std::string 
    toString(const unsigned short);
    
    bool verboseMode;

    bool debugMode;

    bool warningMode;

    bool useTimeSubStepping;
    
    unsigned short maximumSubStepping;

  }; // end of MFrontUMATInterface

} // end of namespace mfront

#endif /* _LIB_MFRONTUMATINTERFACE_H */
