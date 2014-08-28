/*!
 * \file   tests/Math/ExpressionTemplates/test_expr8.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   17 Oct 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cassert>
#include<cstdlib>
#include<algorithm>
#include<iterator>

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Math/General/FctObjectRandomAccessConstIterator.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/functions.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::math;
  using tfel::math::vector;
  USING_TFEL_FUNCTIONS;

  vector<double> v(4);
  vector<double>::size_type i;

  v(0) = 12.12;
  v(1) = 12.234;
  v(2) = 22.4322;
  v(3) = 2.2112;

  FctObjectRandomAccessConstIterator<vector<double>,FctSin> begin(v.begin(),sin);
  FctObjectRandomAccessConstIterator<vector<double>,FctSin> end(v.end(),sin);
  FctObjectRandomAccessConstIterator<vector<double>,FctSin> p;
  
  assert(name(p)=="FctObjectRandomAccessConstIterator<tfel::math::vector<double>,FctSin>");

  for(i=0,p=begin;p!=end;++p,++i){
    assert(abs(*p-sin(v(i)))<1.e-14);
  }

  return EXIT_SUCCESS;
}
