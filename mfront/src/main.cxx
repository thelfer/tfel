/*!
 * \file   mfront/src/main.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<windows.h>
#endif /* __CYGWIN__ */

#include<cstdlib>
#include"MFront/InitDSLs.hxx"
#include"MFront/InitInterfaces.hxx"
#include"MFront/MFront.hxx"

int main(const int argc, const char *const *const argv)
{
  using namespace mfront;
  initParsers();
  initInterfaces();
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
  try{
#endif /* __CYGWIN__ */
  MFront mfront(argc,argv);
  mfront.exe();
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
  }
  catch(std::exception& e){
    MessageBox(nullptr,e.what(),
	       "mfront",0);
    return EXIT_FAILURE;
  }
  catch(...){
    return EXIT_FAILURE;
  }
#endif /* __CYGWIN__ */
  return EXIT_SUCCESS;
}
