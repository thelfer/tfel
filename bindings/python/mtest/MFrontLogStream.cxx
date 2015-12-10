/*! 
 * \file  bindings/python/mfront/MFrontLogStream.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 sept. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>

#include"MFront/MFrontLogStream.hxx"

void declareMFrontLogStream(void);

void declareMFrontLogStream(void)
{
  using namespace boost;
  using namespace boost::python;
  using namespace mfront;

  enum_<VerboseLevel>("VerboseLevel")
    .value("VERBOSE_QUIET",VERBOSE_QUIET)
    .value("VERBOSE_LEVEL0",VERBOSE_LEVEL0)
    .value("VERBOSE_LEVEL1",VERBOSE_LEVEL1)
    .value("VERBOSE_LEVEL2",VERBOSE_LEVEL2)
    .value("VERBOSE_LEVEL3",VERBOSE_LEVEL3)
    .value("VERBOSE_DEBUG",VERBOSE_DEBUG)
    .value("VERBOSE_FULL",VERBOSE_FULL)
    ;

  def("setVerboseMode",setVerboseMode);

}
