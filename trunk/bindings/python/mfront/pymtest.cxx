/*! 
 * \file  system.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 31 aoû 2010
 */

#include<boost/python.hpp>

void declareMTestParser(void);
void declareMTest(void);
void declareMTestLogStream(void);

BOOST_PYTHON_MODULE(mtest)
{
  declareMTestParser();
  declareMTest();
  declareMTestLogStream();
}
