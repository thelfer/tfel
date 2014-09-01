/*!
 * \file   mfront/include/MFront/MFrontBehaviourVirtualInterface.hxx
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

#ifndef _LIB_MFRONTBEHAVIOURVIRTUALINTERFACE_HXX_
#define _LIB_MFRONTBEHAVIOURVIRTUALINTERFACE_HXX_ 

#include<map>
#include<set>
#include<utility>
#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

namespace mfront{
  
  // forward declaration
  struct MechanicalBehaviourDescription;

  // forward declaration
  struct MFrontFileDescription;

  /*!
   * This is the abstract base class of all behaviour interfaces
   */
  struct TFEL_VISIBILITY_EXPORT MFrontBehaviourVirtualInterface
  {

    //! a simple alias
    typedef tfel::material::ModellingHypothesis ModellingHypothesis;
    //! a simple alias
    typedef ModellingHypothesis::Hypothesis Hypothesis;

    /*!
     * set if dynamically allocated arrays are allowed
     * \param[in] b : boolean
     */
    virtual void
    allowDynamicallyAllocatedArrays(const bool) = 0;

    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) = 0;
    /*!
     * \return true if the interface handles the given modelling hypothesis
     * \param[in] h  : modelling hypothesis
     * \param[in] mb : mechanical behaviour description
     */
    virtual bool
    isModellingHypothesisHandled(const Hypothesis,
				 const MechanicalBehaviourDescription&) const = 0;
    
    virtual std::set<Hypothesis>
    getModellingHypothesesToBeTreated(const MechanicalBehaviourDescription&) const = 0;

    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     */
    virtual void 
    writeInterfaceSpecificIncludes(std::ofstream&,
				   const MechanicalBehaviourDescription&) const = 0;

    virtual void 
    exportMechanicalData(std::ofstream&,
			 const Hypothesis,
			 const MechanicalBehaviourDescription&) const = 0;
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
			      const std::string&) const = 0;
    
    virtual void
    writeBehaviourDataConstructor(std::ofstream&,
				  const Hypothesis,
				  const MechanicalBehaviourDescription&) const = 0;
    /*!
     * write the behaviour constructor associated with the law
     * \param[in] behaviourFile : output file
     * \param[in] mb            : mechanical behaviour description
     */
    virtual void 
    writeBehaviourDataMainVariablesSetters(std::ofstream&,
					   const MechanicalBehaviourDescription&) const = 0;
    
    virtual void 
    writeIntegrationDataConstructor(std::ofstream&,
				    const Hypothesis,
				    const MechanicalBehaviourDescription&) const = 0;
    /*!
     * write the behaviour constructor associated with the law
     * \param[in] behaviourFile : output file
     * \param[in] mb            : mechanical behaviour description
     */
    virtual void 
    writeIntegrationDataMainVariablesSetters(std::ofstream&,
					     const MechanicalBehaviourDescription&) const = 0;
    /*!
     * \brief write output files
     * \param[in] mb        : mechanical behaviour description
     * \param[in] fd        : mfront file description
     */
    virtual void
    endTreatement(const MechanicalBehaviourDescription&,
		  const MFrontFileDescription&) const = 0;
    /*!
     * \brief reset the interface
     */
    virtual void
    reset(void) = 0;

    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const MechanicalBehaviourDescription&) const = 0;

    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const MechanicalBehaviourDescription&) const = 0;

    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const MechanicalBehaviourDescription&) const = 0;

    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const MechanicalBehaviourDescription&) const = 0;

    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const MechanicalBehaviourDescription&) const = 0;

    virtual ~MFrontBehaviourVirtualInterface();

  }; // end of MFrontBehaviourVirtualInterface

} // end of namespace mfront  

#endif /* _LIB_MFRONTBEHAVIOURVIRTUALINTERFACE_HXX */
