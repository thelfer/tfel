/*! 
 * \file  DrivingVariable.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 mai 2013
 */

#include"MFront/DrivingVariable.hxx"

namespace mfront
{

  //! comparison operator
  bool
  DrivingVariable::operator < (const DrivingVariable& o) const
  {
    return this->name < o.name;
  }

} // end of namespace mfront

