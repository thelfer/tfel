/*! 
 * \file  mfront/mtest/MTestAccelerationAlgorithm.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 */

#include<sstream>

#include"MFront/MTestAccelerationAlgorithm.hxx"

namespace mfront
{

  unsigned short
  MTestAccelerationAlgorithm::convertToUnsignedShort(const std::string& m,
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

  MTestAccelerationAlgorithm::~MTestAccelerationAlgorithm()
  {} // end of MTestAccelerationAlgorithm::~MTestAccelerationAlgorithm

} // end of namespace mfront
