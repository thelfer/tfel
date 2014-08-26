/*! 
 * \file   pymtest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  10 avril 2014
 */

#include<boost/python.hpp>

void declareMTestParser(void);
void declareMTest(void);
void declareMFrontLogStream(void);

BOOST_PYTHON_MODULE(mtest)
{
  declareMTestParser();
  declareMTest();
  declareMFrontLogStream();
}
