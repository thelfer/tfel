/*!
 * \file   MFrontAsterInterface.hxx
 * \brief  This file declares the MFrontAsterInterface class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#ifndef _LIB_MFRONTASTERINTERFACE_H_
#define _LIB_MFRONTASTERINTERFACE_H_ 

#include<string>
#include<fstream>

#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"MFront/SupportedTypes.hxx"
#include"MFront/VarHandler.hxx"
#include"MFront/ComputedVarHandler.hxx"
#include"MFront/BehaviourCharacteristic.hxx"
#include"MFront/MFrontBehaviourVirtualInterface.hxx"

namespace mfront{

  struct MFrontAsterInterface
    : public SupportedTypes,
      public MFrontBehaviourVirtualInterface
  {
    static std::string 
    getName(void);
    
    MFrontAsterInterface();
    
    virtual void
    setVerboseMode(void);

    virtual void
    setDebugMode(void);

    virtual void
    setWarningMode(void);

    /*!
     * set if dynamically allocated arrays are allowed
     * \param[in] b : boolean
     */
    virtual void
    allowDynamicallyAllocatedArrays(const bool);

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
			      const VarContainer&,
			      const BehaviourCharacteristic);
    
    void writeBehaviourConstructor(std::ofstream&,
				   const std::string&,
				   const VarContainer&,
				   const VarContainer&,
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
				       const VarContainer&,
				       const BehaviourCharacteristic);
    
    void writeIntegrationDataConstructor(std::ofstream&,
					 const std::string&,
					 const VarContainer&,
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
		  const std::string&,
		  const VarContainer&,
		  const VarContainer&,
		  const VarContainer&,
		  const VarContainer&,
		  const VarContainer&,
		  const std::map<std::string,std::string>&,
		  const std::map<std::string,std::string>&,
		  const BehaviourCharacteristic);

    void
    reset(void);

    ~MFrontAsterInterface();
    
  protected:

    static std::string
    getLibraryName(const std::string&,
		   const std::string&);

    std::vector<std::string>
    getGlossaryNames(const VarContainer&,
		     const std::map<std::string,std::string>&,
		     const std::map<std::string,std::string>&);

    void
    appendGlossaryNames(std::vector<std::string>&,
			const VarContainer&,
			const std::map<std::string,std::string>&,
			const std::map<std::string,std::string>&);


    void
    writeGlossaryNames(std::ostream&,
		       const std::vector<std::string>&,
		       const std::string&,
		       const std::string&,
		       const unsigned short = 0u);

    bool   compareToNumericalTangentOperator;

    double strainPerturbationValue;

    double tangentOperatorComparisonCriterium;

    bool verboseMode;

    bool debugMode;

    bool warningMode;

  }; // end of MFrontAsterInterface

} // end of namespace mfront

#endif /* _LIB_MFRONTASTERINTERFACE_H */
