/*! 
 * \file  MFrontZMATInterface.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 23 mai 2014
 */

#ifndef _LIB_MFRONT_MFRONTZMATINTERFACE_H_
#define _LIB_MFRONT_MFRONTZMATINTERFACE_H_ 

#include<set>
#include<string>

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/SupportedTypes.hxx"
#include"MFront/MechanicalBehaviourDescription.hxx"
#include"MFront/MFrontBehaviourVirtualInterface.hxx"

namespace mfront{

  /*!
   * This class provides some helper function for behaviours
   * interfaces based on the umat standard
   */
  struct MFrontZMATInterface
    : public SupportedTypes,
      public MFrontBehaviourVirtualInterface
  {
    //! a simple alias
    typedef tfel::material::ModellingHypothesis ModellingHypothesis;
    //! a simple alias
    typedef ModellingHypothesis::Hypothesis Hypothesis;

    /*!
     * \return the name of the interface
     */
    static std::string
    getName(void);

    MFrontZMATInterface();

    /*!
     * set if dynamically allocated arrays are allowed
     * \param[in] b : boolean
     */
    virtual void
    allowDynamicallyAllocatedArrays(const bool);

    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);
    /*!
     * \return true if the interface handles the given modelling hypothesis
     * \param[in] h  : modelling hypothesis
     * \param[in] mb : mechanical behaviour description
     */
    virtual bool
    isModellingHypothesisHandled(const Hypothesis,
				 const MechanicalBehaviourDescription&) const;
    
    virtual std::set<Hypothesis>
    getModellingHypothesesToBeTreated(const MechanicalBehaviourDescription&) const;
    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     */
    virtual void 
    writeInterfaceSpecificIncludes(std::ofstream&,
				   const MechanicalBehaviourDescription&) const;

    virtual void
    exportMechanicalData(std::ofstream&,
			 const Hypothesis,
			 const MechanicalBehaviourDescription&) const;
    /*!
     * write the behaviour constructor associated with the law
     * \param[in] behaviourFile           : output file
     * \param[in] mb                      : mechanical behaviour description
     * \param[in] initStateVarsIncrements : constructor part assigning
     *                                      default value (zero) to state
     *                                      variable increments
     */
    virtual void
    writeBehaviourConstructor(std::ofstream&,
			      const MechanicalBehaviourDescription&,
			      const std::string&) const;

    virtual void
    writeBehaviourDataConstructor(std::ofstream&,
				  const Hypothesis,
				  const MechanicalBehaviourDescription&) const;

    /*!
     * write the initialisation of the driving variables
     * \param[in] behaviourFile : output file
     * \param[in] mb            : mechanical behaviour description
     */
    virtual void 
    writeBehaviourDataMainVariablesSetters(std::ofstream&,
					   const MechanicalBehaviourDescription&) const;
    
    virtual void
    writeIntegrationDataConstructor(std::ofstream&,
				    const Hypothesis,
				    const MechanicalBehaviourDescription&) const;
    /*!
     * write the initialisation of the driving variables
     * \param[in] behaviourFile : output file
     * \param[in] mb            : mechanical behaviour description
     */
    virtual void 
    writeIntegrationDataMainVariablesSetters(std::ofstream&,
					     const MechanicalBehaviourDescription&) const;
    /*!
     * \brief write output files
     * \param[in] mb        : mechanical behaviour description
     * \param[in] fd        : mfront file description
     */
    virtual void
    endTreatement(const MechanicalBehaviourDescription&,
		  const MFrontFileDescription&) const;
    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const MechanicalBehaviourDescription&) const;

    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const MechanicalBehaviourDescription&) const;

    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const MechanicalBehaviourDescription&) const;

    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const MechanicalBehaviourDescription&) const;

    virtual void
    reset(void);
    
    virtual ~MFrontZMATInterface();

  protected:

    /*!
     * \brief write behaviour initialisation for the given hypothesis
     * \param[out] out : output file
     * \param[in]  mb  : mechancial behaviour description
     * \param[in]  h   : modelling hypothesis
     */
    void
    writeBehaviourInitialisation(std::ostream&,
				 const MechanicalBehaviourDescription&,
				 const MFrontZMATInterface::Hypothesis) const;
    /*!
     * \brief write material properties initialisation for the given hypothesis
     * \param[out] out : output file
     * \param[in]  mb  : mechancial behaviour description
     * \param[in]  h   : modelling hypothesis
     */
    void
    writeMaterialPropertiesInitialisation(std::ostream&,
					  const MechanicalBehaviourDescription&,
					  const MFrontZMATInterface::Hypothesis) const;
    /*!
     * \brief write behaviour initialisation for the given hypothesis
     * \param[out] out : output file
     * \param[in]  mb  : mechancial behaviour description
     * \param[in]  h   : modelling hypothesis
     */
    void
    writeCallMFrontBehaviour(std::ostream&,
			     const MechanicalBehaviourDescription&,
			     const MFrontZMATInterface::Hypothesis) const;

    
  }; //end of struct MFrontZMATInterface

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTZMATINTERFACE_H */

