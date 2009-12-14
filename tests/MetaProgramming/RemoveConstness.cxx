/*!
 * \file   remove_const.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   18 Oct 2006
 */

#include<iostream>
#include<cstdlib>

#include"Metaprogramming/RemoveConstness.hxx"
#include"Utilities/Name.hxx"

int main(void)
{
  using namespace tfel::utilities;
  using namespace tfel::meta;
  using namespace std;
  
  cout << Name<double>::getName() << endl;
  cout << Name<const double>::getName() << endl;
  cout << Name<RemoveConstness<const double>::type>::getName() << endl;

  return EXIT_SUCCESS;
}
