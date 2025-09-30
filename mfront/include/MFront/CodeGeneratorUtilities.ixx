/*!
 * \file   mfront/include/MFront/CodeGeneratorUtilities.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date 03/06/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CODEGENERATORUTILITIES_IXX
#define LIB_MFRONT_CODEGENERATORUTILITIES_IXX

#include <ostream>

namespace mfront {

  template <typename ValueType>
  void exportSymbol(std::ostream& os,
                    const std::string_view t,
                    const std::string_view n,
                    const ValueType& v) {
    os << "MFRONT_EXPORT_SYMBOL(" << t << ", " << n << ", " << v << ");\n\n";
  }  // end of exportSymbol

}  // end of namespace mfront

#endif /* LIB_MFRONT_CODEGENERATORUTILITIES_IXX */
