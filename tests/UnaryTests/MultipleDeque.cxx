#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cassert>
#include<cmath>
#include<vector>

#include "Utilities/MultipleDeque.hxx"

int main(void){

  using namespace tfel::meta;

  typedef GenerateTypeList<std::string,double,int>::type MyList;
  tfel::utilities::MultipleDeque<MyList> mv;
  
  tfel::utilities::MultipleDeque<MyList>::iterator<double> p;
  tfel::utilities::MultipleDeque<MyList>::reverse_iterator<double> p1;

  assert(sizeof(mv)==sizeof(std::deque<std::string>)+sizeof(std::deque<double>)+sizeof(std::deque<int>));

  assert(mv.empty<std::string>()==true);
  mv.push_back<std::string>("Youpie");
  mv.push_back<std::string>("Choupie est la plus belle");
  assert(mv.empty<std::string>()==false);
  assert(mv.size<std::string>()==2);

  mv.push_back<double>(3.);
  mv.push_back<double>(5.68273);
  mv.push_back<double>(1.2);

  assert(mv.size<double>()==3);

  assert(mv.front<std::string>()=="Youpie");

  mv.front<std::string>()="Test";
  assert(mv.front<std::string>()=="Test");

  mv.clear<std::string>();
  assert(mv.empty<std::string>()==true);

  p=mv.begin<double>();
  assert(std::abs(*p-3.<1.e-14));
  ++p;
  assert(std::abs(*p-5.68273<1.e-14));
  ++p;
  assert(std::abs(*p-1.2<1.e-14));
  ++p;
  assert(p==mv.end<double>());
 
  p1=mv.rbegin<double>();
  assert(std::abs(*p1-1.2<1.e-14));
  ++p1;
  assert(std::abs(*p1-5.68273<1.e-14));
  ++p1;
  assert(std::abs(*p1-3.<1.e-14));
  ++p1;
  assert(p1==mv.rend<double>());

  return EXIT_SUCCESS;

}
