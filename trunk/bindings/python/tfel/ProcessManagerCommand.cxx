/*! 
 * \file  ProcessManagerCommand.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 31 aoû 2010
 */

#include<boost/python.hpp>

#include<TFEL/System/ProcessManager.hxx>

struct ProcessManagerCommandWrapper
  : tfel::system::ProcessManager::Command,
    boost::python::wrapper<tfel::system::ProcessManager::Command>
{
  bool execute(const tfel::system::ProcessManager::StreamId i,
	       const tfel::system::ProcessManager::StreamId o)
  {
    return this->get_override("execute")(i,o);
  } // end of execute
  ~ProcessManagerCommandWrapper()
  {} // end of ~ProcessManagerCommandWrapper
}; // end of struct Command


void
declareProcessManagerCommand()
{
  using namespace std;
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::system;

  class_<ProcessManagerCommandWrapper,
    boost::noncopyable>("ProcessManagerCommand")
    .def("execute",pure_virtual(&ProcessManager::Command::execute))
    ;

} // end of declarestensor
