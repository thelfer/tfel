/*!
 * \file   bindings/python/mfront/AbstractDSL.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   04 mai 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<set>
#include<memory>
#include<boost/python.hpp>
#include"TFEL/Python/SharedPtr.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/AbstractDSL.hxx"

static void AbstractDSL_setInterfaces(mfront::AbstractDSL& dsl,
				      const std::vector<std::string>& i)
{
  std::set<std::string> inames(i.begin(),i.end());
  dsl.setInterfaces(inames);
}

static std::vector<std::string>
AbstractDSL_getKeywordsList(mfront::AbstractDSL& dsl)
{
  std::vector<std::string> keys;
  dsl.getKeywordsList(keys);
  return keys;
}

static void
AbstractDSL_analyseFile1(mfront::AbstractDSL& dsl,
			 const std::string& f)
{
  dsl.analyseFile(f,{});
}

static void
AbstractDSL_analyseFile2(mfront::AbstractDSL& dsl,
			 const std::string& f,
			 const std::vector<std::string>& args)
{
  dsl.analyseFile(f,args);
}

void declareAbstractDSL(void){
  using namespace boost::python;
  using namespace mfront;
  enum_<AbstractDSL::DSLTarget>("DSLTarget")
    .value("MATERIALPROPERTYDSL",AbstractDSL::MATERIALPROPERTYDSL)
    .value("BEHAVIOURDSL",AbstractDSL::BEHAVIOURDSL)
    .value("MODELDSL",AbstractDSL::MODELDSL)
    ;
  class_<AbstractDSL,std::shared_ptr<AbstractDSL>,
	 boost::noncopyable>("AbstractDSL",no_init)
    .def("getTargetType",&AbstractDSL::getTargetType)
    .def("getFileDescription",&AbstractDSL::getFileDescription,
	 return_internal_reference<>())
    .def("analyseFile",AbstractDSL_analyseFile1)
    .def("analyseFile",AbstractDSL_analyseFile2)
    .def("analyseString",&AbstractDSL::analyseString)
    .def("getTargetsDescription",&AbstractDSL::getTargetsDescription,
	 return_internal_reference<>())
    .def("generateOutputFiles",&AbstractDSL::generateOutputFiles)
    .def("generateOutputFiles",&AbstractDSL::generateOutputFiles)
    .def("setInterfaces",&AbstractDSL_setInterfaces)
    .def("getKeywordsList",&AbstractDSL_getKeywordsList)
    ;
}
