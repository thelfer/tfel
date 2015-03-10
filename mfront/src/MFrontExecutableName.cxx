/*! 
 * \file  MFrontExecutableName.cxx
 * \brief
 * \author Helfer Thomas
 * \date   03 mars 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>
#include"MFront/MFrontExecutableName.hxx"

namespace mfront{
  
  static std::string&
  getMFrontExecutableNameReference(void)
  {
    static std::string n;
    return n;
  }
  
  MFRONT_VISIBILITY_EXPORT void
  setMFrontExecutableName(const char* const n)
  {
    using namespace std;
    string& s = getMFrontExecutableNameReference();
    if(!s.empty()){
      string msg{"setMFrontExecutableName : "
	  "name already set to '"+s+"' (new name is '"+
	  string(n)+"'"};
      throw(runtime_error{msg});
    }
    s = n;
  }

  MFRONT_VISIBILITY_EXPORT const std::string&
  getMFrontExecutableName(void){
    using namespace std;
    string& s = getMFrontExecutableNameReference();
    if(s.empty()){
      string msg{"getMFrontExecutableName : no name set"};
      throw(runtime_error{msg});
    }
    return s;
  }

} // end of namespace mfront
