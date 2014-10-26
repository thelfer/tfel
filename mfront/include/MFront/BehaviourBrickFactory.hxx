/*! 
 * \file   mfront/include/MFront/BehaviourBrickFactory.hxx
 * \brief
 * \author Helfer Thomas
 * \date   October 20, 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MFRONTBEHAVIOURBRICKFACTORY_H_
#define _LIB_MFRONT_MFRONTBEHAVIOURBRICKFACTORY_H_ 

#include<map>
#include<string>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"MFront/BehaviourBrick.hxx"

namespace mfront
{
  
  // forward declaration
  struct AbstractBehaviourDSL;
  // forward declaration
  struct BehaviourDescription;

  /*!
   * register the non linear BehaviourBrick used by the implicit parser
   */
  struct BehaviourBrickFactory
  {
    //! a simple alias
    typedef tfel::utilities::shared_ptr<BehaviourBrick> (* constructor)(AbstractBehaviourDSL&,
									BehaviourDescription&,
									const BehaviourBrick::Parameters&);
    /*!
     * \return the uniq instance of the BehaviourBrick factory
     */
    static BehaviourBrickFactory&
    getFactory();
    /*!
     * \return the requested BehaviourBrick
     * \param[in]     a   : BehaviourBrick name
     * \param[in,out] dsl : calling domain specific language
     * \param[in,out] mb  : mechanical behaviour description to be
     * treated
     * \param[out]    p   : parameters
     */
    tfel::utilities::shared_ptr<BehaviourBrick>
    get(const std::string&,
	AbstractBehaviourDSL&,
	BehaviourDescription&,
	const BehaviourBrick::Parameters&) const;
    /*!
     * \param[in] a : BehaviourBrick name 
     * \param[in] c : BehaviourBrick constructor 
     */
    void
    registerBehaviourBrick(const std::string&,
		  const constructor);
  private:
    /*!
     * default constructor
     */
    BehaviourBrickFactory();
    /*!
     * copy constructor (disabled)
     */
    BehaviourBrickFactory(const BehaviourBrickFactory&);
    /*!
     * assignement operator (disabled)
     */
    BehaviourBrickFactory&
    operator=(const BehaviourBrickFactory&);
    //! all registred constructors
    std::map<std::string,constructor> constructors;
  }; // end of struct BehaviourBrickFactory

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTBEHAVIOURBRICKFACTORY_H */

