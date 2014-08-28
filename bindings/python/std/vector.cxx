/*! 
 * \file   bindings/python/std/vector.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 avr 2010
 */

#include<string>

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/System/ProcessManager.hxx"

#include"TFEL/Python/VectorConverter.hxx"

struct DontDeclareAnyThing
{
  static void
  declare()
  {}
};

struct DeclareProcessIdVector
{
  static void
  declare(void)
  {
    using namespace tfel::system;
    using namespace tfel::python;
    initializeVectorConverter<std::vector<ProcessManager::ProcessId> >();
  }
};

BOOST_PYTHON_MODULE(_stl_vector)
{
  using namespace std;
  using namespace tfel::meta;
  using namespace tfel::math;
  using namespace tfel::system;
  using namespace tfel::python;
  using std::vector;

  initializeVectorConverter<vector<int> >();
  initializeVectorConverter<vector<vector<int> > >();
  initializeVectorConverter<vector<double> >();
  initializeVectorConverter<vector<vector<double> > >();

  initializeVectorConverter<vector<tvector<1u,double> > >();
  initializeVectorConverter<vector<tvector<2u,double> > >();
  initializeVectorConverter<vector<tvector<3u,double> > >();
  initializeVectorConverter<vector<stensor<1u,double> > >();
  initializeVectorConverter<vector<stensor<2u,double> > >();
  initializeVectorConverter<vector<stensor<3u,double> > >();

  initializeVectorConverter<vector<string> >();
  initializeVectorConverter<vector<vector<string> > >();
  initializeVectorConverter<vector<pair<string,string> > >();
  
  IF<IsSameType<ProcessManager::ProcessId,
		int>::cond,
     DontDeclareAnyThing,
     DeclareProcessIdVector>::type::declare();

}

