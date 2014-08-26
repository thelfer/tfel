/*! 
 * \file   MFrontPedanticMode.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  20/05/2014
 */

#include"MFront/MFrontPedanticMode.hxx"

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

