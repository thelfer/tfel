/*!
 * \file   SingleStructureScheme.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   09 déc. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>
#include"MTest/Evolution.hxx"
#include"MTest/FunctionEvolution.hxx"
#include"MTest/CastemEvolution.hxx"
#include"MTest/SingleStructureScheme.hxx"

static void
SingleStructureScheme_addEvolution(mtest::SingleStructureScheme& t,
		   const std::string&  n,
		   const mtest::real& v,
		   const bool b1,
		   const bool b2)
{
  t.addEvolution(n,mtest::make_evolution(v),b1,b2);
}

static void
SingleStructureScheme_addEvolution2(mtest::SingleStructureScheme& t,
		    const std::string&  n,
		    const std::map<mtest::real,
				   mtest::real>& v,
		    const bool b1,
		    const bool b2)
{
  t.addEvolution(n,mtest::make_evolution(v),b1,b2);
}

static void
SingleStructureScheme_addEvolution3(mtest::SingleStructureScheme& t,
		    const std::string&  n,
		    const std::string& f,
		    const bool b1,
		    const bool b2)
{
  using namespace std;
  using namespace mtest;
  shared_ptr<Evolution> pev(new FunctionEvolution(f,t.getEvolutions()));
  t.addEvolution(n,pev,b1,b2);
}

static void
SingleStructureScheme_setMaterialProperty(mtest::SingleStructureScheme& t,
					  const std::string&  n,
					  const mtest::real& v,
					  const bool b)
{
  using namespace mtest;
  std::shared_ptr<Evolution> pev(new ConstantEvolution(v));
  t.setMaterialProperty(n,pev,b);
}

static void
SingleStructureScheme_setMaterialProperty2(mtest::SingleStructureScheme& t,
					   const std::string& n,
					   const std::string& f,
					   const bool b)
{
  using namespace std;
  using namespace mtest;
  using mtest::real;
  shared_ptr<Evolution> mpev;
  mpev = shared_ptr<Evolution>(new FunctionEvolution(f,t.getEvolutions()));
  t.setMaterialProperty(n,mpev,b);
}

static void
SingleStructureScheme_setMaterialProperty3(mtest::SingleStructureScheme& t,
					   const std::string&  n,
					   const std::string& f,
					   const bool b)
{
  using namespace std;
  using namespace mtest;
  shared_ptr<Evolution> pev(new FunctionEvolution(f,t.getEvolutions()));
  t.setMaterialProperty(n,pev,b);
}

static void
SingleStructureScheme_setCastemMaterialProperty(mtest::SingleStructureScheme& t,
						const std::string& n,
						const std::string& l,
						const std::string& f,
						const bool b)
{
  using namespace std;
  using namespace mtest;
  using mtest::real;
  shared_ptr<Evolution> mpev;
  mpev = shared_ptr<Evolution>(new CastemEvolution(l,f,t.getEvolutions()));
  t.setMaterialProperty(n,mpev,b);
}

static void
SingleStructureScheme_setExternalStateVariable(mtest::SingleStructureScheme& t,
					       const std::string&  n,
					       const mtest::real& v,
					       const bool b)
{
  using namespace mtest;
  std::shared_ptr<Evolution> pev(new ConstantEvolution(v));
  t.setExternalStateVariable(n,pev,b);
}

static void
SingleStructureScheme_setExternalStateVariable2(mtest::SingleStructureScheme& t,
						const std::string&  n,
						const std::map<mtest::real,
						mtest::real>& v,
						const bool b)
{
  using namespace std;
  using namespace mtest;
  using mtest::real;
  vector<real> tv(v.size());
  vector<real> ev(v.size());
  vector<real>::size_type i;
  map<real,real>::const_iterator pv;
  for(pv=v.begin(),i=0;pv!=v.end();++pv,++i){
    tv[i] = pv->first;
    ev[i] = pv->second;
  }
  shared_ptr<Evolution> pev(new LPIEvolution(tv,ev));
  t.setExternalStateVariable(n,pev,b);
}

static void
SingleStructureScheme_setExternalStateVariable3(mtest::SingleStructureScheme& t,
						const std::string&  n,
						const std::string& f,
						const bool b)
{
  using namespace std;
  using namespace mtest;
  shared_ptr<Evolution> pev(new FunctionEvolution(f,t.getEvolutions()));
  t.setExternalStateVariable(n,pev,b);
}

void declareSingleStructureScheme(void);

void declareSingleStructureScheme(void)
{
  using namespace boost::python;
  using namespace mtest;
  class_<SingleStructureScheme,boost::noncopyable,
			bases<SchemeBase>>("SingleStructureScheme",
					   no_init)
    .def("setBehaviour",&SingleStructureScheme::setBehaviour)
    .def("setParameter",&SingleStructureScheme::setParameter)
    .def("setIntegerParameter",
	 &SingleStructureScheme::setIntegerParameter)
    .def("setUnsignedIntegerParameter",
	 &SingleStructureScheme::setUnsignedIntegerParameter)
    .def("setOutOfBoundsPolicy",
	 &SingleStructureScheme::setOutOfBoundsPolicy)
    .def("setMaterialProperty",
	 SingleStructureScheme_setMaterialProperty,
	 (arg("name"),"value",arg("check")=true))
    .def("setMaterialProperty",
	 SingleStructureScheme_setMaterialProperty2,
	 (arg("name"),"value",arg("check")=true))
    .def("setMaterialProperty",
	 SingleStructureScheme_setMaterialProperty3,
	 (arg("name"),"value","function",arg("check")=true))
    .def("setCastemMaterialProperty",
	 SingleStructureScheme_setCastemMaterialProperty,
	 (arg("name"),arg("library"),arg("function"),arg("check")=true))
    .def("setExternalStateVariable",
	 SingleStructureScheme_setExternalStateVariable,
	 (arg("name"),"value",arg("check")=true))
    .def("setExternalStateVariable",
	 SingleStructureScheme_setExternalStateVariable2,
	 (arg("name"),"value",arg("check")=true))
    .def("setExternalStateVariable",
	 SingleStructureScheme_setExternalStateVariable3,
	 (arg("name"),"value","function",arg("check")=true))
    .def("addReal",
	 SingleStructureScheme_addEvolution,
	 (arg("name"),"value",arg("b1")=true,arg("b2")=true))
    .def("addEvolution",
	 SingleStructureScheme_addEvolution,
	 (arg("name"),"value",arg("b1")=true,arg("b2")=true))
    .def("addEvolution",
	 SingleStructureScheme_addEvolution2,
	 (arg("name"),"value",arg("b1")=true,arg("b2")=true))
    .def("addEvolution",
	 SingleStructureScheme_addEvolution3,
	 (arg("name"),"value","function",arg("b1")=true,arg("b2")=true))
    ;

}
