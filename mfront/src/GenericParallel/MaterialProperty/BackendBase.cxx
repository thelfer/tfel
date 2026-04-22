/*!
 * \file   BackendBase.cxx
 * \brief  This file implements the `Backend` class
 * \author Thomas Helfer
 * \date   07/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "MFront/VariableDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/GenericParallelMaterialPropertyInterface.hxx"
#include "MFront/GenericParallel/MaterialProperty/BackendBase.hxx"

namespace mfront::generic_parallel::material_property {

  BackendBase::BackendBase() = default;

  void BackendBase::writeProxyObjects(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {}  // end of writeProxyObject

  void BackendBase::writePhysicalBounds(std::ostream& os,
                                        const VariableDescription& v,
                                        const size_t i,
                                        const bool useQuantities) const {
    if (!v.hasPhysicalBounds()) {
      return;
    }
    const auto& b = v.getPhysicalBounds();
    const auto to_string = useQuantities
                               ? "std::to_string(" + v.name + ".getValue())"
                               : "std::to_string(" + v.name + ")";
    if ((b.boundsType == VariableBoundsDescription::LOWER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      os << "if(" << v.name << " < " << v.type << "(" << b.lowerBound << ")){\n"
         << "mfront_bounds_statuses[" << 2 * i << "] = -1;\n"
         << "return;\n"
         << "}\n";
    }
    if ((b.boundsType == VariableBoundsDescription::UPPER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      os << "if(" << v.name << " > " << v.type << "(" << b.upperBound << ")){\n"
         << "mfront_bounds_statuses[" << 2 * i << "] = 1;\n"
         << "return;\n"
         << "}\n";
    }
  }  // end of writePhysicalBounds

  void BackendBase::writeBounds(std::ostream& os,
                                const std::string& iucname,
                                const VariableDescription& v,
                                const size_t i,
                                const bool useQuantities) const {
    if (!v.hasBounds()) {
      return;
    }
    const auto to_string = useQuantities
                               ? "std::to_string(" + v.name + ".getValue())"
                               : "std::to_string(" + v.name + ")";
    const auto& b = v.getBounds();
    if ((b.boundsType == VariableBoundsDescription::LOWER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      os << "if(" << v.name << " < " << v.type << "(" << b.lowerBound << ")){\n"
         << "mfront_bounds_statuses[" << 2 * i + 1 << "] = -1;\n"
         << "if (mfront_out_of_bounds_policy==" << iucname
         << "_STRICT_POLICY){\n"
         << "return;\n"
         << "}\n"
         << "}\n";
    }
    if ((b.boundsType == VariableBoundsDescription::UPPER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      os << "if(" << v.name << " > " << v.type << "(" << b.upperBound << ")){\n"
         << "mfront_bounds_statuses[" << 2 * i + 1 << "] = 1;\n"
         << "if (mfront_out_of_bounds_policy==" << iucname
         << "_STRICT_POLICY){\n"
         << "return;\n"
         << "}\n"
         << "}\n";
    }
  }  // end of writeBounds

  void BackendBase::writeBoundsStatusesAnalysis(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    if (!((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs)) ||
          (mpd.output.hasPhysicalBounds()) || (mpd.output.hasBounds()))) {
      return;
    }
    const auto iucname = i.getInterfaceNameInUpperCase();
    const auto prefix = i.getOutOfBoundsPolicyEnumerationPrefix();
    os << "#ifndef NO_" << iucname << "_BOUNDS_CHECK\n";
    auto write_physical_bounds_violation = [&os](const VariableDescription& v,
                                                 std::size_t idx) {
      os << "if(mfront_bounds_statuses[" << 2 * idx << "] > 0){\n"
         << "mfront_report(\"" << v.name
         << " is over its physical upper bound at at least one "
         << "point\");\n"
         << "mfront_output_status->status = -1;\n"
         << "mfront_output_status->bounds_status = -" << idx + 1 << ";\n"
         << "return;\n"
         << "}\n";
      os << "if(mfront_bounds_statuses[" << 2 * idx << "] < 0){\n"
         << "mfront_report(\"" << v.name
         << " is below its physical lower bound at at least one "
         << "point\");\n"
         << "mfront_output_status->status = -1;\n"
         << "mfront_output_status->bounds_status = -" << idx + 1 << ";\n"
         << "return;\n"
         << "}\n";
    };
    for (decltype(mpd.inputs.size()) idx = 0; idx != mpd.inputs.size(); ++idx) {
      const auto& v = mpd.inputs[idx];
      if (!v.hasPhysicalBounds()) {
        continue;
      }
      write_physical_bounds_violation(v, idx);
    }
    if (mpd.output.hasPhysicalBounds()) {
      write_physical_bounds_violation(mpd.output, mpd.inputs.size());
    }
    if ((hasBounds(mpd.inputs)) || (mpd.output.hasBounds())) {
      auto write_bounds_violation = [&os, &prefix](const VariableDescription& v,
                                                   std::size_t idx) {
        os << "if(mfront_bounds_statuses[" << 2 * idx + 1 << "] > 0){\n"
           << "mfront_report(\"" << v.name
           << " is over its upper bound at at least one point\");\n"
           << "mfront_output_status->bounds_status = " << idx + 1 << ";\n"
           << "if(mfront_out_of_bounds_policy==" << prefix
           << "_STRICT_POLICY){\n"
           << "mfront_output_status->status = -1;\n"
           << "return;\n"
           << "}\n"
           << "}\n";
        os << "if(mfront_bounds_statuses[" << 2 * idx + 1 << "] < 0){\n"
           << "mfront_report(\"" << v.name
           << " is below its lower bound at at least one point\");\n"
           << "mfront_output_status->bounds_status = " << idx + 1 << ";\n"
           << "if(mfront_out_of_bounds_policy==" << prefix
           << "_STRICT_POLICY){\n"
           << "mfront_output_status->status = -1;\n"
           << "return;\n"
           << "}\n"
           << "}\n";
      };
      os << "if((mfront_out_of_bounds_policy==" << prefix
         << "_STRICT_POLICY)||\n"
         << "(mfront_out_of_bounds_policy==" << prefix
         << "_WARNING_POLICY)){\n";
      for (decltype(mpd.inputs.size()) idx = 0; idx != mpd.inputs.size();
           ++idx) {
        const auto& v = mpd.inputs[idx];
        if (!v.hasBounds()) {
          continue;
        }
        write_bounds_violation(v, idx);
      }
      if (mpd.output.hasBounds()) {
        write_bounds_violation(mpd.output, mpd.inputs.size());
      }
      os << "}\n";
    }
    os << "#endif /* NO_" << iucname << "_BOUNDS_CHECK */\n";
  }

  BackendBase::~BackendBase() noexcept = default;

}  // end of namespace mfront::generic_parallel::material_property
