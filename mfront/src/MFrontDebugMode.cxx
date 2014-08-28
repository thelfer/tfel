/*! 
 * \file  mfront/src/MFrontDebugMode.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 avril 2014
 */

#include"MFront/MFrontDebugMode.hxx"

namespace mfront{

  static bool& getMFrontDebugMode()
  {
    static bool b = false;
    return b;
  }

  bool getDebugMode(void)
  {
    return getMFrontDebugMode();
  }

  void setDebugMode(const bool b)
  {
    getMFrontDebugMode() = b;
  }

}

