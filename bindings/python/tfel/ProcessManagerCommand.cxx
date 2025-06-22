/*!
 * \file  bindings/python/tfel/ProcessManagerCommand.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 31 aoû 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

#include <TFEL/System/ProcessManager.hxx>

struct ProcessManagerCommandWrapper final
    : tfel::system::ProcessManager::Command,
      boost::python::wrapper<tfel::system::ProcessManager::Command> {
  bool execute(const tfel::system::ProcessManager::StreamId,
               const tfel::system::ProcessManager::StreamId) override final;
  ~ProcessManagerCommandWrapper() override;
};  // end of struct Command

bool ProcessManagerCommandWrapper::execute(
    const tfel::system::ProcessManager::StreamId i,
    const tfel::system::ProcessManager::StreamId o) {
  return this->get_override("execute")(i, o);
}  // end of ProcessManagerCommandWrapper::execute

ProcessManagerCommandWrapper::~ProcessManagerCommandWrapper() = default;

void declareProcessManagerCommand();

void declareProcessManagerCommand() {
  using namespace std;
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::system;

  class_<ProcessManagerCommandWrapper, boost::noncopyable>(
      "ProcessManagerCommand")
      .def("execute", pure_virtual(&ProcessManager::Command::execute));

}  // end of declarestensor
