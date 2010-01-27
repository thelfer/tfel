/*!
 * \file   parser.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 sep 2007
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>

#include"Math/Evaluator.hxx"

int
main(const int,
     const char * const * const)
{
  using namespace std;
  using namespace tfel::math;
  
  Evaluator ev("min(x,y)*x");
  ev.setVariableValue("x",1.5);
  ev.setVariableValue("y",2.54);

  return EXIT_SUCCESS;
}
