/*!
 * \file   ST2toST2FTMV.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   17 oct 2008
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cassert>
#include<cstdlib>

#include"TFEL/Math/ST2toST2/ST2toST2FTMV.hxx"

int
main(void)
{
  using namespace std;
  using namespace tfel::math;
  tmatrix<6,6> m(0.);
  ST2toST2FTMV<1,6,6,0,0>::type ms(m);
  ST2toST2FTMV<1,6,6,3,3>::type ms2(m);
  unsigned short i;
  unsigned short j;
  for(i=0;i!=6;++i){
    m(i,i) = 1.;
  }

  for(i=0;i!=3;++i){
    for(j=0;j!=3;++j){
      if(i==j){
	assert(abs(ms(i,i)-1.)<1.e-14);
      } else {
	assert(abs(ms(i,j))<1.e-14);
      }
    }
  }

  for(i=0;i!=3;++i){
    for(j=0;j!=3;++j){
      if(i==j){
	assert(abs(ms2(i,i)-1.)<1.e-14);
      } else {
	assert(abs(ms2(i,j))<1.e-14);
      }
    }
  }

  return EXIT_SUCCESS;
}
