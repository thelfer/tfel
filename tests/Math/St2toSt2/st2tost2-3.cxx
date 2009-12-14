/*!
 * \file   st2tost2-3.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   19 mar 2008
 */

#include<iostream>
#include<cstdlib>

#include"Math/st2tost2.hxx"
#include"Math/stensor.hxx"

int
main(void)
{
  using namespace tfel::math;
  using namespace std;

  cout << st2tost2<2>::Id() << endl;
  cout << st2tost2<2>::J() << endl;
  cout << st2tost2<2>::IxI() << endl;
  cout << st2tost2<2>::K() << endl;
  cout << st2tost2<2>::Id()-st2tost2<2>::IxI()/3 << endl;
  cout << st2tost2<2>::K()-(st2tost2<2>::Id()-st2tost2<2>::IxI()/3) << endl;

  return EXIT_SUCCESS;
}

