/*! 
 * \file  bindings/python/mfront/MTestParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 20 sept. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>

#include"MTest/MTest.hxx"
#include"MTest/MTestParser.hxx"

void declareMTestParser(void)
{
  using namespace boost;
  using namespace boost::python;
  using namespace mfront;
  class_<MTestParser,noncopyable>("MTestParser")
    .def("execute",&MTestParser::execute)
    ;

} // end of declareExternalLibraryManager
