/*! 
 * \file   mfront/include/MFront/DrivingVariable.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  11 mai 2013
 */

#ifndef _LIB_MFRONT_DRIVINGVARIABLE_H_
#define _LIB_MFRONT_DRIVINGVARIABLE_H_ 

#include<string>

namespace mfront
{

  /*!
   * structure in charge of describing the main inputs of a behaviour
   * and their associated thermodynamic forces.  For example, small
   * strain behaviours have the total strain as driving variable.
   */
  struct DrivingVariable
  {
    //! name of the driving variable
    std::string name;
    //! type of the driving variable
    std::string type;
    //! This flags tells which of the driving variable increment or of
    //! the value of driving variable at the end of the time step is
    //! given.
    bool increment_known;
    /*!
     * comparison operator
     * \param[in] o : the other driving variable
     */
    bool operator < (const DrivingVariable&) const;
  }; // end of struct DrivingVariable
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_DRIVINGVARIABLE_H */

