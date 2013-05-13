/*! 
 * \file  ThermodynamicForce.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 mai 2013
 */

#ifndef _LIB_MFRONT_THERMODYNAMICFORCE_H_
#define _LIB_MFRONT_THERMODYNAMICFORCE_H_ 

namespace mfront
{

  /*!
   * structure describes the thermodynamic force associated with a
   * driving variable.
   */
  struct ThermodynamicForce
  {
    //! name of the thermodynamic force
    std::string name;
    //! type of the thermodynamic force
    std::string type;
  }; // end of struct ThermodynamicForce
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_THERMODYNAMICFORCE_H */

