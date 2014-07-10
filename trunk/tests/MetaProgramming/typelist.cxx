/*!
 * \file   typelist.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   06 aoû 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cassert>
#include<cstdlib>
#include<vector>
#include<string>

#include"TFEL/Metaprogramming/TypeList.hxx"
#include"TFEL/Utilities/Name.hxx"

struct A{
  static const std::string getName(void)
  {
    return "A";
  }
};

struct B
  : public A
{
  static const std::string getName(void)
  {
    return "B";
  }
};

struct C
  : public B
{
  static const std::string getName(void)
  {
    return "C";
  }
};


int main(void)
{
  using namespace std;
  using namespace tfel::meta;

  typedef GenerateTypeList<double,float,string>::type List;
  typedef TLAppend<List,unsigned short>::type List2;
  typedef TLPrepend<List,long>::type List3;
  typedef GenerateTypeList<double,A,B,C,float,C,B,A>::type List4;
  typedef TLExtractSubClassesOf<List4,A>::type List5;
  typedef GenerateTypeList<double>::type List6;
  typedef GenerateTypeList<float>::type List7;
  typedef TLConcatenate<List6,List7>::type List8;

  assert(tfel::utilities::Name<List>::getName()==
	 "TLNode<double,TLNode<float,TLNode<std::string,TLE>>>");
  assert(tfel::utilities::Name<List2>::getName()==
	 "TLNode<double,TLNode<float,TLNode<std::string,TLNode<unsigned short,TLE>>>>");
  assert(tfel::utilities::Name<List3>::getName()==
	 "TLNode<long int,TLNode<double,TLNode<float,TLNode<std::string,TLE>>>>");
  assert(tfel::utilities::Name<List5>::getName()==
	 "TLNode<A,TLNode<B,TLNode<C,TLNode<C,TLNode<B,TLNode<A,TLE>>>>>>");
  assert(tfel::utilities::Name<List8>::getName()==
	 "TLNode<double,TLNode<float,TLE>>");

  return EXIT_SUCCESS;
}
