/*! 
 * \file  stl.cxx
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
    initializeVectorConverter<ProcessManager::ProcessId>();
  }
};

BOOST_PYTHON_MODULE(_stl_vector)
{
  using namespace std;
  using namespace tfel::meta;
  using namespace tfel::math;
  using namespace tfel::system;
  using namespace tfel::python;

  initializeVectorConverter<int>();
  initializeVectorConverter<vector<int> >();
  initializeVectorConverter<double>();
  initializeVectorConverter<vector<double> >();

  initializeVectorConverter<tvector<1u,double> >();
  initializeVectorConverter<tvector<2u,double> >();
  initializeVectorConverter<tvector<3u,double> >();
  initializeVectorConverter<stensor<1u,double> >();
  initializeVectorConverter<stensor<2u,double> >();
  initializeVectorConverter<stensor<3u,double> >();

  initializeVectorConverter<string>();
  initializeVectorConverter<vector<string> >();
  initializeVectorConverter<pair<string,string> >();
  
  IF<IsSameType<ProcessManager::ProcessId,
		int>::cond,
     DontDeclareAnyThing,
     DeclareProcessIdVector>::type::declare();

}

