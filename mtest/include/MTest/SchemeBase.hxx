/*!
 * \file   mtest/include/MTest/SchemeBase.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   02 oct. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MTEST_SCHEMEBASE_HXX_
#define _LIB_MTEST_SCHEMEBASE_HXX_

#include<vector>
#include<string>
#include<memory>
#include<fstream>

#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Material/MechanicalBehaviour.hxx"

#include"MTest/Types.hxx"
#include"MTest/Config.hxx"
#include"MTest/Study.hxx"
#include"MTest/Scheme.hxx"
#include"MTest/SolverOptions.hxx"

namespace mtest{

  // forward declaration
  struct AccelerationAlgorithm;
  
  struct MTEST_VISIBILITY_EXPORT SchemeBase
    : public Scheme,
      public Study
  {
    using EvolutionPtr = std::shared_ptr<Evolution>;
    SchemeBase();
    SchemeBase(SchemeBase&&) = delete;
    SchemeBase(const SchemeBase&) = delete;
    SchemeBase& operator=(SchemeBase&&) = delete;
    SchemeBase& operator=(const SchemeBase&) = delete;
    /*!
     * \param[in] h : modelling hypothesis
     */
    virtual void
    setModellingHypothesis(const std::string&);
    /*!
     * \param[in] t : times
     */
    virtual void
    setTimes(const std::vector<real>&);
    /*!
     * \brief set the output file
     * \param[in] f : file name
     */
    virtual void
    setOutputFileName(const std::string&);
    /*!
     * \brief set the output file precision
     * \param[in] p : precision
     */
    virtual void
    setOutputFilePrecision(const unsigned int);
    /*!
     * \brief set the residual file
     * \param[in] f : file name
     */
    virtual void
    setResidualFileName(const std::string&);
    /*!
     * \brief set the residual file precision
     * \param[in] p : precision
     */
    virtual void
    setResidualFilePrecision(const unsigned int);
    /*!
     * \brief print usefull information in the output file 
     * \param[in] t  : time
     * \param[in] state  : current state
     */
    virtual void printOutput(const real,const StudyCurrentState&) = 0;
    /*!
     * \return the list of evolutions
     */
    std::shared_ptr<EvolutionManager>
    getEvolutions() const;
    /*!
     * \brief add a new evolution
     * \param[in] n  : evolution name
     * \param[in] p  : evolution pointer
     * \param[in] b1 : declare the variable.
     * \param[in] b2 : check if the evolution already exists.
     * \note if b1 is false, we check that the variable is already
     * declared. Otherwise, an exeception is thrown.
     * \note if b2 is true and the evolution already exists, an
     * exeception is thrown.
     */
    virtual void
    addEvolution(const std::string&,
		 const EvolutionPtr,
		 const bool,
		 const bool);
    /*!
     * \brief set evolution value for a given time
     * \param[in] n  : evolution name
     * \param[in] t  : time
     * \param[in] v  : value
     *
     * \note the evolution *must* be of type LPIEvolution
     */
    virtual void
    setEvolutionValue(const std::string&,
		      const real,
		      const real);
    /*!
     * \brief set the description
     * \param[in] d : description
     */
    virtual void setDescription(const std::string&);
    /*!
     * \brief set the author
     * \param[in] a : author
     */
    virtual void setAuthor(const std::string&);
    /*!
     * \brief set the date
     * \param[in] d : date
     */
    virtual void setDate(const std::string&);
    virtual void setMaximumNumberOfIterations(const unsigned int);
    virtual void setMaximumNumberOfSubSteps(const unsigned int);
    //! \return the dimension
    virtual unsigned short
    getDimension(void) const;
    //! \return the modelling hypothesis
    virtual tfel::material::ModellingHypothesis::Hypothesis
    getModellingHypothesis(void) const;
    /*!
     * \brief set the stiffness updating policy
     * \param[in] b : boolean
     */
    virtual void
    setStiffnessUpdatingPolicy(const StiffnessUpdatingPolicy);
    /*!
     * \brief set the prediction policy
     * \param[in] p : prediction policy
     */
    virtual void setPredictionPolicy(const PredictionPolicy);
    /*!
     * \brief set the stiffness matrix type
     * \param[in] k : stiffness matrix type
     */
    virtual void setStiffnessMatrixType(const StiffnessMatrixType);
    /*!
     * \brief set the use of the castem acceleration algorithm
     * \param[in] b : boolean
     */
    virtual void setUseCastemAccelerationAlgorithm(const bool);
    /*!
     * \brief set the acceleration algorithm to be used
     * \param[in] a : acceleration algorithm
     */
    virtual void setAccelerationAlgorithm(const std::string&);
    /*!
     * \brief set a parameter of the acceleration algorithm
     * \param[in] p : parameter name
     * \param[in] v : parameter value
     */
    virtual void setAccelerationAlgorithmParameter(const std::string&,
						   const std::string&);
    /*!
     * \brief set at which iteration the use of the castem
     * acceleration algorithm  will begin
     * \param[in] i : iteration number
     */
    virtual void setCastemAccelerationTrigger(const int);
    /*!
     * \brief set at which period the use of the castem
     * acceleration algorithm  will take place
     * \param[in] p : period
     */
    virtual void setCastemAccelerationPeriod(const int);
    //! destructor
    virtual ~SchemeBase();
  protected:
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    /*!
     * complete the initialisation. This method must be called once.
     * \note this method must be called by the derived class.
     */ 
    virtual void completeInitialisation(void);
    //! \brief set the modelling hypothesis to the default one
    virtual void setDefaultModellingHypothesis(void) = 0;
    //! \return the default stiffness matrix type
    virtual StiffnessMatrixType
    getDefaultStiffnessMatrixType(void) const = 0;
    /*!
     * \brief declare a new variable
     * \param[in] v : variable name
     */
    void declareVariable(const std::string&,
			 const bool);
    /*!
     * \brief declare a list of new variables
     * \param[in] v : variable names
     */
    void declareVariables(const std::vector<std::string>&,
			  const bool);
    //! declared variable names
    std::vector<std::string> vnames;
    //! initilisation stage
    bool initialisationFinished = false;
    //! solver options
    SolverOptions options;
    //! times
    std::vector<real> times;
    //! description of the test
    std::string description;
    //! author
    std::string author;
    //! date
    std::string date;
    //! modelling hypothesis
    ModellingHypothesis::Hypothesis hypothesis = 
      ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    //! list of evolutions
    std::shared_ptr<EvolutionManager> evm;
    //! output file name
    std::string output;
    //! output file
    std::ofstream out;
    //! residual file name
    std::string residualFileName;
    //! file where residuals evolutions as a function of the iteration
    //! number are saved
    mutable std::ofstream residual;
    //! output file precision
    int oprec = -1;
    //! residual file precision
    int rprec = -1;
  }; // end of struct SchemeBase
  
} // end of namespace mtest

#endif /* _LIB_MTEST_SCHEMEBASE_HXX_ */
