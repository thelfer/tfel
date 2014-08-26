/*! 
 * \file  Glossary.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 31 mars 2014
 */

#include<boost/python.hpp>

#include"TFEL/Glossary/Glossary.hxx"

void
declareGlossary(void)
{
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::glossary;

class_<Glossary,noncopyable>("Glossary",no_init)
  .def("getGlossary",Glossary::getGlossary,
       return_value_policy<reference_existing_object>())
  .staticmethod("getGlossary")
  .def("contains",&Glossary::contains)
  .def_readonly("IrradiationTemperature",&Glossary::IrradiationTemperature,
		"la température moyenne (temporelle) au cours de l'irradiation au point considéré")
  .def_readonly("Temperature",&Glossary::Temperature,
		"la température")
  .def_readonly("KelvinTemperature",&Glossary::KelvinTemperature,
		"la température")
  ;
} // end of declareGlossary
