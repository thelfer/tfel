/*! 
 * \file  ModellingHypothesis.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 mars 2013
 */

#include<boost/python.hpp>

#include"TFEL/Material/ModellingHypothesis.hxx"

BOOST_PYTHON_MODULE(material)
{
  using namespace boost::python;
  using namespace tfel::material;
  enum_<ModellingHypothesis::Hypothesis>("ModellingHypothesis")
    .value("AXISYMETRICALGENERALISEDPLANESTRAIN",
	   ModellingHypothesis::AXISYMETRICALGENERALISEDPLANESTRAIN)
    .value("AXISYMETRICAL",ModellingHypothesis::AXISYMETRICAL)
    .value("PLANESTRESS",ModellingHypothesis::PLANESTRESS)
    .value("PLANESTRAIN",ModellingHypothesis::PLANESTRAIN)
    .value("GENERALISEDPLANESTRAIN",ModellingHypothesis::GENERALISEDPLANESTRAIN)
    .value("TRIDIMENSIONAL",ModellingHypothesis::TRIDIMENSIONAL)
    .value("UNDEFINEDHYPOTHESIS",ModellingHypothesis::UNDEFINEDHYPOTHESIS)
    ;

}

