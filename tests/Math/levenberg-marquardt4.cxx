/*!
 * \file   levenberg-marquardt4.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   19 nov 2008
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<iostream>
#include<iterator>
#include<algorithm>
#include<functional>

#include<cstdlib>
#include<cmath>

#include<utility>
#include<vector>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Math/Evaluator.hxx"
#include"TFEL/Math/LevenbergMarquardt.hxx"
#include"TFEL/Math/LevenbergMarquardt/LevenbergMarquardtEvaluatorWrapper.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::math;
  using tfel::math::vector;
  typedef LevenbergMarquardtEvaluatorWrapper EvaluatorWrapper;
  const double x_data[] = {0.840188,
			   0.783099,
			   0.911647,
			   0.335223,
			   0.277775,
			   0.477397,
			   0.364784,
			   0.952230,
			   0.635712,
			   0.141603};

  const double y_data[] ={2.12989,
			  2.29532,
			  1.93195,
			  2.72801,
			  2.71560,
			  2.66178,
			  2.69570,
			  1.89371,
			  2.52963,
			  2.72843};
  std::vector<string> v;
  v.push_back("x");
  v.push_back("p0");
  v.push_back("p1");
  shared_ptr<Evaluator> test(new Evaluator(v,"p1*exp(p0*cos(x*x))"));
  LevenbergMarquardt<EvaluatorWrapper> levmar(EvaluatorWrapper(test,1u,2u));
  vector<double> x(1u);
  vector<double> p(2u);
  unsigned short i;
  for(i=0;i!=10;++i){
    x(0) = x_data[i];
    levmar.addData(x,y_data[i]);
  }
  // initial guess
  p(0)=1.2;
  p(1)=-0.2;
  levmar.setInitialGuess(p);
  // execute
  p = levmar.execute();
  cout << "res : ";
  copy(p.begin(),p.end(),ostream_iterator<double>(cout," "));
  cout << endl;
  return EXIT_SUCCESS;
} // end of main
