/*!
 * \file   process.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 nov 2007
 */

#include<iostream>
#include<cstdlib>

#include"System/ProcessManager.hxx"

int
main(void)
{
  using namespace std;
  using namespace tfel::system;
  
  ProcessManager m;
  //  m.execute("cat ./test.txt","","out.txt");
  cout << "creating process : " << m.createProcess("./test") << endl;
  cout << "creating process : " << m.createProcess("./test") << endl;
  return EXIT_SUCCESS;
}
