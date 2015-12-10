/*!
 * \file   SingleStructureScheme.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   09 déc. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MTEST_SINGLESTRUCTURESCHEME_HXX_
#define _LIB_MTEST_SINGLESTRUCTURESCHEME_HXX_

#include"TFEL/Material/OutOfBoundsPolicy.hxx"
#include"MTest/Config.hxx"
#include"MTest/Types.hxx"
#include"MTest/SchemeBase.hxx"

namespace mtest{

  // forward declaration
  struct Behaviour;

  /*!
   * Base class for schemes describing a single structure
   */
  struct MTEST_VISIBILITY_EXPORT SingleStructureScheme
    : public SchemeBase
  {
    //! default constructor
    SingleStructureScheme();
    /*!
     * \brief set the behaviour
     * \param[in] i : interface
     * \param[in] l : library name
     * \param[in] f : function
     */
    virtual void
    setBehaviour(const std::string&,
		 const std::string&,
		 const std::string&);
    /*!
     * \brief define a material property
     * \param[in] n     : evolution name
     * \param[in] p     : evolution pointer
     * \param[in] check : check if the evolution already exists.
     * \note if check is true and the evolution already exists, an
     * exeception is thrown.
     */
    virtual void
    setMaterialProperty(const std::string&,
			const EvolutionPtr,
			const bool);
    /*!
     * \brief define an external state variable
     * \param[in] n     : evolution name
     * \param[in] p     : evolution pointer
     * \param[in] check : check if the evolution already exists.
     * \note if check is true and the evolution already exists, an
     * exeception is thrown.
     */
    virtual void
    setExternalStateVariable(const std::string&,
			     const EvolutionPtr,
			     const bool);
    //! \return the behaviour type
    virtual tfel::material::MechanicalBehaviourBase::BehaviourType
    getBehaviourType(void) const;
    //! \return the behaviour
    virtual std::shared_ptr<Behaviour>
    getBehaviour(void);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter value
     */
    virtual void
    setParameter(const std::string&,
		 const double);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter value
     */
    virtual void
    setIntegerParameter(const std::string&,
			const int);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter value
     */
    virtual void
    setUnsignedIntegerParameter(const std::string&,
				const unsigned int);
    /*!
     * \brief set the out of bounds policy
     * \param[in] a : out of bounds policy
     */
    virtual void setOutOfBoundsPolicy(const tfel::material::OutOfBoundsPolicy);
    //! destructor
    virtual ~SingleStructureScheme();
  protected:
    /*!
     * complete the initialisation. This method must be called once.
     * \note this method must be called by the derived class.
     */ 
    virtual void completeInitialisation(void) override;
    //! \return the default stiffness matrix type
    virtual StiffnessMatrixType
    getDefaultStiffnessMatrixType(void) const override;
    //! list of internal variable names, including their suffixes
    std::vector<std::string> ivfullnames;
    //! the mechanical behaviour
    std::shared_ptr<Behaviour> b;
    //! default values for material properties as given by the behaviour
    std::shared_ptr<EvolutionManager> dmpv;
  }; // end of SingleStructureScheme
  
} // end of namespace mtest

#endif /* _LIB_MTEST_SINGLESTRUCTURESCHEME_HXX_ */
