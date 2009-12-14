/*!
  \file   IntegersSet.cxx
  \author Helfer Thomas
  \date   13 mai 2006
  
  \brief  
*/

#include<iostream>
#include<cstdlib>
#include<cassert>

#include "Utilities/IntegersSet.hxx"

int main(void){

  tfel::utilities::IntegersSet<> test(12,14);
  assert(test.size()==3);
  assert(test.block_number()==1);

  test.add(12);
  assert(test.size()==3);
  assert(test.block_number()==1);
  assert(*(test.begin())==12);
  assert(test.position(13)==1);

  test.add(17);
  assert(test.size()==4);
  assert(test.block_number()==2);
  assert(*(test.begin())==12);
  assert(test.position(17)==3);
  
  test.add(11);
  assert(test.size()==5);
  assert(test.block_number()==2);

  test.add(15);
  assert(test.size()==6);
  assert(test.block_number()==2);

  test.add(16);
  assert(test.size()==7);
  assert(test.block_number()==1);

  test.add(18);
  assert(test.size()==8);
  assert(test.block_number()==1);

  assert(*(test.begin())==11);
  assert(test.position(13)==2);

  std::cout << "success" << std::endl;

  return EXIT_SUCCESS;

}
