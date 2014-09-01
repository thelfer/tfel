/*! 
 * \file   bindings/python/std/vector.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 avr 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

