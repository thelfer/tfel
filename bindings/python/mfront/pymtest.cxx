/*! 
 * \file   bindings/python/mfront/pymtest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  10 avril 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>

void declareMTestParser(void);
void declareMTest(void);
void declareCurrentState(void);
void declareMFrontLogStream(void);

BOOST_PYTHON_MODULE(mtest)
{
  declareCurrentState();
  declareMTestParser();
  declareMTest();
  declareMFrontLogStream();
}
