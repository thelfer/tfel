/*! 
 * \file   mfront/include/MFront/AbstractBehaviourBrick.hxx
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

#ifndef LIB_MFRONT_MFRONTBEHAVIOURBRICK_H_
#define LIB_MFRONT_MFRONTBEHAVIOURBRICK_H_ 

#include<map>
#include<string>

namespace mfront
{

  // forward declaration
  struct BehaviourDescription;

  /*!
   * AbstractBehaviourBrick are ready-to use block used to build a complex behaviour.
   */
  struct AbstractBehaviourBrick
  {
    /*!
     * Object used to pass parameters to AbstractBehaviourBricks constructor
     * The key   is the parameter name.
     * The value is the parameter value.
     */
    using Parameters = std::map<std::string,std::string>;
    //! a simple alias
    using Parameter  = Parameters::value_type;
    //! ends the file treatment
    virtual void 
    endTreatment(BehaviourDescription&) const = 0;
    //! destructor
    virtual ~AbstractBehaviourBrick();
  }; // end of struct AbstractBehaviourBrick

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBEHAVIOURBRICK_H_ */

