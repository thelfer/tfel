/*!
 * \file   process.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 nov 2007
 */

#include<cstdlib>

#include"TFEL/System/ProcessManager.hxx"

int
main(void)
{
  using namespace tfel::system;
  ProcessManager m;
  m.createProcess("./test");
  return EXIT_SUCCESS;
}
