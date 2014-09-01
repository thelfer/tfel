/*! 
 * \file  mfront/src/DrivingVariable.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 mai 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

