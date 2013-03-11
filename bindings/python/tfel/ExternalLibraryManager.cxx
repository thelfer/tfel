/*! 
 * \file  ExternalLibraryManager.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 juil. 2011
 */

#include<boost/python.hpp>

#include"TFEL/System/ExternalLibraryManager.hxx"

static void
ExternalLibraryManager_loadLibrary(tfel::system::ExternalLibraryManager& elm,
				   const std::string& n)
{
  elm.loadLibrary(n);
}

void declareExternalLibraryManager(void)
{
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::system;
  
  class_<ExternalLibraryManager,noncopyable>("ExternalLibraryManager",no_init)
    .def("getExternalLibraryManager",ExternalLibraryManager::getExternalLibraryManager,
	 return_value_policy<reference_existing_object>())
    .staticmethod("getExternalLibraryManager")
    .def("loadLibrary",ExternalLibraryManager_loadLibrary)
    ;

} // end of declareExternalLibraryManager
