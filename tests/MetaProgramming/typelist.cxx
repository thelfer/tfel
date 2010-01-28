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
  typedef TypeListAppend<List,unsigned short>::type List2;
  typedef TypeListPrepend<List,long>::type List3;
  typedef GenerateTypeList<double,A,B,C,float,C,B,A>::type List4;
  typedef TypeListExtractSubClassesOf<List4,A>::type List5;
  typedef GenerateTypeList<double>::type List6;
  typedef GenerateTypeList<float>::type List7;
  typedef TypeListConcatenate<List6,List7>::type List8;

  assert(tfel::utilities::Name<List>::getName()==
	 "TypeListNode<double,TypeListNode<float,TypeListNode<std::string,TypeListEndType>>>");
  assert(tfel::utilities::Name<List2>::getName()==
	 "TypeListNode<double,TypeListNode<float,TypeListNode<std::string,TypeListNode<unsigned short,TypeListEndType>>>>");
  assert(tfel::utilities::Name<List3>::getName()==
	 "TypeListNode<long int,TypeListNode<double,TypeListNode<float,TypeListNode<std::string,TypeListEndType>>>>");
  assert(tfel::utilities::Name<List5>::getName()==
	 "TypeListNode<A,TypeListNode<B,TypeListNode<C,TypeListNode<C,TypeListNode<B,TypeListNode<A,TypeListEndType>>>>>>");
  assert(tfel::utilities::Name<List8>::getName()==
	 "TypeListNode<double,TypeListNode<float,TypeListEndType>>");

  return EXIT_SUCCESS;
}
