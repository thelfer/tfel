/*!
 * \file   tests/Math/ExpressionTemplates/test_expr_tvector.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   16 Oct 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include"TFEL/Tests/Test.hxx"
#include"TFEL/Tests/TestManager.hxx"
#include"TFEL/Tests/TestFunctionWrapper.hxx"

#include"tvector-header.hxx"

bool test2(void)
{
  using namespace std;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;

  tvector<2,tvector<3,unsigned short> >    v;
  tvector<2,tvector<3,float> >  v2;
  tvector<2,tvector<3,double> > v3;

  v(0)(0) = 2;
  v(0)(1) = 7;
  v(0)(2) = 9;
  v(1)(0) = 2;
  v(1)(1) = 7;
  v(1)(2) = 9;

  v2(0)(0) = 2.34f;
  v2(0)(1) = 54.2f;
  v2(0)(2) = 412.232f;
  v2(1)(0) = 2.34f;
  v2(1)(1) = 54.2f;
  v2(1)(2) = 412.232f;

  v3 = 2.*v+v2;

  if(!(abs(v3(0)(0)-(2.*v(0)(0)+v2(0)(0)))<1.e-10)){
    return false;
  }
  if(!(abs(v3(0)(1)-(2.*v(0)(1)+v2(0)(1)))<1.e-10)){
    return false;
  }
  if(!(abs(v3(0)(2)-(2.*v(0)(2)+v2(0)(2)))<1.e-10)){
    return false;
  }
  if(!(abs(v3(1)(0)-(2.*v(1)(0)+v2(1)(0)))<1.e-10)){
    return false;
  }
  if(!(abs(v3(1)(1)-(2.*v(1)(1)+v2(1)(1)))<1.e-10)){
    return false;
  }
  if(!(abs(v3(1)(2)-(2.*v(1)(2)+v2(1)(2)))<1.e-10)){
    return false;
  }
  return true;
}

bool test3(void)
{
  using namespace std;
  using namespace tfel::math;

  tvector<2,tvector<3,float> >  v;
  tvector<2,tvector<3,float> >  v2;
  tvector<2,tvector<3,double> > v3;
  tvector<2,tvector<3,double> > v4;

  v(0)(0) = 2.f;
  v(0)(1) = 7.f;
  v(0)(2) = 9.f;
  v(1)(0) = 2.f;
  v(1)(1) = 7.f;
  v(1)(2) = 9.f;

  v2(0)(0) = 2.34f;
  v2(0)(1) = 54.2f;
  v2(0)(2) = 412.232f;
  v2(1)(0) = 2.34f;
  v2(1)(1) = 54.2f;
  v2(1)(2) = 412.232f;

  v3(0)(0) = 2.34f;
  v3(0)(1) = 54.2f;
  v3(0)(2) = 412.232f;
  v3(1)(0) = 2.34f;
  v3(1)(1) = 54.2f;
  v3(1)(2) = 412.232f;

  v4 = v+v2+v3;

  if(!(abs(v4(0)(0)-(v(0)(0)+v2(0)(0)+v3(0)(0)))<1.e-14)){
    return false;
  }
  if(!(abs(v4(0)(1)-(v(0)(1)+v2(0)(1)+v3(0)(1)))<1.e-14)){
    return false;
  }
  if(!(abs(v4(0)(2)-(v(0)(2)+v2(0)(2)+v3(0)(2)))<1.e-14)){
    return false;
  }
  if(!(abs(v4(1)(0)-(v(1)(0)+v2(1)(0)+v3(1)(0)))<1.e-14)){
    return false;
  }
  if(!(abs(v4(1)(1)-(v(1)(1)+v2(1)(1)+v3(1)(1)))<1.e-14)){
    return false;
  }
  if(!(abs(v4(1)(2)-(v(1)(2)+v2(1)(2)+v3(1)(2)))<1.e-14)){
    return false;
  }
  return true;
}

int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  typedef TestFunctionWrapper<test1<float> >       Wrapper1;
  typedef TestFunctionWrapper<test1<double> >      Wrapper2;
  typedef TestFunctionWrapper<test1<long double> > Wrapper3;
  typedef TestFunctionWrapper<test2>               Wrapper4;
  typedef TestFunctionWrapper<test3>               Wrapper5;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("test_expr_tvector.xml");
  manager.addTest("test1",shared_ptr<Test>(new Wrapper1("test1<float>")));
  manager.addTest("test1",shared_ptr<Test>(new Wrapper2("test1<double>")));
  manager.addTest("test1",shared_ptr<Test>(new Wrapper3("test1<long double>")));
  manager.addTest("test2",shared_ptr<Test>(new Wrapper4("test2")));
  manager.addTest("test3",shared_ptr<Test>(new Wrapper5("test3")));
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main
