/*! 
 * \file  BehaviourBrickBase.hxx
 * \brief
 * \author Helfer Thomas
 * \date   21 oct. 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_BEHAVIOURBRICKBASE_H_
#define _LIB_MFRONT_BEHAVIOURBRICKBASE_H_ 

#include"MFront/BehaviourBrick.hxx"

namespace mfront{


  //! forward declaration
  struct VariableDescriptionContainer;
  //! forward declaration
  struct AbstractBehaviourDSL;
  //! forward declaration
  struct BehaviourData;
  //! forward declaration
  struct BehaviourDescription;

  /*!
   * an helper class providing a set of helper method to build BehaviourBricks.
   */
  struct BehaviourBrickBase
    : public BehaviourBrick
  {
    /*!
     * constructor
     * \param[in] dsl_ : domain specific language
     * \param[in] bd_  : behaviour description
     */
    BehaviourBrickBase(AbstractBehaviourDSL&,
		       BehaviourDescription&);
    //! destructor
    virtual ~BehaviourBrickBase();
  protected:
    /*!
     * \brief throw an exception if the given parameter has a value
     * \param[in] p : parameter
     */
    virtual void
    checkThatParameterHasNoValue(const Parameter&) const;
    /*!
     * \brief add a new material property
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] g: glossary name
     */
    virtual void
    addMaterialPropertyIfNotDefined(const std::string&,
				    const std::string&,
				    const std::string&) const;
    /*!
     * \brief add a new material property
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     */
    virtual void
    addLocalVariable(const std::string&,
		     const std::string&) const;
    //! calling domain specific language
    AbstractBehaviourDSL& dsl;
    //! mechanical behaviour description of which the BehaviourBrick acts
    BehaviourDescription& bd;
  }; // end of struct BehaviourBrickBase

}

#endif /* _LIB_MFRONT_BEHAVIOURBRICKBASE_H */

