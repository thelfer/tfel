/*! 
 * \file  bindings/python/mfront/MTestParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 20 sept. 2013
 */

#include<boost/python.hpp>

#include"MFront/MTest.hxx"
#include"MFront/MTestParser.hxx"

void declareMTestParser(void)
{
  using namespace boost;
  using namespace boost::python;
  using namespace mfront;
  class_<MTestParser,noncopyable>("MTestParser")
    .def("execute",&MTestParser::execute)
    ;

} // end of declareExternalLibraryManager
