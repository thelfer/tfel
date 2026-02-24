
/*!
 * \file  bindings/python/tfel/ProcessManagerCommand.cxx
 * \brief
 * \author Thomas Helfer
 * \date 31/08/2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <TFEL/System/ProcessManager.hxx>

struct ProcessManagerCommandWrapper final
    : tfel::system::ProcessManager::Command {
  bool execute(const tfel::system::ProcessManager::StreamId,
               const tfel::system::ProcessManager::StreamId) override final;
  ~ProcessManagerCommandWrapper() override;
};  // end of struct Command

bool ProcessManagerCommandWrapper::execute(
    const tfel::system::ProcessManager::StreamId in,
    const tfel::system::ProcessManager::StreamId out) {
  using namespace tfel::system;
  using CommandClass = ProcessManager::Command;
  PYBIND11_OVERRIDE_PURE(bool, CommandClass, execute, in, out);
}  // end of ProcessManagerCommandWrapper::execute

ProcessManagerCommandWrapper::~ProcessManagerCommandWrapper() = default;

void declareProcessManagerCommand(pybind11::module_&);

void declareProcessManagerCommand(pybind11::module_& m) {
  using namespace tfel::system;
  pybind11::class_<ProcessManager::Command, ProcessManagerCommandWrapper>(
      m, "ProcessManagerCommand")
      .def("execute", &ProcessManager::Command::execute);
}  // end of declarestensor
