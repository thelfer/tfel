/*! 
 * \file   mfront/include/MFront/BehaviourBrick.hxx
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

  /*!
   * BehaviourBrick are ready-to use block used to build a complex behaviour.
   */
  struct BehaviourBrick
  {
    /*!
     * Object used to pass parameters to BehaviourBricks constructor
     * The key   is the parameter name.
     * The value is the parameter value.
     */
    typedef std::map<std::string,std::string> Parameters;
    //! a simple alias
    typedef Parameters::value_type Parameter;
    /*!
     * destructor
     */
    virtual ~BehaviourBrick();
  }; // end of struct BehaviourBrick

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBEHAVIOURBRICK_H_ */

