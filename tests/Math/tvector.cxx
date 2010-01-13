#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>

#include "Math/qt.hxx"
#include "Math/tvector.hxx"

int main(void){
  
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::utilities;

  typedef tvector<5,qt<Acceleration> > vector1;
  typedef tvector<5,qt<Acceleration,int> > vector2;
  typedef tvector<5,qt<Force> > vector3;

  qt<Mass,int> m = qt<Mass,int>(2);
  vector1 v1;
  vector2 v2;
  vector3 v3;

  v2(0)=qt<Acceleration,int>(2);
  v2(1)=qt<Acceleration,int>(1);
  v2(2)=qt<Acceleration,int>(4);
  v2(3)=qt<Acceleration,int>(2);
  v2(4)=qt<Acceleration,int>(3);

  v1=2.*(v2+v2);
  v3 = m*v1;

//   cout << v2 << endl;
//   cout << v1 << endl;
//   cout << v3 << endl;
    
//   //  cout << norm(v3) << endl;
//   cout << (v3|v1) << endl;
//   cout << name(v3) << endl;
//   cout << name(v1) << endl;
//   cout << name((v3|v1)) << endl;

  return EXIT_SUCCESS;

}
