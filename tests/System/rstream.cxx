/*!
 * \file   rstream.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 nov 2007
 */

#include<iostream>
#include<cstdlib>

#include"System/rfstream.hxx"

int
main(void)
{
  using namespace std;

  tfel::system::rfstream r("test.bin");
  char c[5];
  double t;
  
  r.read(c,4u);
  c[4]='\0';
  r >> t;

  cout << c << endl;
  cout << t << endl;
  r.read(c,4u);
  c[4]='\0';
  cout << c << endl;

  r.close();
  
  return EXIT_SUCCESS;
} // end of main
