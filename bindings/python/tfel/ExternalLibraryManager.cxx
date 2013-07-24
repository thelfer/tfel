/*! 
 * \file  ExternalLibraryManager.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 juil. 2011
 */

#include<boost/python.hpp>

#include"TFEL/System/ExternalLibraryManager.hxx"

static void
ELM_loadLibrary(tfel::system::ExternalLibraryManager& elm,
				   const std::string& n)
{
  elm.loadLibrary(n);
}

static void
ELM_setUShortParameter(tfel::system::ExternalLibraryManager& elm,
					  const std::string& l,
					  const std::string& n,
					  const std::string& p,
					  const unsigned short v)
{
  elm.setParameter(l,n,p,v);
}

static void
ELM_setIntParameter(tfel::system::ExternalLibraryManager& elm,
				       const std::string& l,
				       const std::string& n,
				       const std::string& p,
				       const int v)
{
  elm.setParameter(l,n,p,v);
}

static void
ELM_setDoubleParameter(tfel::system::ExternalLibraryManager& elm,
					  const std::string& l,
					  const std::string& n,
					  const std::string& p,
					  const double v)
{
  elm.setParameter(l,n,p,v);
}

void declareExternalLibraryManager(void)
{
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::system;
  typedef ExternalLibraryManager ELM;
  class_<ELM,noncopyable>("ExternalLibraryManager",no_init)
    .def("getExternalLibraryManager",ELM::getExternalLibraryManager,
	 return_value_policy<reference_existing_object>())
    .staticmethod("getExternalLibraryManager")
    .def("loadLibrary",ELM_loadLibrary)
    .def("getSource",&ELM::getSource)
    .def("setUShortParameter",ELM_setUShortParameter)
    .def("setIntParameter",ELM_setIntParameter)
    .def("setDoubleParameter",ELM_setDoubleParameter)
    .def("setParameter",ELM_setDoubleParameter)
    .def("contains",&ELM::contains)
    .def("getUMATSymmetryType",
	 &ELM::getUMATSymmetryType)
    .def("getUMATMaterialPropertiesNames",
	 &ELM::getUMATMaterialPropertiesNames)
    .def("getUMATInternalStateVariablesNames",
	 &ELM::getUMATInternalStateVariablesNames)
    .def("getUMATInternalStateVariablesTypes",
	 &ELM::getUMATInternalStateVariablesTypes)
    .def("getUMATExternalStateVariablesNames",
	 &ELM::getUMATExternalStateVariablesNames)
    .def("checkIfAsterBehaviourRequiresElasticMaterialPropertiesOffset",
	 &ELM::checkIfAsterBehaviourRequiresElasticMaterialPropertiesOffset)
    .def("checkIfAsterBehaviourRequiresThermalExpansionMaterialPropertiesOffset",
	 &ELM::checkIfAsterBehaviourRequiresThermalExpansionMaterialPropertiesOffset)
    .def("checkIfAsterBehaviourSaveTangentOperator",
	 &ELM::checkIfAsterBehaviourSavesTangentOperator)
    ;

} // end of declareExternalLibraryManager
