/*! 
 * \file  bindings/python/tfel/system.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 31 aoû 2010
 */

#include<boost/python.hpp>

void declareProcessManagerCommand(void);
void declareExternalLibraryManager(void);

BOOST_PYTHON_MODULE(system)
{
  declareProcessManagerCommand();
  declareExternalLibraryManager();
}
