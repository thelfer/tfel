/*!
 * \file   mfront/include/MFront/DSLUtilities.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date 03/06/2022
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DSLUTILITIES_IXX
#define LIB_MFRONT_DSLUTILITIES_IXX

namespace mfront {

  template <typename ValueType>
  void exportSymbol(std::ostream& os,
                    const std::string& t,
                    const std::string& n,
                    const ValueType& v) {
    os << "MFRONT_EXPORT_SYMBOL(" << t << ", " << n << ", " << v << ");\n\n";
  }  // end of exportSymbol

}  // end of namespace mfront

#endif /* LIB_MFRONT_DSLUTILITIES_IXX */
