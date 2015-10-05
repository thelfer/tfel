/*! 
 * \file  mfront/mtest/AccelerationAlgorithm.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>

#include"MTest/AccelerationAlgorithm.hxx"

namespace mtest
{

  unsigned short
  AccelerationAlgorithm::convertToUnsignedShort(const std::string& m,
						     const std::string& v)
  {
    using namespace std;
    istringstream converter(v);
    unsigned short res;
    converter >> res;
    if((!converter)&&(!converter.eof())){
      string msg(m+" : "
		 "can't convert string '"+v+"' to unsigned short");
      throw(runtime_error(msg));
    }
    return res;
  }

  AccelerationAlgorithm::~AccelerationAlgorithm()
  {} // end of AccelerationAlgorithm::~AccelerationAlgorithm

} // end of namespace mtest
