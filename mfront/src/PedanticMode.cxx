/*! 
 * \file   mfront/src/PedanticMode.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  20/05/2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/PedanticMode.hxx"

namespace mfront{

  static bool& getMFrontPedanticMode()
  {
    static bool b = false;
    return b;
  }

  bool getPedanticMode(void)
  {
    return getMFrontPedanticMode();
  }

  void setPedanticMode(const bool b)
  {
    getMFrontPedanticMode() = b;
  }

}

