/*!
 * \file   mfront/include/MFront/AbstractBehaviourDSL.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   October 23, 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_ABSTRACTBEHAVIOURDSL_HXX_
#define LIB_MFRONT_ABSTRACTBEHAVIOURDSL_HXX_ 

#include"MFront/MFrontConfig.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/AbstractDSL.hxx"

namespace mfront{

  // forward declaration
  struct BehaviourDescription;

  /*!
   * Interface class for all domain specific languages.
   */
  struct MFRONT_VISIBILITY_EXPORT AbstractBehaviourDSL
    : public virtual AbstractDSL
  {
    //! constructor
    AbstractBehaviourDSL() = default;
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis =  ModellingHypothesis::Hypothesis;
    //! \return the target of the dsl
    virtual DSLTarget getTargetType(void) const override final;
    //! \return the behaviour description
    virtual const BehaviourDescription&
    getBehaviourDescription(void) const = 0;
    /*!
     * \return the list of hypothesis a priori supported by
     * the parser.
     *
     * To enable other hypothesis or restrict the hypotheses
     * supported, the user must use the `@ModellingHypothesis` or
     * `@ModellingHypotheses` keywords.
     */
    virtual std::set<Hypothesis>
    getDefaultModellingHypotheses(void) const = 0;
    /*!
     * \return true if the given modelling hypothesis is handled by
     * the parser
     *
     * Some parsers have restrictions on the modelling hypotheses
     * supported. For example, the isotropic behaviours were not able
     * to handle plane stress hypotheses when this comment was
     * written(but it was planned, so this comment may be outdated
     * now).
     *
     * The fact that this method returns true means that the user
     * *can* provide code to support this modelling hypothesis. For
     * example, to support plane stress in RungeKutta and Implicit
     * parsers, the user must provide some hand-crafted code. He must
     * enable this modelling hypothesis by calling explicitely
     * `@ModellingHypothesis` or `@ModellingHypotheses` keywords.
     */
    virtual bool
    isModellingHypothesisSupported(const Hypothesis) const = 0;
    //! destructor
    virtual ~AbstractBehaviourDSL();
  };

} // end of namespace mfront

#endif /* LIB_MFRONT_ABSTRACTBEHAVIOURDSL_HXX_ */

