/*! 
 * \file  MTestFileGeneratorBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 09 juil. 2013
 */

#ifndef _LIB_MFRONT_MTESTFILEGENERATORBASE_H_
#define _LIB_MFRONT_MTESTFILEGENERATORBASE_H_ 

#include<map>
#include<set>
#include<vector>
#include<string>
#include<ostream>

#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/MFrontConfig.hxx"
#include"MFront/SupportedTypes.hxx"

namespace mfront
{

  /*!
   * \brief a base class for mtest files generation
   * \see UMATSmallStrainMTestFileGenerator
   */
  struct MTESTFILEGENERATOR_VISIBILITY_EXPORT MTestFileGeneratorBase
  {
    //! a simple alias
    typedef double real;
    /*!
     * \return an uniq identifier
     */
    static unsigned int
    getIdentifier(void);
    /*!
     * Constructor
     */
    MTestFileGeneratorBase();
    /*!
     * \set the modelling hypothesis
     * \param[in] h : modelling hypothesis
     */
    virtual void
    setModellingHypothesis(tfel::material::ModellingHypothesis::Hypothesis);
    /*!
     * \brief add a computed time
     * \param[in] t : time
     */
    virtual void
    addTime(const real);
    /*!
     * add a material property
     * \param[in] n : material property name
     * \param[in] v : value
     */
    virtual void
    addMaterialProperty(const std::string&,
			const real);
    /*!
     * add internal state variable
     * \param[in] n : internal state variable name
     * \param[in] f : variable type
     * \param[in] v : value
     */
    virtual void
    addInternalStateVariable(const std::string&,
			     const SupportedTypes::TypeFlag,
			     const real * const);
    /*!
     * add the value of an external state variable at the given time
     * \param[in] n : external state variable name
     * \param[in] t : time
     * \param[in] v : value
     */
    virtual void
    addExternalStateVariableValue(const std::string&,
				  const real,
				  const real);
    
    virtual void
    generate(const std::string&) const;
    /*!
     * return the size of an tiny vector
     */
    virtual unsigned short
    getTVectorSize() const;
    /*!
     * return the size of an stensor
     */
    virtual unsigned short
    getStensorSize() const;
    /*!
     * return the size of an tensor
     */
    virtual unsigned short
    getTensorSize() const;
    /*!
     * destructor
     */
    virtual ~MTestFileGeneratorBase();
  protected:
    /*!
     * return the names of the components of the strain tensor
     */
    virtual std::vector<std::string>
    getStrainComponentsNames() const;
    /*!
     * return the names of the components of the stress tensor
     */
    virtual std::vector<std::string>
    getStressComponentsNames() const;
    /*!
     * write modelling hypothesis
     * \param[in] os : output stream
     */
    virtual void
      writeModellingHypothesis(std::ostream&) const;
    /*!
     * write behaviour declaration
     * \param[in] os : output stream
     */
    virtual void
    writeBehaviourDeclaration(std::ostream&) const = 0;
    /*!
     * write material properties
     * \param[in] os : output stream
     */
    virtual void
    writeMaterialProperties(std::ostream&) const;
    /*!
     * write material properties
     * \param[in] os : output stream
     */
    virtual void
    writeInternalStateVariables(std::ostream&) const;
    /*!
     * write external state variable
     * \param[in] os : output stream
     */
    virtual void
    writeExternalStateVariables(std::ostream&) const;
    /*!
     * write times
     * \param[in] os : output stream
     */
    virtual void
    writeTimes(std::ostream&) const;
    /*!
     * write driving variables
     * \param[in] os : output stream
     */
    virtual void
    writeDrivingVariables(std::ostream&) const = 0;
    /*!
     * structure representing an internal state variable
     */
    struct InternalStateVariable
    {
      //! name
      std::string name;
      //! type of the variable
      SupportedTypes::TypeFlag type;
      //! initial value
      real values[9];
    }; // end of struct InternalStateVariable
    //! modelling hypothesis
    tfel::material::ModellingHypothesis::Hypothesis hypothesis;
    //! real time step
    std::set<real> times;
    //! internal state variable
    std::vector<InternalStateVariable> ivs;
    //! values of the material properties
    std::map<std::string,real> mps;
    //! values of the external state variables
    std::map<std::string,
	     std::map<real,real> > evs;
  };

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTFILEGENERATORBASE_H */

